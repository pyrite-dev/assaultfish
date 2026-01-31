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

GSDECL GSPhysicsObject GSPhysicsCreateBox(GSPhysics physics, GSNumber mass, GSVector3 size);
GSDECL GSPhysicsObject GSPhysicsCreateSphere(GSPhysics physics, GSNumber mass, GSNumber radius);
GSDECL void	       GSPhysicsGetPosition(GSPhysicsObject obj, GSVector3 vec);
GSDECL void	       GSPhysicsSetPosition(GSPhysicsObject obj, GSVector3 vec);
GSDECL void	       GSPhysicsGetRotation(GSPhysicsObject obj, GSMatrix3x3 mat);
GSDECL void	       GSPhysicsDestroyObject(GSPhysicsObject obj);

#ifdef __cplusplus
}
#endif

#endif
