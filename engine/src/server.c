#include <GearSrc/Server.h>

#include <GearSrc/Log.h>
#include <GearSrc/Net.h>
#include <GearSrc/Physics.h>

GSServer GSServerCreate(GSEngine engine) {
	GSServer server = malloc(sizeof(*server));

	memset(server, 0, sizeof(*server));

	server->engine = engine;

	if((server->net = GSNetServerOpen(server, engine->param->port)) == NULL) {
		GSLog(engine, GSLogError, "Failed to start server");
		GSServerDestroy(server);

		return NULL;
	}

	if((server->physics = GSPhysicsCreate(server)) == NULL) {
		GSLog(engine, GSLogError, "Failed to create physics engine");
		GSServerDestroy(server);

		return NULL;
	}

	GSLog(engine, GSLogInfo, "Created server");

	return server;
}

void GSServerDestroy(GSServer server) {
	if(server->net != NULL) GSNetServerClose(server->net);
	if(server->physics != NULL) GSPhysicsDestroy(server->physics);

	GSLog(server->engine, GSLogInfo, "Destroyed server");

	free(server);
}

void GSServerStep(GSServer server) {
	GSNetServerStep(server->net);
}
