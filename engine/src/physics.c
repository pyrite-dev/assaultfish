#include <GearSrc/Physics.h>

#include <GearSrc/Log.h>

#include <ode/ode.h>

void GSPhysicsInit(void) {
	dInitODE2(0);
}

GSPhysics GSPhysicsCreate(GSServer server) {
	GSPhysics physics = malloc(sizeof(*physics));

	memset(physics, 0, sizeof(*physics));

	physics->engine = server->engine;

	physics->world = dWorldCreate();
	physics->space = dSimpleSpaceCreate(0);

	GSLog(server->engine, GSLogInfo, "Created physics engine");

	return physics;
}

void GSPhysicsDestroy(GSPhysics physics) {
	dSpaceDestroy(physics->space);
	dWorldDestroy(physics->world);

	GSLog(physics->engine, GSLogInfo, "Destroyed physics engine");

	free(physics);
}

void GSPhysicsStep(GSPhysics physics) {
}
