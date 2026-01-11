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

static void scene(GBClient client) {
}

void GBClientStep(GBClient client) {
	GBGLClear(client->gl);
	GBGLCameraSetup(client->gl);
	GBGLSetLight(client->gl);

	if(GBGLShadowBeforeMapping(client->gl)) {
		scene(client);
		GBGLShadowAfterMapping(client->gl);
	}

	scene(client);
	GBGLShadowEnd(client->gl);

	client->engine->param->gl_swapbuffer();

	client->camera[0] = cos(r) * 5;
	client->camera[2] = sin(r) * 5;

	r += 0.1;
}
