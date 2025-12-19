#include <af_common.h>

#include <stb_image.h>

static GLuint font_atlas;
static int    font_width = 0, font_height = 0;

void gl_font_init(void) {
	unsigned int   ch;
	unsigned char* data = stbi_load(DATAROOTDIR "/assaultfish/font.png", &font_width, &font_height, &ch, 4);

	glGenTextures(1, &font_atlas);
	glBindTexture(GL_TEXTURE_2D, font_atlas);
	if(data != NULL) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, font_width, font_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		free(data);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void gl_font_text(const char* text, int x, int y, double scale) {
	int i;
	int gw = font_width / 16 * scale, gh = font_height / 16 * scale;
	int is = glIsEnabled(GL_TEXTURE_2D) ? 1 : 0;

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);
	if(!is) glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, font_atlas);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, MwGetInteger(opengl, MwNwidth), MwGetInteger(opengl, MwNheight), 0, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glBegin(GL_QUADS);
	for(i = 0; text[i] != 0; i++) {
		double bx = 1.0 / 16 * (text[i] % 16), by = 1.0 / 16 * (text[i] / 16);

		glTexCoord2f(bx, by);
		glVertex2f(i * gw + x, y);
		glTexCoord2f(bx + 1.0 / 16, by);
		glVertex2f(i * gw + x + gw, y);
		glTexCoord2f(bx + 1.0 / 16, by + 1.0 / 16);
		glVertex2f(i * gw + x + gw, y + gh);
		glTexCoord2f(bx, by + 1.0 / 16);
		glVertex2f(i * gw + x, y + gh);
	}
	glEnd();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);

	glBindTexture(GL_TEXTURE_2D, 0);
	if(!is) glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
}

int gl_font_width(const char* text) {
	return font_width / 16 * strlen(text);
}

int gl_font_height(const char* text) {
	return font_height / 16;
}
