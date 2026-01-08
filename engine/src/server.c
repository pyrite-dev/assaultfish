#include <GearBox/Server.h>

#include <GearBox/Log.h>

GBServer GBServerCreate(GBEngine engine) {
	GBServer server = malloc(sizeof(*server));

	memset(server, 0, sizeof(*server));

	GBLog(GBLogInfo, "Creating server");

	return server;
}

void GBServerDestroy(GBServer server) {
	free(server);
}

void GBServerStep(GBServer server) {
}
