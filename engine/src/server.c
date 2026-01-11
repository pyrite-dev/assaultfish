#include <GearSrc/Server.h>

#include <GearSrc/Log.h>

GSServer GSServerCreate(GSEngine engine) {
	GSServer server = malloc(sizeof(*server));

	memset(server, 0, sizeof(*server));

	server->engine = engine;

	GSLog(GSLogInfo, "Created server");

	return server;
}

void GSServerDestroy(GSServer server) {
	free(server);

	GSLog(GSLogInfo, "Destroyed server");
}

void GSServerStep(GSServer server) {
}
