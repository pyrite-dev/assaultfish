#ifndef __GEARBOX_CORE_H__
#define __GEARBOX_CORE_H__

#include <GearBox/MachDep.h>
#include <GearBox/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

GBDECL void	GBInit(void);
GBDECL GBEngine GBEngineCreate(GBEngineParam* param);
GBDECL void	GBEngineDestroy(GBEngine engine);
GBDECL void	GBEngineStart(GBEngine engine);
GBDECL void	GBEngineLoop(GBEngine engine);

#ifdef __cplusplus
}
#endif

#endif
