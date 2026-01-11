#include <GearBox/SkyBox.h>

#include <GearBox/GL.h>
#include <GearBox/String.h>

GBSkyBox GBSkyBoxOpen(GBClient client, const char* base) {
	GBSkyBox skybox = malloc(sizeof(*skybox));
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
		p = GBStringConcat(base, ptr[i + 1]);
		if(!GBGLTextureLoadFile(client->gl, ptr[i], NULL, NULL, p)) {
			free(p);
			GBSkyBoxClose(skybox);
			return NULL;
		}
		free(p);
	}

	return skybox;
}

void GBSkyBoxDraw(GBSkyBox skybox) {
	GBGL	  gl = skybox->engine->client->gl;
	int	  i;
	GBVector3 v[4];
	GBVector2 t[4];
	GBNumber  s = 0.0001;

	t[0][0] = 0, t[0][1] = 0;

	t[1][0] = 0, t[1][1] = 1;

	t[2][0] = 1, t[2][1] = 1;

	t[3][0] = 1, t[3][1] = 0;

	GBGLIgnoreDepth(gl);
	GBGLShadowDisable(gl);

	for(i = 0; i < 6; i++) {
		GLuint tex;
		int    j, k;

		if(i == 0) {
			tex = skybox->front;

			v[0][0] = -1 - s, v[0][1] = 1 + s, v[0][2] = -1;
			v[1][0] = -1 - s, v[1][1] = -1 - s, v[1][2] = -1;
			v[2][0] = 1 + s, v[2][1] = -1 - s, v[2][2] = -1;
			v[3][0] = 1 + s, v[3][1] = 1 + s, v[3][2] = -1;
		} else if(i == 1) {
			tex = skybox->back;

			v[0][0] = 1 + s, v[0][1] = 1 + s, v[0][2] = 1;
			v[1][0] = 1 + s, v[1][1] = -1 - s, v[1][2] = 1;
			v[2][0] = -1 - s, v[2][1] = -1 - s, v[2][2] = 1;
			v[3][0] = -1 - s, v[3][1] = 1 + s, v[3][2] = 1;
		} else if(i == 2) {
			tex = skybox->left;

			v[0][0] = -1, v[0][1] = 1 + s, v[0][2] = 1 + s;
			v[1][0] = -1, v[1][1] = -1 - s, v[1][2] = 1 + s;
			v[2][0] = -1, v[2][1] = -1 - s, v[2][2] = -1 - s;
			v[3][0] = -1, v[3][1] = 1 + s, v[3][2] = -1 - s;
		} else if(i == 3) {
			tex = skybox->right;

			v[0][0] = 1, v[0][1] = 1 + s, v[0][2] = -1 - s;
			v[1][0] = 1, v[1][1] = -1 - s, v[1][2] = -1 - s;
			v[2][0] = 1, v[2][1] = -1 - s, v[2][2] = 1 + s;
			v[3][0] = 1, v[3][1] = 1 + s, v[3][2] = 1 + s;
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
				v[j][k] *= GBGLMaxDistance / 2;
				v[j][k] += gl->engine->client->camera[k];
			}
		}

		GBGLTextureSet(gl, tex);

		GBGLPolygon(gl, 4, v, t);
	}

	GBGLTextureSet(gl, 0);

	GBGLShadowEnable(gl);
	GBGLCareDepth(gl);
}

void GBSkyBoxClose(GBSkyBox skybox) {
	if(skybox->left > 0) GBGLTextureDelete(skybox->engine->client->gl, skybox->left);
	free(skybox);
}
