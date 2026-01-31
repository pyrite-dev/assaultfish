#ifndef __GEARSRC_PHYSICS_H__
#define __GEARSRC_PHYSICS_H__

#include <GearSrc/MachDep.h>
#include <GearSrc/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

GSDECL void	 GSPhysicsInit(void);
GSDECL GSPhysics GSPhysicsCreate(GSServer server);
GSDECL void	 GSPhysicsDestroy(GSPhysics physics);
GSDECL void	 GSPhysicsStep(GSPhysics physics);

#ifdef __cplusplus
}
#endif

#endif
