#include <GearBox/Server.h>

#include <GearBox/Log.h>

GBServer GBServerCreate(GBEngine engine) {
	GBServer server = malloc(sizeof(*server));

	memset(server, 0, sizeof(*server));

	server->engine = engine;

	GBLog(GBLogInfo, "Created server");

	return server;
}

void GBServerDestroy(GBServer server) {
	free(server);

	GBLog(GBLogInfo, "Destroyed server");
}

void GBServerStep(GBServer server) {
}
