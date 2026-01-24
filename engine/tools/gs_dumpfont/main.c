#include <stb_image_write.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GearSrc/File.h>

#define CHECK(s) ((linelen > strlen(s)) && memcmp(line, s, strlen(s)) == 0)

int main() {
	char*	       line = NULL;
	int	       linesize;
	int	       linelen;
	int	       iw, ih;
	unsigned char* font = NULL;
	int	       c;
	int	       p = 0;
	int	       y = 0;
	int	       w, h;
	GSFile	       file = GSFileOpen(NULL, "/dev/stdin");

	while((linelen = GSFileGetLine(file, &line, &linesize)) != -1) {
		linelen--;

		line[linelen] = 0;

		if(CHECK("FONTBOUNDINGBOX ")) {
			sscanf(line, "FONTBOUNDINGBOX %d %d", &w, &h);
			printf("%dx%d font\n", w, h);

			if(font != NULL) free(font);

			iw = w * 16;
			ih = h * 16;

			font = malloc(iw * ih * 4);
			memset(font, 0, iw * ih * 4);
		} else if(CHECK("ENCODING ")) {
			sscanf(line, "ENCODING %d", &c);
		} else if(strcmp(line, "BITMAP") == 0) {
			p = 1;
			y = 0;
		} else if(strcmp(line, "ENDCHAR") == 0) {
			p = 0;
		} else if(p && y < h) {
			int bx = c % 16;
			int by = c / 16;
			int i;
			int n = 0;

			for(i = 0; i < w; i++) {
				if((i % 4) == 0) {
					char buf[2];

					buf[0] = line[i / 4];
					buf[1] = 0;

					n = strtol(buf, NULL, 16);
				}

				if(n & (1 << 3)) {
					unsigned char* px = &font[((by * h + y) * iw + bx * w + i) * 4];
					memset(px, 255, 4);
				}

				n = n << 1;
			}

			y++;
		}
	}

	if(font != NULL) stbi_write_png("font.png", iw, ih, 4, font, 0);
	return 0;
}
