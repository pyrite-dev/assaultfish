#include <GearSrc/GL.h>

void GSGLTexturePrepare(GSGL gl, GLuint* texture, unsigned char* rgba, int width, int height) {
	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GSGLTextureEnable(GSGL gl, GLuint texture) {
	glBindTexture(GL_TEXTURE_2D, texture);
	glEnable(GL_TEXTURE_2D);
}

void GSGLTextureDisable(GSGL gl, GLuint texture) {
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GSGLTextureSet(GSGL gl, GLuint texture) {
	glBindTexture(GL_TEXTURE_2D, texture);
	if(texture == 0) {
		glDisable(GL_TEXTURE_2D);
	} else {
		glEnable(GL_TEXTURE_2D);
	}
}

void GSGLTextureDelete(GSGL gl, GLuint texture) {
	glDeleteTextures(1, &texture);
}
