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

	physics->world	       = dWorldCreate();
	physics->space	       = dSimpleSpaceCreate(0);
	physics->contact_group = dJointGroupCreate(0);

	dWorldSetGravity(physics->world, 0, 0, -9.81);

	GSLog(server->engine, GSLogInfo, "Created physics engine");

	return physics;
}

void GSPhysicsDestroy(GSPhysics physics) {
	dJointGroupDestroy(physics->contact_group);
	dSpaceDestroy(physics->space);
	dWorldDestroy(physics->world);

	GSLog(physics->engine, GSLogInfo, "Destroyed physics engine");

	free(physics);
}

static void near_callback(void* data, dGeomID o1, dGeomID o2) {
	const int N = 256;
	dContact  contact[256];
	GSPhysics physics = data;

	if(dGeomIsSpace(o1) || dGeomIsSpace(o2)) {
		dSpaceCollide2(o1, o2, data, near_callback);
		if(dGeomIsSpace(o1)) dSpaceCollide((dSpaceID)o1, data, near_callback);
		if(dGeomIsSpace(o2)) dSpaceCollide((dSpaceID)o2, data, near_callback);
	} else {
		int how_many = dCollide(o1, o2, N, &contact[0].geom, sizeof(dContact));
		int i;

		for(i = 0; i < how_many; i++) {
			dJointID joint;

			contact[i].surface.mode = dContactApprox1;
			contact[i].surface.mu	= 2;

			joint = dJointCreateContact(physics->world, physics->contact_group, &contact[i]);

			dJointAttach(joint, dGeomGetBody(contact[i].geom.g1), dGeomGetBody(contact[i].geom.g2));
		}
	}
}

void GSPhysicsStep(GSPhysics physics) {
	dSpaceCollide(physics->space, physics, near_callback);
	dWorldStep(physics->world, 1.0 / physics->engine->tps);
	dJointGroupEmpty(physics->contact_group);
}
