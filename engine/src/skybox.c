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
	ptr[1]	= "/left.png";
	ptr[2]	= &skybox->right;
	ptr[3]	= "/right.png";
	ptr[4]	= &skybox->front;
	ptr[5]	= "/front.png";
	ptr[6]	= &skybox->back;
	ptr[7]	= "/back.png";
	ptr[8]	= &skybox->up;
	ptr[9]	= "/up.png";
	ptr[10] = &skybox->down;
	ptr[11] = "/down.png";

	for(i = 0; i < 12; i += 2) {
		p = GSStringConcat(base, ptr[i + 1]);
		if(!GSGLTextureLoadFile(client->gl, ptr[i], NULL, NULL, p)) {
			free(p);
			GSSkyBoxClose(skybox);
			return NULL;
		}
		free(p);
	}

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
	GSVector3 n = {0, 0, 0};
	GSNumber  s = 0.0001;

	t[0][0] = 0, t[0][1] = 0;

	t[1][0] = 0, t[1][1] = 1;

	t[2][0] = 1, t[2][1] = 1;

	t[3][0] = 1, t[3][1] = 0;

	GSGLIgnoreDepth(gl);
	GSGLShadowDisable(gl);

	for(i = 0; i < 6; i++) {
		GLuint tex;
		int    j, k;

		if(i == 0) {
			tex = skybox->left;

			v[0][0] = -1, v[0][1] = 1 + s, v[0][2] = 1 + s;
			v[1][0] = -1, v[1][1] = -1 - s, v[1][2] = 1 + s;
			v[2][0] = -1, v[2][1] = -1 - s, v[2][2] = -1 - s;
			v[3][0] = -1, v[3][1] = 1 + s, v[3][2] = -1 - s;
		} else if(i == 1) {
			tex = skybox->right;

			v[0][0] = 1, v[0][1] = 1 + s, v[0][2] = -1 - s;
			v[1][0] = 1, v[1][1] = -1 - s, v[1][2] = -1 - s;
			v[2][0] = 1, v[2][1] = -1 - s, v[2][2] = 1 + s;
			v[3][0] = 1, v[3][1] = 1 + s, v[3][2] = 1 + s;
		} else if(i == 2) {
			tex = skybox->front;

			v[0][0] = -1 - s, v[0][1] = 1 + s, v[0][2] = -1;
			v[1][0] = -1 - s, v[1][1] = -1 - s, v[1][2] = -1;
			v[2][0] = 1 + s, v[2][1] = -1 - s, v[2][2] = -1;
			v[3][0] = 1 + s, v[3][1] = 1 + s, v[3][2] = -1;
		} else if(i == 3) {
			tex = skybox->back;

			v[0][0] = 1 + s, v[0][1] = 1 + s, v[0][2] = 1;
			v[1][0] = 1 + s, v[1][1] = -1 - s, v[1][2] = 1;
			v[2][0] = -1 - s, v[2][1] = -1 - s, v[2][2] = 1;
			v[3][0] = -1 - s, v[3][1] = 1 + s, v[3][2] = 1;
		} else if(i == 4) {
			tex = skybox->up;

			v[0][0] = -1 - s, v[0][1] = 1, v[0][2] = 1 + s;
			v[1][0] = -1 - s, v[1][1] = 1, v[1][2] = -1 - s;
			v[2][0] = 1 + s, v[2][1] = 1, v[2][2] = -1 - s;
			v[3][0] = 1 + s, v[3][1] = 1, v[3][2] = 1 + s;
		} else if(i == 5) {
			tex = skybox->down;

			v[0][0] = -1 - s, v[0][1] = -1, v[0][2] = -1 - s;
			v[1][0] = -1 - s, v[1][1] = -1, v[1][2] = 1 + s;
			v[2][0] = 1 + s, v[2][1] = -1, v[2][2] = 1 + s;
			v[3][0] = 1 + s, v[3][1] = -1, v[3][2] = -1 - s;
		}

		for(j = 0; j < 4; j++) {
			for(k = 0; k < 3; k++) {
				v[j][k] *= GSGLMaxDistance / 2;
				v[j][k] += gl->engine->client->camera[k];
			}
		}

		GSGLTextureSet(gl, tex);

		GSGLPolygon(gl, 4, v, t, n);
	}

	GSGLTextureSet(gl, 0);

	GSGLShadowEnable(gl);
	GSGLCareDepth(gl);
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
