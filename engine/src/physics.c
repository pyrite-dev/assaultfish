#include <GearSrc/Physics.h>

#include <GearSrc/Log.h>

#include <ode/ode.h>

void GSPhysicsInit(void){
	dInitODE2(0);
}
