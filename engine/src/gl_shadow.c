#include <GearBox/GL.h>

#define SHADOW_WIDTH 1024
#define SHADOW_HEIGHT 1024

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
		glUniform1i(glGetUniformLocation(gl->shadow_shader, "depth_texture"), 1);
		glUniform1i(glGetUniformLocation(gl->shadow_shader, "current_texture"), 0);
	}
}

void GBGLShadowBeforeMapping(GBGL gl) {
	GBVector3 zero = {0, 0, 0};

	glClear(GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);

	glMatrixMode(GL_PROJECTION);
	glGetDoublev(GL_PROJECTION_MATRIX, gl->shadow_old_projection);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glGetDoublev(GL_MODELVIEW_MATRIX, gl->shadow_old_modelview);
	glLoadIdentity();
	GBGLPerspective(gl, SHADOW_WIDTH, SHADOW_HEIGHT);
	GBGLLookAt(gl, gl->engine->client->light0, zero);

	glGetDoublev(GL_MODELVIEW_MATRIX, gl->shadow_modelview);

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	glDisable(GL_LIGHTING);

	glCullFace(GL_FRONT);
}

void GBGLShadowAfterMapping(GBGL gl) {
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gl->shadow_texture);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);

	glViewport(0, 0, gl->engine->width, gl->engine->height);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glEnable(GL_LIGHTING);
	glCullFace(GL_BACK);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMultMatrixd(gl->shadow_old_projection);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixd(gl->shadow_old_modelview);

	glActiveTexture(GL_TEXTURE1);
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();

	glTranslated(0.5, 0.5, 0.5);
	glScaled(0.5, 0.5, 0.5);
	glMultMatrixd(gl->shadow_modelview);
	glMultMatrixd(gl->shadow_old_modelview);
	glActiveTexture(GL_TEXTURE0);

	glMatrixMode(GL_MODELVIEW);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gl->shadow_texture);
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GBGLShadowEnd(GBGL gl) {
	glActiveTexture(GL_TEXTURE1);
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);

	glActiveTexture(GL_TEXTURE1);
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glActiveTexture(GL_TEXTURE0);

	glMatrixMode(GL_MODELVIEW);
}
