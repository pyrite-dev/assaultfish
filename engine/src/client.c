#include <GearBox/Client.h>

#include <GearBox/Log.h>

GBClient GBClientCreate(GBEngine engine) {
	GBClient client = malloc(sizeof(*client));

	memset(client, 0, sizeof(*client));

	client->engine = engine;

	GBLog(GBLogInfo, "Creating client");

	return client;
}

void GBClientDestroy(GBClient client) {
	free(client);
}

void GBClientStep(GBClient client) {
}
