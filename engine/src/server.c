#include <GearBox/Server.h>

#include <GearBox/Log.h>

GBServer GBServerCreate(GBEngine engine) {
	GBLog(GBLogInfo, "Creating server");

	return NULL;
}

void GBServerDestroy(GBServer server) {

	free(server);
}
