#include <GearBox/Client.h>

#include <GearBox/Log.h>
#include <GearBox/GL.h>

GBClient GBClientCreate(GBEngine engine) {
	GBClient client = malloc(sizeof(*client));

	memset(client, 0, sizeof(*client));

	client->engine = engine;

	client->camera[0] = 0;
	client->camera[1] = 1.5;
	client->camera[2] = 0;

	client->look_at[0] = 0;
	client->look_at[1] = 0;
	client->look_at[2] = 0;

	client->light0[0] = 0;
	client->light0[1] = 1;
	client->light0[2] = -5;
	client->light0[3] = 1;

	client->gl = GBGLCreate(client);

	GBLog(GBLogInfo, "Created client");

	return client;
}

void GBClientDestroy(GBClient client) {
	GBGLDestroy(client->gl);

	free(client);

	GBLog(GBLogInfo, "Destroyed client");
}

static void scene(void) {
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glVertex3f(-1, 0, -1);
	glVertex3f(-1, 0, 1);
	glVertex3f(1, 0, 1);
	glVertex3f(1, 0, -1);
	glEnd();

	int i;

	glColor3f(1, 0, 0);
	for(i = 0; i < 2; i++) {
		glPushMatrix();
		glRotatef(180 * i, 0, 1, 0);
		glBegin(GL_QUADS);
		glNormal3f(-1, 0, 0);
		glVertex3f(-1, 0.5, 0);
		glVertex3f(-1, 0, 0);
		glVertex3f(1, 0, 0);
		glVertex3f(1, 0.5, 0);
		glEnd();
		glPopMatrix();
	}
}

void GBClientStep(GBClient client) {
	GLfloat f[4];
	int	i;

	for(i = 0; i < 4; i++) f[i] = client->light0[i];

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GBGLPerspective(client->gl, client->engine->width, client->engine->height);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	GBGLSetCamera(client->gl);

	glLightfv(GL_LIGHT0, GL_POSITION, f);

	GBGLShadowBeforeMapping(client->gl);
	scene();
	GBGLShadowAfterMapping(client->gl);
	scene();
	GBGLShadowEnd(client->gl);

	client->engine->param->gl_swapbuffer();

	static double r = 0;

	client->camera[0] = cos(r) * 2;
	client->camera[2] = sin(r) * 2;

	// client->light0[0] = -cos(r) * 2;
	// client->light0[2] = -sin(r) * 2;

	r += 0.1;
}
