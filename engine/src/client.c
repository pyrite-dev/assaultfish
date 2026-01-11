#include <GearBox/Client.h>

#include <GearBox/Log.h>
#include <GearBox/GL.h>
#include <GearBox/Math.h>

GBClient GBClientCreate(GBEngine engine) {
	GBClient client = malloc(sizeof(*client));

	memset(client, 0, sizeof(*client));

	client->engine = engine;

	client->camera[0] = 5;
	client->camera[1] = 5;
	client->camera[2] = 5;

	client->look_at[0] = 0;
	client->look_at[1] = 0;
	client->look_at[2] = 0;

	client->light0[0] = 5;
	client->light0[1] = 5;
	client->light0[2] = 5;
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

static double r = 0;

static void scene(void) {
	glColor3f(1, 1, 1);

	int    i;
	double u = 1;

	glPushMatrix();
	glTranslatef(0, fabs(sin(r)), 1);
	glBegin(GL_QUADS);
	glNormal3f(0, -1, 0);
	for(i = 0; i < 4; i++) {
		glVertex3f(cos(90 * i / 180.0 * GBMathPi), u, sin(90 * i / 180.0 * GBMathPi));
	}
	glEnd();
	for(i = 0; i < 4; i++) {
		double n[3];
		double v0[3];
		double v1[3];
		double v2[3];

		v0[0] = 0;
		v0[1] = 1 + u;
		v0[2] = 0;

		v1[0] = cos(90 * (i + 1) / 180.0 * GBMathPi);
		v1[1] = u;
		v1[2] = sin(90 * (i + 1) / 180.0 * GBMathPi);

		v2[0] = cos(90 * i / 180.0 * GBMathPi);
		v2[1] = u;
		v2[2] = sin(90 * i / 180.0 * GBMathPi);

		GBMathNormal3x3(n, v0, v1, v2);

		glBegin(GL_TRIANGLES);
		glNormal3f(n[0], n[1], n[2]);
		glVertex3f(v0[0], v0[1], v0[2]);
		glVertex3f(v1[0], v1[1], v1[2]);
		glVertex3f(v2[0], v2[1], v2[2]);
		glEnd();
	}
	glPopMatrix();
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

	if(GBGLShadowBeforeMapping(client->gl)) {
		scene();
		GBGLShadowAfterMapping(client->gl);
	}

	glDisable(GL_DEPTH_TEST);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glVertex3f(-10, -0, -10);
	glVertex3f(-10, -0, 10);
	glVertex3f(10, -0, 10);
	glVertex3f(10, -0, -10);
	glEnd();
	glEnable(GL_DEPTH_TEST);

	scene();
	GBGLShadowEnd(client->gl);

	client->engine->param->gl_swapbuffer();

	client->light0[0] = cos(r) * 5;
	client->light0[2] = sin(r) * 5;

	r += 0.1;
}
