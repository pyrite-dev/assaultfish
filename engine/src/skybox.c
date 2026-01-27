#include <GearSrc/SkyBox.h>

#include <GearSrc/GL.h>
#include <GearSrc/String.h>

GSSkyBox GSSkyBoxOpen(GSClient client, const char* base) {
	GSSkyBox skybox = malloc(sizeof(*skybox));
	char*	 p;
	void*	 ptr[12];
	int	 i;

	memset(skybox, 0, sizeof(*skybox));

	skybox->engine = client->engine;

	ptr[0]	= &skybox->left;
	ptr[1]	= "/left";
	ptr[2]	= &skybox->right;
	ptr[3]	= "/right";
	ptr[4]	= &skybox->front;
	ptr[5]	= "/front";
	ptr[6]	= &skybox->back;
	ptr[7]	= "/back";
	ptr[8]	= &skybox->up;
	ptr[9]	= "/up";
	ptr[10] = &skybox->down;
	ptr[11] = "/down";

	for(i = 0; i < 12; i += 2) {
		char* b = GSStringConcat(base, ptr[i + 1]);

		if(!GSGLTextureTry(client->gl, ptr[i], NULL, NULL, b)) {
			free(b);
			GSSkyBoxClose(skybox);
			return NULL;
		}

		free(b);
	}

	p = GSStringConcat(base, "/cloud0");
	GSGLTextureTry(client->gl, &skybox->cloud0, NULL, NULL, p);
	free(p);

	p = GSStringConcat(base, "/cloud1");
	GSGLTextureTry(client->gl, &skybox->cloud1, NULL, NULL, p);
	free(p);

	skybox->tick = 0;

	return skybox;
}

GSSkyBox GSSkyBoxTry(GSClient client, const char* name) {
	const char* ns[] = {"game", "base"};
	int	    i;
	GSSkyBox    sb = NULL;

	for(i = 0; i < 2; i++) {
		char* b = GSStringConcat(ns[i], ":/skybox/");
		char* s = GSStringConcat(b, name);

		sb = GSSkyBoxOpen(client, s);

		free(s);
		free(b);

		if(sb != NULL) break;
	}

	return sb;
}

void GSSkyBoxDraw(GSSkyBox skybox) {
	GSGL	  gl = skybox->engine->client->gl;
	int	  i;
	GSVector3 v[4];
	GSVector2 t[4];
	GSVector3 n	    = {0, 0, 0};
	GSNumber  s	    = GSGLMaxDistance / 2 / 10000;
	GSVector2 scroll[2] = {
	    {-0.002, 0.0005}, /**/
	    {0.006, 0.005}    /**/
	};

	t[0][0] = 0, t[0][1] = 0;
	t[1][0] = 0, t[1][1] = 1;
	t[2][0] = 1, t[2][1] = 1;
	t[3][0] = 1, t[3][1] = 0;

	GSGLPushMatrix(gl);
	GSGLShadowDisable(gl);

	GSGLInitialTranslation(gl);

	for(i = 0; i < 6; i++) {
		GLuint tex;
		int    j, k;

		if(i == 0) {
			tex = skybox->left;

			v[0][0] = -1, v[0][1] = 1 + s, v[0][2] = -1 - s;
			v[1][0] = -1, v[1][1] = -1 - s, v[1][2] = -1 - s;
			v[2][0] = -1, v[2][1] = -1 - s, v[2][2] = 1 + s;
			v[3][0] = -1, v[3][1] = 1 + s, v[3][2] = 1 + s;
		} else if(i == 1) {
			tex = skybox->right;

			v[0][0] = 1, v[0][1] = 1 + s, v[0][2] = 1 + s;
			v[1][0] = 1, v[1][1] = -1 - s, v[1][2] = 1 + s;
			v[2][0] = 1, v[2][1] = -1 - s, v[2][2] = -1 - s;
			v[3][0] = 1, v[3][1] = 1 + s, v[3][2] = -1 - s;
		} else if(i == 2) {
			tex = skybox->front;

			v[0][0] = -1 - s, v[0][1] = 1 + s, v[0][2] = 1;
			v[1][0] = -1 - s, v[1][1] = -1 - s, v[1][2] = 1;
			v[2][0] = 1 + s, v[2][1] = -1 - s, v[2][2] = 1;
			v[3][0] = 1 + s, v[3][1] = 1 + s, v[3][2] = 1;
		} else if(i == 3) {
			tex = skybox->back;

			v[0][0] = 1 + s, v[0][1] = 1 + s, v[0][2] = -1;
			v[1][0] = 1 + s, v[1][1] = -1 - s, v[1][2] = -1;
			v[2][0] = -1 - s, v[2][1] = -1 - s, v[2][2] = -1;
			v[3][0] = -1 - s, v[3][1] = 1 + s, v[3][2] = -1;
		} else if(i == 4) {
			tex = skybox->up;

			v[0][0] = -1 - s, v[0][1] = 1, v[0][2] = -1 - s;
			v[1][0] = -1 - s, v[1][1] = 1, v[1][2] = 1 + s;
			v[2][0] = 1 + s, v[2][1] = 1, v[2][2] = 1 + s;
			v[3][0] = 1 + s, v[3][1] = 1, v[3][2] = -1 - s;
		} else if(i == 5) {
			tex = skybox->down;

			v[0][0] = -1 - s, v[0][1] = -1, v[0][2] = 1 + s;
			v[1][0] = -1 - s, v[1][1] = -1, v[1][2] = -1 - s;
			v[2][0] = 1 + s, v[2][1] = -1, v[2][2] = -1 - s;
			v[3][0] = 1 + s, v[3][1] = -1, v[3][2] = 1 + s;
		}

		GSGLTextureSet(gl, tex);

		GSGLPolygon(gl, 4, v, t, n);
	}

	GSGLIgnoreDepth(gl);
	for(i = 0; i < 2; i++) {
		GLuint t = (i == 0) ? skybox->cloud0 : skybox->cloud1;
		if(t != 0) {
			GSVector3 vc[4];
			GSVector2 tc[4];
			GSVector3 n;
			int	  j, k;

			n[0] = 0;
			n[1] = -1;
			n[2] = 0;

			vc[0][0] = -1 - s, vc[0][1] = 1, vc[0][2] = -1 - s;
			vc[1][0] = -1 - s, vc[1][1] = 1, vc[1][2] = 1 + s;
			vc[2][0] = 1 - s, vc[2][1] = 1, vc[2][2] = 1 + s;
			vc[3][0] = 1 - s, vc[3][1] = 1, vc[3][2] = -1 - s;

			tc[0][0] = -1, tc[0][1] = -1;
			tc[1][0] = -1, tc[1][1] = 1;
			tc[2][0] = 1, tc[2][1] = 1;
			tc[3][0] = 1, tc[3][1] = -1;

			for(j = 0; j < 4; j++) {
				for(k = 0; k < 2; k++) {
					tc[j][k] *= GSGLMaxDistance;
				}
				for(k = 0; k < 2; k++) {
					tc[j][k] += scroll[i][k] * skybox->tick;
				}
			}

			for(j = 0; j < 4; j++) {
				for(k = 0; k < 3; k++) {
					vc[j][k] *= GSGLMaxDistance / 2;
				}

				vc[j][1] = 0.05;
			}

			GSGLTextureSet(gl, t);
			GSGLPolygon(gl, 4, vc, tc, n);
		}
	}
	GSGLCareDepth(gl);

	GSGLTextureSet(gl, 0);

	GSGLClearDepth(gl);
	GSGLShadowEnable(gl);
	GSGLPopMatrix(gl);

	/* this overflows in:
	 * 3.402823466e+38/60/60/24/365 = 1.0790282426433283e+31
	 * years. which would NEVER happen.
	 */
	skybox->tick += 1 / skybox->engine->tps;
}

void GSSkyBoxClose(GSSkyBox skybox) {
	GLuint* ptr[6];
	int	i;

	ptr[0] = &skybox->left;
	ptr[1] = &skybox->right;
	ptr[2] = &skybox->front;
	ptr[3] = &skybox->back;
	ptr[4] = &skybox->up;
	ptr[5] = &skybox->down;

	for(i = 0; i < 6; i++) {
		if(*ptr[i] > 0) GSGLTextureDelete(skybox->engine->client->gl, *ptr[i]);
	}
	free(skybox);
}
