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
		glUniform1i(glGetUniformLocation(gl->shadow_shader, "shadow_texture"), 1);
		glUniform1i(glGetUniformLocation(gl->shadow_shader, "current_texture"), 0);
	}
}
