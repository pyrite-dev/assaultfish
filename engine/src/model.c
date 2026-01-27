#include <GearSrc/Model.h>

#include <GearSrc/GL.h>
#include <GearSrc/Core.h>
#include <GearSrc/File.h>
#include <GearSrc/Log.h>
#include <GearSrc/Math.h>
#include <GearSrc/String.h>

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

			if(f[strlen(f) - 1] == '\r') f[strlen(f) - 1] = 0;

			if((arg = strchr(f, ' ')) != NULL) {
				arg[0] = 0;
				arg++;

				if(strcmp(f, "usemtl") == 0) {
					int   k = arrlen(model->face);
					char* v = GSStringDuplicate(arg);

					hmput(model->usemtl, k, v);
				} else if(strcmp(f, "v") == 0) {
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
								face.vertex[i][0] = (model->vertex[varr[i] * 3 + 0] - lm - lr / 2) / sc;
								face.vertex[i][1] = (model->vertex[varr[i] * 3 + 1] - dm - ud / 2) / sc;
								face.vertex[i][2] = -(model->vertex[varr[i] * 3 + 2] - bm - bf / 2) / sc;
							}

							if(arrlen(model->texcoord) > tarr[i] * 2) {
								face.texcoord[i][0] = model->texcoord[tarr[i] * 2 + 0];
								face.texcoord[i][1] = 1 - model->texcoord[tarr[i] * 2 + 1];
							}
						}
					}

					if(c == 3) GSMathNormal3x3(face.normal, face.vertex[2], face.vertex[1], face.vertex[0]);
					if(c == 4) GSMathNormal3x4(face.normal, face.vertex[3], face.vertex[2], face.vertex[1], face.vertex[0]);

					arrput(model->face, face);
				}
			}
		}

		if(s == NULL) break;
		f = s + 1;
	}
}

static void parse_mtl(GSModel model, char* txt) {
	char* f = txt;
	char* m = NULL;

	while(1) {
		char* s = strchr(f, '\n');

		if(s != NULL) s[0] = 0;

		if(strlen(f) > 0 && f[0] != '#') {
			char* arg;

			if(f[strlen(f) - 1] == '\r') f[strlen(f) - 1] = 0;

			if((arg = strchr(f, ' ')) != NULL) {
				arg[0] = 0;
				arg++;

				if(strcmp(f, "newmtl") == 0) {
					GLuint v = 0;

					shput(model->material, arg, v);

					if(m != NULL) free(m);
					m = GSStringDuplicate(arg);
				} else if(strcmp(f, "map_Kd") == 0 && m != NULL) {
					GLuint t = 0;
					char*  p = GSStringConcat("model-ws:/", arg);

					if(GSGLTextureTry(model->engine->client->gl, &t, NULL, NULL, p)) {
						shput(model->material, m, t);
					}
				}
			}
		}

		if(s == NULL) break;
		f = s + 1;
	}

	if(m != NULL) free(m);
}

GSModel GSModelOpen(GSEngine engine, const char* path) {
	GSModel	     model = malloc(sizeof(*model));
	GSFile	     f;
	char*	     buffer;
	unsigned int size;
	GSGL	     gl = NULL;

	if(engine->client != NULL) gl = engine->client->gl;

	memset(model, 0, sizeof(*model));

	model->engine = engine;

	if(!GSEngineRegisterResource(engine, "model-ws", path)) {
		GSModelClose(model);
		return NULL;
	}

	model->material = NULL;
	sh_new_strdup(model->material);

	model->usemtl = NULL;

	if(gl != NULL) {
		GLuint t = 0;

		GSGLTextureTry(gl, &t, NULL, NULL, "model-ws:/model");

		if(t != 0) {
			char* k = "default";
			shput(model->material, k, t);
		}
	}

	if((f = GSFileOpen(engine, "model-ws:/model.obj")) == NULL) {
		GSEngineUnregisterResource(engine, "model-ws");

		GSModelClose(model);
		return NULL;
	}

	size	     = GSFileSize(f);
	buffer	     = malloc(size + 1);
	buffer[size] = 0;

	GSFileRead(f, buffer, size);
	GSFileClose(f);

	parse_obj(model, buffer);

	free(buffer);

	if(gl != NULL && (f = GSFileOpen(engine, "model-ws:/model.mtl")) != NULL) {
		size	     = GSFileSize(f);
		buffer	     = malloc(size + 1);
		buffer[size] = 0;

		GSFileRead(f, buffer, size);
		GSFileClose(f);

		parse_mtl(model, buffer);

		free(buffer);
	}

	GSEngineUnregisterResource(engine, "model-ws");

	GSLog(engine, GSLogInfo, "%s: %d vertexes, %d faces", path, arrlen(model->vertex), arrlen(model->face));

	/* TODO: check for animation */
	if(gl != NULL) {
		GLuint list = GSGLBeginList(gl);
		GSModelDraw(model);
		GSGLEndList(gl);

		model->call_list = list;
	}

	return model;
}

void GSModelDraw(GSModel model) {
	GSGL gl = model->engine->client->gl;
	int  i;

	if(model->call_list == 0) {
		char* k = "default";

		GSGLTextureSet(gl, shget(model->material, k));

		for(i = 0; i < arrlen(model->face); i++) {
			int ind = hmgeti(model->usemtl, i);

			if(ind != -1) {
				GSGLTextureSet(gl, shget(model->material, model->usemtl[ind].value));
			}

			GSGLPolygon(gl, model->face[i].count, model->face[i].vertex, model->face[i].texcoord, model->face[i].normal);
		}

		GSGLTextureSet(gl, 0);
	} else {
		GSGLCallList(gl, model->call_list);
	}
}

void GSModelClose(GSModel model) {
	int i;
	for(i = 0; i < arrlen(model->face); i++) {
		if(model->face[i].vertex != NULL) free(model->face[i].vertex);
		if(model->face[i].texcoord != NULL) free(model->face[i].texcoord);
	}
	arrfree(model->face);

	for(i = 0; i < shlen(model->material); i++) {
		GSGLTextureDelete(model->engine->client->gl, model->material[i].value);
	}
	shfree(model->material);

	for(i = 0; i < hmlen(model->usemtl); i++) {
		free(model->usemtl[i].value);
	}
	hmfree(model->usemtl);

	arrfree(model->vertex);
	arrfree(model->texcoord);

	free(model);
}
