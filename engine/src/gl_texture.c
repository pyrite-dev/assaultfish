#include <GearBox/GL.h>

#include <GearBox/File.h>

#include <stb_image.h>

void GBGLTexturePrepare(GBGL gl, GLuint* texture, unsigned char* rgba, int width, int height) {
	int y;

	/* image gets flipped here... because opengl coord system is quite confusing */
	for(y = 0; y < height / 2; y++) {
		int x;
		for(x = 0; x < width; x++) {
			int p;
			for(p = 0; p < 4; p++) {
				unsigned char* s1 = &rgba[(y * width + x) * 4 + p];
				unsigned char* s2 = &rgba[((height - 1 - y) * width + x) * 4 + p];
				unsigned char  p1 = *s1;
				unsigned char  p2 = *s2;

				*s2 = p1;
				*s1 = p2;
			}
		}
	}

	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GBGLTextureEnable(GBGL gl, GLuint texture) {
	glBindTexture(GL_TEXTURE_2D, texture);
	glEnable(GL_TEXTURE_2D);
}

void GBGLTextureDisable(GBGL gl, GLuint texture) {
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

int GBGLTextureLoadFile(GBGL gl, GLuint* texture, int* width, int* height, const char* filename) {
	GBFile	       f = GBFileOpen(gl->engine, filename);
	unsigned char* data;
	unsigned char* rgba;
	unsigned long  sz;
	int	       ch;
	int	       st = 1;
	int	       w, h;
	if(f == NULL) return 0;

	sz = GBFileSize(f);

	data = malloc(sz);
	GBFileRead(f, data, sz);
	GBFileClose(f);

	if((rgba = stbi_load_from_memory(data, sz, &w, &h, &ch, 4)) == NULL) {
		st = 0;
	} else {
		GBGLTexturePrepare(gl, texture, rgba, w, h);

		free(rgba);
	}

	if(width != NULL) *width = w;
	if(height != NULL) *height = h;

	free(data);

	return st;
}

void GBGLTextureSet(GBGL gl, GLuint texture) {
	glBindTexture(GL_TEXTURE_2D, texture);
	if(texture == 0) {
		glDisable(GL_TEXTURE_2D);
	} else {
		glEnable(GL_TEXTURE_2D);
	}
}

void GBGLTextureDelete(GBGL gl, GLuint texture) {
	glDeleteTextures(1, &texture);
}
