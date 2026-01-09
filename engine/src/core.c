#include <GearBox/Core.h>

#include <GearBox/Version.h>
#include <GearBox/Log.h>
#include <GearBox/Client.h>
#include <GearBox/Server.h>
#include <GearBox/GL.h>
#include <GearBox/Resource.h>

#include <stb_ds.h>

void GBInit(void) {
	GBVersion version;

	GBVersionGet(&version);

	GBLog(GBLogInfo, "GearBox Engine %s", version.string);

	gload_init();
}

GBEngine GBEngineCreate(GBEngineParam* param) {
	GBEngine   engine = malloc(sizeof(*engine));
	GBResource resource;

	memset(engine, 0, sizeof(*engine));

	engine->param = malloc(sizeof(*engine->param));
	memcpy(engine->param, param, sizeof(*param));

	if(param->ready != NULL) {
		param->ready(1024, 768);
	}

	sh_new_strdup(engine->resource);

	resource = GBResourceOpen(engine, "base.pak");
	shput(engine->resource, "base", resource);

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
	shfree(engine->resource);

	if(engine->server != NULL) GBServerDestroy(engine->server);
	if(engine->client != NULL) GBClientDestroy(engine->client);
	if(engine->param != NULL) free(engine->param);

	free(engine);
}

void GBEngineParamInit(GBEngineParam* param) {
	memset(param, 0, sizeof(*param));

	param->server	     = 0;
	param->client	     = 1;
	param->gl_swapbuffer = NULL;
	param->ready	     = NULL;
	param->sleep	     = NULL;
	param->get_tick	     = NULL;
}

void GBEngineLoop(GBEngine engine) {
	long t		   = 0;
	int  wait_actually = 0;
	int  more	   = 0;
	long wait	   = 1000 / 50.0;

	if(engine->param->sleep == NULL || engine->param->get_tick == NULL) {
		GBLog(GBLogWarn, "sleep and/or get_tick parameter are missing!");
		GBLog(GBLogWarn, "This will result game to be crazy fast!!!");
	} else {
		t = engine->param->get_tick();

		wait_actually = 1;
	}

	while(1) {
		if(engine->client != NULL) GBClientStep(engine->client);
		if(engine->server != NULL) GBServerStep(engine->server);

		if(engine->param->tick != NULL) engine->param->tick();

		if(wait_actually) {
			int diff = (wait - more) - (engine->param->get_tick() - t);

			if(diff > 0) {
				engine->param->sleep(diff);
			}

			more -= diff;
			if(more < 0) more = 0;

			t = engine->param->get_tick();
		}
	}
}

GBClient GBEngineGetClient(GBEngine engine) {
	return engine->client;
}

GBServer GBEngineGetServer(GBEngine engine) {
	return engine->server;
}
