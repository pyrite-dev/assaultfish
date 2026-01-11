#include <GearSrc/GL.h>

#include <GearSrc/File.h>

#include <stb_image.h>

void GSGLTexturePrepare(GSGL gl, GLuint* texture, unsigned char* rgba, int width, int height) {
	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
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

int GSGLTextureLoadFile(GSGL gl, GLuint* texture, int* width, int* height, const char* filename) {
	GSFile	       f = GSFileOpen(gl->engine, filename);
	unsigned char* data;
	unsigned char* rgba;
	unsigned long  sz;
	int	       ch;
	int	       st = 1;
	int	       w, h;
	if(f == NULL) return 0;

	sz = GSFileSize(f);

	data = malloc(sz);
	GSFileRead(f, data, sz);
	GSFileClose(f);

	if((rgba = stbi_load_from_memory(data, sz, &w, &h, &ch, 4)) == NULL) {
		st = 0;
	} else {
		GSGLTexturePrepare(gl, texture, rgba, w, h);

		free(rgba);
	}

	if(width != NULL) *width = w;
	if(height != NULL) *height = h;

	free(data);

	return st;
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
