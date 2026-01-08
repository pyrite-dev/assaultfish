#include <GearBox/Core.h>

#include <GearBox/Version.h>
#include <GearBox/Log.h>
#include <GearBox/Client.h>
#include <GearBox/Server.h>

void GBInit(void) {
	GBVersion version;

	GBVersionGet(&version);

	GBLog(GBLogInfo, "GearBox Engine %s", version.string);
}

GBEngine GBEngineCreate(GBEngineParam* param) {
	GBEngine engine = malloc(sizeof(*engine));

	memset(engine, 0, sizeof(*engine));

	if(engine != NULL && param->client && (engine->client = GBClientCreate(engine)) == NULL) {
		GBLog(GBLogError, "Failed to create client");

		GBEngineDestroy(engine);

		engine = NULL;
	}

	if(engine != NULL && param->server && (engine->server = GBServerCreate(engine)) == NULL) {
		GBLog(GBLogError, "Failed to create server");

		GBEngineDestroy(engine);

		engine = NULL;
	}

	if(engine == NULL) {
		GBLog(GBLogError, "Failed to create engine");
	}

	return engine;
}

void GBEngineDestroy(GBEngine engine) {
	if(engine->server != NULL) GBServerDestroy(engine->server);
	if(engine->client != NULL) GBClientDestroy(engine->client);

	free(engine);
}

void GBEngineStart(GBEngine engine) {
}

void GBEngineLoop(GBEngine engine) {
}
