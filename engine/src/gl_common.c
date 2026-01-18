#include <GearSrc/GL.h>

#include <GearSrc/String.h>
#include <GearSrc/File.h>

#include <stb_image.h>

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

int GSGLTextureTry(GSGL gl, GLuint* texture, int* width, int* height, const char* prefix) {
	char* png  = GSStringConcat(prefix, ".png");
	char* bmp  = GSStringConcat(prefix, ".bmp");
	char* jpg  = GSStringConcat(prefix, ".jpg");
	char* jpeg = GSStringConcat(prefix, ".jpeg");
	int   st   = 1;

	if(GSGLTextureLoadFile(gl, texture, width, height, prefix)) {
	} else if(GSGLTextureLoadFile(gl, texture, width, height, png)) {
	} else if(GSGLTextureLoadFile(gl, texture, width, height, bmp)) {
	} else if(GSGLTextureLoadFile(gl, texture, width, height, jpg)) {
	} else if(GSGLTextureLoadFile(gl, texture, width, height, jpeg)) {
	} else {
		st = 0;
	}

	free(jpeg);
	free(jpg);
	free(bmp);
	free(png);

	return st;
}
