#include <GearSrc/Server.h>

#include <GearSrc/Log.h>
#include <GearSrc/Net.h>

GSServer GSServerCreate(GSEngine engine) {
	GSServer server = malloc(sizeof(*server));

	memset(server, 0, sizeof(*server));

	server->engine = engine;

	if((server->net = GSNetServerOpen(server, engine->param->port)) == NULL) {
		GSLog(engine, GSLogError, "Failed to start server");
		GSServerDestroy(server);

		return NULL;
	}

	GSLog(engine, GSLogInfo, "Created server");

	return server;
}

void GSServerDestroy(GSServer server) {
	if(server->net != NULL) GSNetServerClose(server->net);

	GSLog(server->engine, GSLogInfo, "Destroyed server");

	free(server);
}

void GSServerStep(GSServer server) {
}
