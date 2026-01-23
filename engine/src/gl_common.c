#include <GearSrc/GL.h>

#include <GearSrc/String.h>
#include <GearSrc/File.h>

#include <stb_image.h>

GSBool GSGLTextureLoadFile(GSGL gl, GLuint* texture, int* width, int* height, const char* filename) {
	GSFile	       f = GSFileOpen(gl->engine, filename);
	unsigned char* data;
	unsigned char* rgba;
	unsigned long  sz;
	int	       ch;
	GSBool	       st = GSTrue;
	int	       w, h;
	if(f == NULL) return GSFalse;

	sz = GSFileSize(f);

	data = malloc(sz);
	GSFileRead(f, data, sz);
	GSFileClose(f);

	if((rgba = stbi_load_from_memory(data, sz, &w, &h, &ch, 4)) == NULL) {
		st = GSFalse;
	} else {
		GSGLTexturePrepare(gl, texture, rgba, w, h);

		free(rgba);
	}

	if(width != NULL) *width = w;
	if(height != NULL) *height = h;

	free(data);

	return st;
}

GSBool GSGLTextureTry(GSGL gl, GLuint* texture, int* width, int* height, const char* prefix) {
	char*  png  = GSStringConcat(prefix, ".png");
	char*  bmp  = GSStringConcat(prefix, ".bmp");
	char*  jpg  = GSStringConcat(prefix, ".jpg");
	char*  jpeg = GSStringConcat(prefix, ".jpeg");
	GSBool st   = GSTrue;

	if(GSGLTextureLoadFile(gl, texture, width, height, prefix)) {
	} else if(GSGLTextureLoadFile(gl, texture, width, height, png)) {
	} else if(GSGLTextureLoadFile(gl, texture, width, height, bmp)) {
	} else if(GSGLTextureLoadFile(gl, texture, width, height, jpg)) {
	} else if(GSGLTextureLoadFile(gl, texture, width, height, jpeg)) {
	} else {
		st = GSFalse;
	}

	free(jpeg);
	free(jpg);
	free(bmp);
	free(png);

	return st;
}

void GSGLTextEx(GSGL gl, double x, double y, double sx, double sy, const char* text) {
	double px = x;
	double py = y;
	int    i;
	double gw = gl->engine->client->font_width / 16 * sx;
	double gh = gl->engine->client->font_height / 16 * sy;

	GSGLBegin2D(gl);
	GSGLTextureSet(gl, gl->engine->client->font);

	for(i = 0; text[i] != 0; i++) {
		GSVector2 v[4];
		GSVector2 t[4];
		double	  gx = text[i] % 16;
		double	  gy = text[i] / 16;

		gx = gx / 16;
		gy = gy / 16;

		v[0][0] = px, v[0][1] = py;

		v[1][0] = px, v[1][1] = py + gh;

		v[2][0] = px + gw, v[2][1] = py + gh;

		v[3][0] = px + gw, v[3][1] = py;

		t[0][0] = gx, t[0][1] = gy;
		t[1][0] = gx, t[1][1] = gy + 1.0 / 16;
		t[2][0] = gx + 1.0 / 16, t[2][1] = gy + 1.0 / 16;
		t[3][0] = gx + 1.0 / 16, t[3][1] = gy;

		GSGLPolygon2D(gl, 4, v, t);

		px += gw;
	}

	GSGLTextureSet(gl, 0);
	GSGLEnd2D(gl);
}

void GSGLText(GSGL gl, double x, double y, const char* text) {
	GSGLTextEx(gl, x, y, 1, 1, text);
}

double GSGLTextWidth(GSGL gl, const char* text) {
	return gl->engine->client->font_width / 16 * strlen(text);
}

double GSGLTextHeight(GSGL gl, const char* text) {
	return gl->engine->client->font_height / 16;
}
