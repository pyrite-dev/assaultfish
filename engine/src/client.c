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

	client->skybox = GSSkyBoxTry(client, "default");

	GSLog(GSLogInfo, "Created client");

	return client;
}

void GSClientDestroy(GSClient client) {
	GSGLDestroy(client->gl);

	free(client);

	GSLog(GSLogInfo, "Destroyed client");
}

static void scene(GSClient client) {
}

void GSClientStep(GSClient client) {
	static double r = 0;

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

	client->look_at[0] = 5 + cos(r) * 5;
	client->look_at[2] = 5 + sin(r) * 5;

	client->look_at[0] = 0;
	client->look_at[2] = 0;

	r += 0.1;
}
