#ifndef __GEARBOX_CLIENT_H__
#define __GEARBOX_CLIENT_H__

#include <GearBox/MachDep.h>
#include <GearBox/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

GBDECL GBClient GBClientCreate(GBEngine engine);
GBDECL void	GBClientDestroy(GBClient client);

#ifdef __cplusplus
}
#endif

#endif
