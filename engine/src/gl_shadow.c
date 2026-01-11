#include <GearBox/GL.h>

#include <GearBox/Math.h>

#define SHADOW_WIDTH 512
#define SHADOW_HEIGHT 512
#define SHADOW_CULL

void GBGLShadowInit(GBGL gl) {
	glGenTextures(1, &gl->shadow_texture);
	glBindTexture(GL_TEXTURE_2D, gl->shadow_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);

	glBindTexture(GL_TEXTURE_2D, 0);

	if(GBGLShaderPrepare(gl, &gl->shadow_shader, "base:/shader/shadow.vs", "base:/shader/shadow.fs")) {
		glUseProgram(gl->shadow_shader);
		glUniform1i(glGetUniformLocation(gl->shadow_shader, "depth_texture"), 7);
		glUniform1i(glGetUniformLocation(gl->shadow_shader, "current_texture"), 0);
		glUniform1f(glGetUniformLocation(gl->shadow_shader, "enable_lighting"), 1);
		glUseProgram(0);

		gl->shadow_use_shader = 1;
	} else {
		glDeleteTextures(1, &gl->shadow_texture);

		gl->shadow_use_shader = 0;
	}
}

int GBGLShadowBeforeMapping(GBGL gl) {
	GBVector3 zero = {0, 0, 0};

	if(!gl->shadow_use_shader) return 0;

	GBGLClear(gl);

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);

	glMatrixMode(GL_PROJECTION);
	glGetDoublev(GL_PROJECTION_MATRIX, gl->shadow_old_projection);
	glLoadIdentity();
	GBGLCameraPerspective(gl, SHADOW_WIDTH, SHADOW_HEIGHT);

	glGetDoublev(GL_PROJECTION_MATRIX, gl->shadow_projection);

	glMatrixMode(GL_MODELVIEW);
	glGetDoublev(GL_MODELVIEW_MATRIX, gl->shadow_old_modelview);
	glLoadIdentity();
	GBGLCameraLookAt(gl, gl->engine->client->light0, zero);

	glGetDoublev(GL_MODELVIEW_MATRIX, gl->shadow_modelview);

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	glDisable(GL_LIGHTING);

#ifdef SHADOW_CULL
	glCullFace(GL_FRONT);
#else
	glDisable(GL_CULL_FACE);
#endif

	glPolygonOffset(1.1f, 4.0f);
	glEnable(GL_POLYGON_OFFSET_FILL);

	return 1;
}

void GBGLShadowAfterMapping(GBGL gl) {
	GBMatrix4x4 in, out;
	GLdouble    mat[16];
	int	    i;

	glDisable(GL_POLYGON_OFFSET_FILL);

	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, gl->shadow_texture);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);

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

	glActiveTexture(GL_TEXTURE7);
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();

	for(i = 0; i < 16; i++) in[i] = gl->shadow_old_modelview[i];
	GBMathInvert4x4(out, in);
	for(i = 0; i < 16; i++) mat[i] = out[i];

	glTranslated(0.5, 0.5, 0.5);
	glScaled(0.5, 0.5, 0.5);
	glMultMatrixd(gl->shadow_projection);
	glMultMatrixd(gl->shadow_modelview);
	glMultMatrixd(mat);
	glActiveTexture(GL_TEXTURE0);

	glMatrixMode(GL_MODELVIEW);

	GBGLClear(gl);

	GBGLShadowEnable(gl);
}

void GBGLShadowDisable(GBGL gl) {
	glDisable(GL_LIGHTING);
	if(gl->shadow_use_shader) {
		glUseProgram(0);

		glActiveTexture(GL_TEXTURE7);
		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE0);
	}
}

void GBGLShadowEnable(GBGL gl) {
	glEnable(GL_LIGHTING);
	if(gl->shadow_use_shader) {
		glUseProgram(gl->shadow_shader);

		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D, gl->shadow_texture);
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
	}
}

void GBGLShadowEnd(GBGL gl) {
	if(!gl->shadow_use_shader) return;

	GBGLShadowDisable(gl);

	glActiveTexture(GL_TEXTURE7);
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);

	glActiveTexture(GL_TEXTURE7);
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glActiveTexture(GL_TEXTURE0);

	glMatrixMode(GL_MODELVIEW);
}

void GBGLShadowDeinit(GBGL gl) {
	if(gl->shadow_use_shader) {
		glDeleteTextures(1, &gl->shadow_texture);
		glDeleteProgram(gl->shadow_shader);
	}
}
