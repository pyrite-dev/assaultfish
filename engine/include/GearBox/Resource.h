#ifndef __GEARBOX_RESOURCE_H__
#define __GEARBOX_RESOURCE_H__

#include <GearBox/MachDep.h>
#include <GearBox/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

GBDECL GBResource GBResourceOpen(GBEngine engine, const char* path);
GBDECL void	  GBResourceClose(GBResource resource);

#ifdef __cplusplus
}
#endif

#endif
