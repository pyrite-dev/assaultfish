#include <GearBox/Client.h>

#include <GearBox/Log.h>
#include <GearBox/GL.h>

GBClient GBClientCreate(GBEngine engine) {
	GBClient client = malloc(sizeof(*client));

	memset(client, 0, sizeof(*client));

	client->engine = engine;

	client->gl = GBGLCreate(client);

	GBLog(GBLogInfo, "Created client");

	return client;
}

void GBClientDestroy(GBClient client) {
	GBGLDestroy(client->gl);

	free(client);

	GBLog(GBLogInfo, "Destroyed client");
}

void GBClientStep(GBClient client) {
	client->engine->param->gl_swapbuffer();
}
