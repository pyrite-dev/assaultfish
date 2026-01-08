#include <GearBox/Client.h>

#include <GearBox/Log.h>

GBClient GBClientCreate(GBEngine engine) {
	GBLog(GBLogInfo, "Creating client");

	return NULL;
}

void GBClientDestroy(GBClient client) {
	free(client);
}
