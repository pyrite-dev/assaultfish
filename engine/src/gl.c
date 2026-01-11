#include <GearBox/GL.h>

GBGL GBGLCreate(GBClient client) {
	GLfloat lightamb[] = {0.5, 0.5, 0.5, 1.0};
	GLfloat lightcol[] = {1.0, 1.0, 1.0, 1.0};
	GBGL	gl;

	gl = malloc(sizeof(*client->gl));

	gl->engine = client->engine;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_BLEND);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);

	glLightfv(GL_LIGHT0, GL_AMBIENT, lightamb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightcol);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightcol);

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GBGLShadowInit(gl);

	return gl;
}

void GBGLDestroy(GBGL gl) {
	GBGLShadowDeinit(gl);

	free(gl);
}

void GBGLClear(GBGL gl) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GBGLIgnoreDepth(GBGL gl) {
	glDisable(GL_DEPTH_TEST);
}

void GBGLCareDepth(GBGL gl) {
	glEnable(GL_DEPTH_TEST);
}

void GBGLSetLight(GBGL gl) {
	GLfloat f[4];
	int	i;

	for(i = 0; i < 4; i++) f[i] = gl->engine->client->light0[i];

	glLightfv(GL_LIGHT0, GL_POSITION, f);
}
