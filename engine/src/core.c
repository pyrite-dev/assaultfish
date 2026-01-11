#include <GearSrc/Core.h>

#include <GearSrc/Version.h>
#include <GearSrc/Log.h>
#include <GearSrc/Client.h>
#include <GearSrc/Server.h>
#include <GearSrc/GL.h>
#include <GearSrc/Resource.h>

#include <stb_ds.h>

void GSInit(void) {
	GSVersion version;

	GSVersionGet(&version);

	GSLog(GSLogInfo, "GearSrc Engine %s", version.string);
}

int GSEngineRegisterResource(GSEngine engine, const char* name, const char* path) {
	GSResource resource;
	if((resource = GSResourceOpen(engine, path)) != NULL) {
		shput(engine->resource, name, resource);

		return 1;
	}

	return 0;
}

GSEngine GSEngineCreate(GSEngineParam* param) {
	GSEngine engine = malloc(sizeof(*engine));

	memset(engine, 0, sizeof(*engine));

	engine->param = malloc(sizeof(*engine->param));
	memcpy(engine->param, param, sizeof(*param));

	engine->width  = 1024;
	engine->height = 768;

	if(param->ready != NULL && param->client) {
		param->ready(engine->width, engine->height);
	}

	gload_init();

	sh_new_strdup(engine->resource);

	GSEngineRegisterResource(engine, "base", "base.pak");
	GSEngineRegisterResource(engine, "game", "game.pak");

	if(engine != NULL && param->client && (engine->client = GSClientCreate(engine)) == NULL) {
		GSLog(GSLogError, "Failed to create client");

		GSEngineDestroy(engine);

		engine = NULL;
	}

	if(engine != NULL && param->server && (engine->server = GSServerCreate(engine)) == NULL) {
		GSLog(GSLogError, "Failed to create server");

		GSEngineDestroy(engine);

		engine = NULL;
	}

	if(engine == NULL) {
		GSLog(GSLogError, "Failed to create engine");
	}

	return engine;
}

void GSEngineDestroy(GSEngine engine) {
	int i;
	for(i = 0; i < shlen(engine->resource); i++) {
		GSResourceClose(engine->resource[i].value);
	}
	shfree(engine->resource);

	if(engine->server != NULL) GSServerDestroy(engine->server);
	if(engine->client != NULL) GSClientDestroy(engine->client);
	if(engine->param != NULL) free(engine->param);

	free(engine);
}

void GSEngineParamInit(GSEngineParam* param) {
	memset(param, 0, sizeof(*param));

	param->server	     = 0;
	param->client	     = 1;
	param->gl_swapbuffer = NULL;
	param->ready	     = NULL;
	param->sleep	     = NULL;
	param->get_tick	     = NULL;
}

void GSEngineLoop(GSEngine engine) {
	long t		   = 0;
	int  wait_actually = 0;
	int  more	   = 0;
	long wait	   = 1000 / 50.0;

	if(engine->param->sleep == NULL || engine->param->get_tick == NULL) {
		GSLog(GSLogWarn, "sleep and/or get_tick parameter are missing!");
		GSLog(GSLogWarn, "This will result game to be crazy fast!!!");
	} else {
		t = engine->param->get_tick();

		wait_actually = 1;
	}

	while(1) {
		if(engine->client != NULL) GSClientStep(engine->client);
		if(engine->server != NULL) GSServerStep(engine->server);

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

GSClient GSEngineGetClient(GSEngine engine) {
	return engine->client;
}

GSServer GSEngineGetServer(GSEngine engine) {
	return engine->server;
}
