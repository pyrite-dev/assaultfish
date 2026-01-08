#ifndef __GEARBOX_SERVER_H__
#define __GEARBOX_SERVER_H__

#include <GearBox/MachDep.h>
#include <GearBox/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

GBDECL GBServer GBServerCreate(GBEngine engine);
GBDECL void	GBServerDestroy(GBServer server);

#ifdef __cplusplus
}
#endif

#endif
