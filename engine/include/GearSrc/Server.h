#ifndef __GEARBOX_SERVER_H__
#define __GEARBOX_SERVER_H__

#include <GearSrc/MachDep.h>
#include <GearSrc/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

GSDECL GSServer GSServerCreate(GSEngine engine);
GSDECL void	GSServerDestroy(GSServer server);
GSDECL void	GSServerStep(GSServer server);

#ifdef __cplusplus
}
#endif

#endif
