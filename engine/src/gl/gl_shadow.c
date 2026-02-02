#include <GearSrc/GL.h>

#include <GearSrc/Math.h>

#define SHADOW_CULL

void GSGLShadowInit(GSGL gl) {
	GLfloat color[] = {1, 1, 1, 1};
	int	i;

	glGenTextures(GSGL_SHADOW_CASCADES, gl->shadow_texture);

	for(i = 0; i < GSGL_SHADOW_CASCADES; i++) {
		glBindTexture(GL_TEXTURE_2D, gl->shadow_texture[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, GSGLMaxShadowDistance, GSGLMaxShadowDistance, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

		glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	if(GSGLShaderPrepare(gl, &gl->shadow_shader, "base:/shader/shadow.vs", "base:/shader/shadow.fs")) {
		glUseProgram(gl->shadow_shader);
		glUniform1i(glGetUniformLocation(gl->shadow_shader, "shadow_map0"), 5);
		glUniform1i(glGetUniformLocation(gl->shadow_shader, "shadow_map1"), 6);
		glUniform1i(glGetUniformLocation(gl->shadow_shader, "shadow_map2"), 7);

		glUniform1i(glGetUniformLocation(gl->shadow_shader, "current_texture"), 0);
		glUniform1f(glGetUniformLocation(gl->shadow_shader, "enable_lighting"), 1);
		glUniform1f(glGetUniformLocation(gl->shadow_shader, "enable_texture"), 0);
		glUseProgram(0);

		gl->shadow_use_shader = 1;
	} else {
		glDeleteTextures(GSGL_SHADOW_CASCADES, gl->shadow_texture);

		gl->shadow_use_shader = 0;
	}
}

GSBool GSGLShadowBegin(GSGL gl) {
	if(!gl->shadow_use_shader) return GSFalse;

	glMatrixMode(GL_PROJECTION);
	glGetDoublev(GL_PROJECTION_MATRIX, gl->shadow_old_projection);

	glMatrixMode(GL_MODELVIEW);
	glGetDoublev(GL_MODELVIEW_MATRIX, gl->shadow_old_modelview);

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glDisable(GL_LIGHTING);

#ifdef SHADOW_CULL
	glCullFace(GL_FRONT);
#else
	glDisable(GL_CULL_FACE);
#endif

	glPolygonOffset(1.1f, 4.0f);
	glEnable(GL_POLYGON_OFFSET_FILL);

	return GSTrue;
}

int GSGLShadowCascadeSteps(GSGL gl){
	if(gl->engine->client->light0[3]) return 1;

	return GSGL_SHADOW_CASCADES;
}

void GSGLShadowCascade(GSGL gl, int cascade) {
	GSVector3 zero = {0, 0, 0};
	GSNumber  size;

	GSGLClear(gl);
	glViewport(0, 0, GSGLMaxShadowDistance, GSGLMaxShadowDistance);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Simple cascade sizing
	if(cascade == 0) size = 10;
	else if(cascade == 1) size = 40;
	else size = 100; // Covers max distance roughly

	// If light is directional (light0[3] == 0)
	// Current existing code logic:
	if(gl->engine->client->light0[3]) {
		// Point/Spot light - no cascade support implies here?
		// Fallback to perspective for all cascades?
		// Or just use the original logic.
		GSGLCameraPerspective(gl, GSGLMaxShadowDistance, GSGLMaxShadowDistance);
	} else {
		// Directional
		glOrtho(-size, size, -size, size, -size * 2, size * 2);
	}

	glGetDoublev(GL_PROJECTION_MATRIX, gl->shadow_projection[cascade]);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if(gl->engine->client->light0[3]) {
		GSGLCameraLookAt(gl, gl->engine->client->light0, zero);
	} else {
		GSVector3 n;
		int	  i;

		for(i = 0; i < 3; i++) n[i] = gl->engine->client->light0[i];

		GSMathNormalize3(n);

		for(i = 0; i < 3; i++) {
			n[i] *= size; // Move light pos away based on size to ensure coverage
			n[i] += gl->engine->client->position[i];
		}

		GSGLCameraLookAt(gl, n, gl->engine->client->position);
	}

	glGetDoublev(GL_MODELVIEW_MATRIX, gl->shadow_modelview[cascade]);
}

void GSGLShadowCopy(GSGL gl, int cascade) {
	if(cascade < 0 || cascade >= GSGL_SHADOW_CASCADES) return;

	glBindTexture(GL_TEXTURE_2D, gl->shadow_texture[cascade]);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, GSGLMaxShadowDistance, GSGLMaxShadowDistance);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GSGLShadowApply(GSGL gl) {
	GSMatrix4x4 in, in2, out, out2;
	GLdouble    mat[16];
	int	    i, c;

	glDisable(GL_POLYGON_OFFSET_FILL);

	glViewport(0, 0, gl->engine->width, gl->engine->height);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glEnable(GL_LIGHTING);

#ifdef SHADOW_CULL
	glCullFace(GL_BACK);
#else
	glEnable(GL_CULL_FACE);
#endif

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMultMatrixd(gl->shadow_old_projection);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixd(gl->shadow_old_modelview);

	// Calculate texture matrices for each cascade
	for(c = 0; c < GSGL_SHADOW_CASCADES; c++) {
		glActiveTexture(GL_TEXTURE5 + c);
		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();

		for(i = 0; i < 16; i++) in[i] = gl->shadow_old_modelview[i];
		GSMathColumnToRow4x4(in2, in);
		GSMathInvert4x4(out, in2);
		GSMathRowToColumn4x4(out2, out);
		for(i = 0; i < 16; i++) mat[i] = out2[i];

		glTranslated(0.5, 0.5, 0.5);
		glScaled(0.5, 0.5, 0.5);
		glMultMatrixd(gl->shadow_projection[c]);
		glMultMatrixd(gl->shadow_modelview[c]);
		glMultMatrixd(mat);
	}
	glActiveTexture(GL_TEXTURE0);
	glMatrixMode(GL_MODELVIEW);

	GSGLClear(gl); // Clear for main render

	GSGLShadowEnable(gl);
}

void GSGLShadowDisable(GSGL gl) {
	int i;
	glDisable(GL_LIGHTING);
	if(gl->shadow_use_shader) {
		glUseProgram(0);

		for(i = 0; i < GSGL_SHADOW_CASCADES; i++) {
			glActiveTexture(GL_TEXTURE5 + i);
			glDisable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		glActiveTexture(GL_TEXTURE0);
	}
}

void GSGLShadowEnable(GSGL gl) {
	int i;
	glEnable(GL_LIGHTING);
	if(gl->shadow_use_shader) {
		glUseProgram(gl->shadow_shader);

		for(i = 0; i < GSGL_SHADOW_CASCADES; i++) {
			glActiveTexture(GL_TEXTURE5 + i);
			glBindTexture(GL_TEXTURE_2D, gl->shadow_texture[i]);
			glEnable(GL_TEXTURE_2D);
		}
		glActiveTexture(GL_TEXTURE0);
	}
}

void GSGLShadowEnd(GSGL gl) {
	int i;
	if(!gl->shadow_use_shader) return;

	GSGLShadowDisable(gl);

	// Reset texture matrices
	for(i = 0; i < GSGL_SHADOW_CASCADES; i++) {
		glActiveTexture(GL_TEXTURE5 + i);
		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
	}
	glActiveTexture(GL_TEXTURE0);
	glMatrixMode(GL_MODELVIEW);
}

void GSGLShadowDeinit(GSGL gl) {
	if(gl->shadow_use_shader) {
		glDeleteTextures(GSGL_SHADOW_CASCADES, gl->shadow_texture);
		glDeleteProgram(gl->shadow_shader);
	}
}
