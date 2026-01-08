#ifndef __GEARBOX_VERSION_H__
#define __GEARBOX_VERSION_H__

#include <GearBox/MachDep.h>
#include <GearBox/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

GBDECL void GBVersionGet(GBVersion* version);

#ifdef __cplusplus
}
#endif

#endif
