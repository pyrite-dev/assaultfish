#include <GearSrc/Model.h>

#include <GearSrc/GL.h>
#include <GearSrc/Core.h>
#include <GearSrc/File.h>
#include <GearSrc/Log.h>
#include <GearSrc/Math.h>

#include <stb_ds.h>

static void parse_obj(GSModel model, char* txt) {
	char* f	 = txt;
	float lm = 0; /* leftmost */
	float rm = 0; /* rightmost */
	float um = 0; /* upmost */
	float dm = 0; /* downmost */
	float fm = 0; /* frontmost */
	float bm = 0; /* backmost */

	while(1) {
		char* s = strchr(f, '\n');

		if(s != NULL) s[0] = 0;

		if(strlen(f) > 0 && f[0] != '#') {
			char* arg;

			if(f[strlen(f) - 1] == '\n') f[strlen(f) - 1] = 0;

			if((arg = strchr(f, ' ')) != NULL) {
				arg[0] = 0;
				arg++;

				if(strcmp(f, "v") == 0) {
					float v0, v1, v2;

					sscanf(arg, "%f %f %f", &v0, &v1, &v2);

					if(lm > v0) lm = v0;
					if(rm < v0) rm = v0;

					if(dm > v1) dm = v1;
					if(um < v1) um = v1;

					if(bm > v2) bm = v2;
					if(fm < v2) fm = v2;

					arrput(model->vertex, v0);
					arrput(model->vertex, v1);
					arrput(model->vertex, v2);
				} else if(strcmp(f, "vt") == 0) {
					float t0, t1;

					sscanf(arg, "%f %f", &t0, &t1);

					arrput(model->texcoord, t0);
					arrput(model->texcoord, t1);
				} else if(strcmp(f, "f") == 0) {
					int	    i;
					int	    c  = 1;
					int	    sl = 0;
					GSModelFace face;
					int	    varr[4];
					int	    tarr[4];
					float	    sc = 0;
					float	    lr = fabs(lm - rm);
					float	    ud = fabs(um - dm);
					float	    bf = fabs(bm - fm);

					if(sc < lr) sc = lr;
					if(sc < ud) sc = ud;
					if(sc < bf) sc = bf;

					sc /= 2;

					memset(&face, 0, sizeof(face));

					for(i = 0; arg[i] != 0; i++) {
						if(c == 1 && arg[i] == '/') {
							sl++;
						}
						if(arg[i] == ' ') c++;
					}

					face.count    = c;
					face.vertex   = malloc(sizeof(*face.vertex) * face.count);
					face.texcoord = malloc(sizeof(*face.texcoord) * face.count);

					if(c == 3) {
						int v0, v1, v2;
						int t0, t1, t2;
						int n0, n1, n2;

						if(sl == 2) {
							sscanf(arg, "%d/%d/%d %d/%d/%d %d/%d/%d", &v0, &t0, &n0, &v1, &t1, &n1, &v2, &t2, &n2);
						} else {
							sscanf(arg, "%d/%d %d/%d %d/%d", &v0, &t0, &v1, &t1, &v2, &t2);
						}

						varr[0] = v0 - 1;
						varr[1] = v1 - 1;
						varr[2] = v2 - 1;

						tarr[0] = t0 - 1;
						tarr[1] = t1 - 1;
						tarr[2] = t2 - 1;
					} else if(c == 4) {
						int v0, v1, v2, v3;
						int t0, t1, t2, t3;
						int n0, n1, n2, n3;

						if(sl == 2) {
							sscanf(arg, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &v0, &t0, &n0, &v1, &t1, &n1, &v2, &t2, &n2, &v3, &t3, &n3);
						} else {
							sscanf(arg, "%d/%d %d/%d %d/%d %d/%d", &v0, &t0, &v1, &t1, &v2, &t2, &v3, &t3);
						}

						varr[0] = v0 - 1;
						varr[1] = v1 - 1;
						varr[2] = v2 - 1;
						varr[3] = v3 - 1;

						tarr[0] = t0 - 1;
						tarr[1] = t1 - 1;
						tarr[2] = t2 - 1;
						tarr[3] = t3 - 1;
					}

					if(c == 3 || c == 4) {
						for(i = 0; i < c; i++) {
							if(arrlen(model->vertex) > varr[i] * 3) {
								face.vertex[i][0] = model->vertex[varr[i] * 3 + 0] / sc;
								face.vertex[i][1] = (model->vertex[varr[i] * 3 + 1] + dm) / sc;
								face.vertex[i][2] = model->vertex[varr[i] * 3 + 2] / sc;
							}

							if(arrlen(model->texcoord) > tarr[i] * 2) {
								face.texcoord[i][0] = model->texcoord[tarr[i] * 2 + 0];
								face.texcoord[i][1] = 1 - model->texcoord[tarr[i] * 2 + 1];
							}
						}
					}

					if(c == 3) GSMathNormal3x3(face.normal, face.vertex[0], face.vertex[1], face.vertex[2]);
					if(c == 4) GSMathNormal3x4(face.normal, face.vertex[0], face.vertex[1], face.vertex[2], face.vertex[3]);

					arrput(model->face, face);
				}
			}
		}

		if(s == NULL) break;
		f = s + 1;
	}
}

GSModel GSModelOpen(GSEngine engine, const char* path) {
	GSModel	     model = malloc(sizeof(*model));
	GSFile	     f;
	char*	     buffer;
	unsigned int size;
	int	     w, h;
	GSGL	     gl = NULL;

	if(engine->client != NULL) gl = engine->client->gl;

	memset(model, 0, sizeof(*model));

	model->engine = engine;

	if(!GSEngineRegisterResource(engine, "model-ws", path)) {
		GSModelClose(model);
		return NULL;
	}

	if((f = GSFileOpen(engine, "model-ws:/model.obj")) == NULL) {
		GSEngineUnregisterResource(engine, "model-ws");

		GSModelClose(model);
		return NULL;
	}

	if(gl != NULL) {
		if(GSGLTextureLoadFile(gl, &model->texture, &w, &h, "model-ws:/model.png")) {
		} else if(GSGLTextureLoadFile(gl, &model->texture, &w, &h, "model-ws:/model.bmp")) {
		} else if(GSGLTextureLoadFile(gl, &model->texture, &w, &h, "model-ws:/model.jpg")) {
		} else if(GSGLTextureLoadFile(gl, &model->texture, &w, &h, "model-ws:/model.jpeg")) {
		}
	}

	size	     = GSFileSize(f);
	buffer	     = malloc(size + 1);
	buffer[size] = 0;

	GSFileRead(f, buffer, size);
	GSFileClose(f);

	parse_obj(model, buffer);

	free(buffer);

	GSEngineUnregisterResource(engine, "model-ws");

	GSLog(GSLogInfo, "Opened model %s, %d vertexes, %d faces", path, arrlen(model->vertex), arrlen(model->face));

	/* TODO: check for animation */
	if(gl != NULL) {
		GSVector3 pos  = {0, 0, 0};
		GSVector3 rot  = {0, 0, 0};
		GLuint	  list = GSGLBeginList(gl);
		GSModelDraw(model, pos, rot);
		GSGLEndList(gl);

		model->call_list = list;
	}

	return model;
}

void GSModelDraw(GSModel model, GSVector3 pos, GSVector3 rot) {
	GSGL gl = model->engine->client->gl;
	int  i;

	GSGLPushMatrix(gl);
	GSGLSetPosition(gl, pos, rot);

	if(model->call_list == 0) {
		GSGLTextureSet(gl, model->texture);

		for(i = 0; i < arrlen(model->face); i++) {
			GSGLPolygon(gl, model->face[i].count, model->face[i].vertex, model->face[i].texcoord, model->face[i].normal);
		}

		GSGLTextureSet(gl, 0);
	} else {
		GSGLCallList(gl, model->call_list);
	}

	GSGLPopMatrix(gl);
}

void GSModelClose(GSModel model) {
	int i;
	for(i = 0; i < arrlen(model->face); i++) {
		if(model->face[i].vertex != NULL) free(model->face[i].vertex);
		if(model->face[i].texcoord != NULL) free(model->face[i].texcoord);
	}
	arrfree(model->face);

	arrfree(model->vertex);
	arrfree(model->texcoord);

	if(model->texture != 0) GSGLTextureDelete(model->engine->client->gl, model->texture);

	free(model);
}
