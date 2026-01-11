#include <GearSrc/Client.h>

#include <GearSrc/Log.h>
#include <GearSrc/GL.h>
#include <GearSrc/SkyBox.h>
#include <GearSrc/Math.h>

GSClient GSClientCreate(GSEngine engine) {
	GSClient client = malloc(sizeof(*client));

	memset(client, 0, sizeof(*client));

	client->engine = engine;

	client->camera[0] = 5;
	client->camera[1] = 0;
	client->camera[2] = 5;

	client->look_at[0] = 0;
	client->look_at[1] = 1;
	client->look_at[2] = 0;

	client->light0[0] = 2.5;
	client->light0[1] = 5;
	client->light0[2] = 5;
	client->light0[3] = 1;

	client->gl = GSGLCreate(client);

	client->skybox = GSSkyBoxOpen(client, "base:/skybox");

	GSLog(GSLogInfo, "Created client");

	return client;
}

void GSClientDestroy(GSClient client) {
	GSGLDestroy(client->gl);

	free(client);

	GSLog(GSLogInfo, "Destroyed client");
}

static void scene(GSClient client) {
	int i;
	static double r = 0;

	GSGLTextureSet(client->gl, 2);

	glBegin(GL_QUADS);
		glNormal3f(0, 1, 0);
		glTexCoord2f(0, 0);
		glVertex3f(-2.5, -2, -2.5);
		glTexCoord2f(0, 2.5);
		glVertex3f(-2.5, -2, 2.5);
		glTexCoord2f(2.5, 2.5);
		glVertex3f(2.5, -2, 2.5);
		glTexCoord2f(2.5, 0);
		glVertex3f(2.5, -2, -2.5);
	glEnd();

	glPushMatrix();
	glRotatef(r++, 1, 1, 1);
	for(i = 0; i < 6; i++){
		glBegin(GL_QUADS);
		glNormal3f(0, 1, 0);
		glTexCoord2f(0, 0);
		glVertex3f(-1, 1, -1);
		glTexCoord2f(0, 1);
		glVertex3f(-1, 1, 1);
		glTexCoord2f(1, 1);
		glVertex3f(1, 1, 1);
		glTexCoord2f(1, 0);
		glVertex3f(1, 1, -1);
		glEnd();
		if(i < 3) glRotatef(90, 0, 0, 1);
		if(i == 3) glRotatef(90, 1, 0, 0);
		if(i == 4) glRotatef(180, 1, 0, 0);
	}
	glPopMatrix();

	GSGLTextureSet(client->gl, 0);
}

void GSClientStep(GSClient client) {
	GSGLClear(client->gl);
	GSGLCameraSetup(client->gl);
	GSGLSetLight(client->gl);

	if(GSGLShadowBeforeMapping(client->gl)) {
		scene(client);
		GSGLShadowAfterMapping(client->gl);
	}

	if(client->skybox != NULL) GSSkyBoxDraw(client->skybox);
	scene(client);
	GSGLShadowEnd(client->gl);

	client->engine->param->gl_swapbuffer();

	static double r = 0;

	client->look_at[0] = 5 + cos(r) * 5;
	client->look_at[2] = 5 + sin(r) * 5;

	client->look_at[0] = 0;
	client->look_at[2] = 0;

	r += 0.1;
}
