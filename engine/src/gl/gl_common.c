#include <GearSrc/GL.h>

#include <GearSrc/String.h>
#include <GearSrc/File.h>
#include <GearSrc/Math.h>

#include <stb_image.h>

GSBool GSGLTextureLoadFileEx(GSGL gl, GLuint* texture, int* width, int* height, const char* filename, int expand) {
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
		GSGLTexturePrepareEx(gl, texture, rgba, w, h, expand);

		free(rgba);
	}

	if(width != NULL) *width = w;
	if(height != NULL) *height = h;

	free(data);

	return st;
}

GSBool GSGLTextureLoadFile(GSGL gl, GLuint* texture, int* width, int* height, const char* filename) {
	return GSGLTextureLoadFileEx(gl, texture, width, height, filename, 1);
}

GSBool GSGLTextureTryEx(GSGL gl, GLuint* texture, int* width, int* height, const char* prefix, int expand) {
	char*  png  = GSStringConcat(prefix, ".png");
	char*  bmp  = GSStringConcat(prefix, ".bmp");
	char*  jpg  = GSStringConcat(prefix, ".jpg");
	char*  jpeg = GSStringConcat(prefix, ".jpeg");
	GSBool st   = GSTrue;

	if(GSGLTextureLoadFileEx(gl, texture, width, height, prefix, expand)) {
	} else if(GSGLTextureLoadFileEx(gl, texture, width, height, png, expand)) {
	} else if(GSGLTextureLoadFileEx(gl, texture, width, height, bmp, expand)) {
	} else if(GSGLTextureLoadFileEx(gl, texture, width, height, jpg, expand)) {
	} else if(GSGLTextureLoadFileEx(gl, texture, width, height, jpeg, expand)) {
	} else {
		st = GSFalse;
	}

	free(jpeg);
	free(jpg);
	free(bmp);
	free(png);

	return st;
}

GSBool GSGLTextureTry(GSGL gl, GLuint* texture, int* width, int* height, const char* prefix) {
	return GSGLTextureTryEx(gl, texture, width, height, prefix, 1);
}

void GSGLTextBold(GSGL gl, GSBool bold) {
	gl->bold = bold;
}

void GSGLTextEx(GSGL gl, GSNumber x, GSNumber y, const char* text, GSNumber sx, GSNumber sy) {
	GSNumber px = x;
	GSNumber py = y;
	int	 i;
	GSClient client = gl->engine->client;
	GSNumber gw	= (gl->bold ? client->glyph_bold_width : client->glyph_normal_width) * sx;
	GSNumber gh	= (gl->bold ? client->glyph_bold_height : client->glyph_normal_height) * sy;
	GSNumber sw	= (GSNumber)(gl->bold ? client->glyph_bold_width : client->glyph_normal_width) / (gl->bold ? client->font_bold_width : client->font_normal_width);
	GSNumber sh	= (GSNumber)(gl->bold ? client->glyph_bold_height : client->glyph_normal_height) / (gl->bold ? client->font_bold_height : client->font_normal_height);

	GSGLBegin2D(gl);
	GSGLTextureSet(gl, gl->bold ? client->font_bold : client->font_normal);

	for(i = 0; text[i] != 0; i++) {
		GSVector2 v[4];
		GSVector2 t[4];
		GSNumber  gx = text[i] % 16;
		GSNumber  gy = text[i] / 16;

		gx = gx * sw;
		gy = gy * sh;

		v[0][0] = px, v[0][1] = py;
		v[1][0] = v[0][0], v[1][1] = py + gh;
		v[2][0] = px + gw, v[2][1] = v[1][1];
		v[3][0] = v[2][0], v[3][1] = v[0][1];

		t[0][0] = gx, t[0][1] = gy;
		t[1][0] = t[0][0], t[1][1] = gy + sh;
		t[2][0] = gx + sw, t[2][1] = t[1][1];
		t[3][0] = t[2][0], t[3][1] = t[0][1];

		GSGLPolygon2D(gl, 4, v, t);

		px += gw;
	}

	GSGLTextureSet(gl, 0);
	GSGLEnd2D(gl);
}

void GSGLText(GSGL gl, GSNumber x, GSNumber y, const char* text) {
	GSGLTextEx(gl, x, y, text, 1, 1);
}

double GSGLTextWidth(GSGL gl, const char* text) {
	return (gl->bold ? gl->engine->client->glyph_bold_width : gl->engine->client->glyph_normal_width) * strlen(text);
}

double GSGLTextHeight(GSGL gl, const char* text) {
	return (gl->bold ? gl->engine->client->glyph_bold_height : gl->engine->client->glyph_normal_height);
}

void GSGLTexturePrepareEx(GSGL gl, GLuint* texture, unsigned char* rgba, int width, int height, int expand) {
	int	       w, h;
	unsigned char* d;
	int	       i, y, x;
	GSNumber       sx, sy;

	w = GSMathClosestPOT(width);
	h = GSMathClosestPOT(height);

	d = malloc(w * h * 4);

	if(expand) {
		sx = (GSNumber)width / w;
		sy = (GSNumber)height / h;

		for(y = 0; y < h; y++) {
			int fy = y * sy;
			for(x = 0; x < w; x++) {
				int	       fx = x * sx;
				unsigned char* fp = &rgba[(fy * width + fx) * 4];
				unsigned char* tp = &d[(y * w + x) * 4];

				for(i = 0; i < 4; i++) {
					tp[i] = fp[i];
				}
			}
		}
	} else {
		memset(d, 0, w * h * 4);

		for(y = 0; y < height; y++) {
			for(x = 0; x < width; x++) {
				for(i = 0; i < 4; i++) d[(y * w + x) * 4 + i] = rgba[(y * width + x) * 4 + i];
			}
		}
	}

	GSGLTextureLoad(gl, texture, d, w, h);

	free(d);
}

void GSGLTexturePrepare(GSGL gl, GLuint* texture, unsigned char* rgba, int width, int height) {
	GSGLTexturePrepareEx(gl, texture, rgba, width, height, 1);
}

/* who even uses this other than second reality? :) */
void GSGLTetrakis(GSGL gl, GSNumber scale, GSVector4 col1, GSVector4 col2) {
	GSVector3 v[] = {
	    {0, 1, 0},
	    {-0.75, 0.75, -0.75},
	    {0.75, 0.75, -0.75}};
	GSVector3 n;
	GSVector3 rot2 = {0, 90, 0};
	GSVector3 rot3 = {0, 0, 90};
	GSVector3 rot4 = {90, 0, 0};
	int	  i, j, k = 0;

	for(i = 0; i < 3; i++) {
		for(j = 0; j < 3; j++) v[i][j] *= scale;
	}

	GSMathNormal3x3(n, v[0], v[1], v[2]);

	GSGLPushMatrix(gl);
	for(i = 0; i < 6; i++) {
		for(j = 0; j < 4; j++) {
			GSGLSetColor(gl, ((k + j) % 2) ? col2 : col1);
			GSGLPolygon(gl, 3, v, NULL, n);

			GSGLSetRotation(gl, rot2);
		}

		if(i == 3) {
			GSGLSetRotation(gl, rot4);
			k++;
		} else if(i == 4) {
			GSGLSetRotation(gl, rot4);
			GSGLSetRotation(gl, rot4);
		} else {
			GSGLSetRotation(gl, rot3);
			k++;
		}
	}
	GSGLPopMatrix(gl);
}
