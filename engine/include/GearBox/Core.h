#ifndef __GEARBOX_CORE_H__
#define __GEARBOX_CORE_H__

#include <GearBox/MachDep.h>
#include <GearBox/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

GBDECL void GBInit(void);

GBDECL GBEngine GBEngineCreate(GBEngineParam* param);
GBDECL void	GBEngineDestroy(GBEngine engine);
GBDECL void	GBEngineParamInit(GBEngineParam* param);
GBDECL void	GBEngineLoop(GBEngine engine);
GBDECL GBClient GBEngineGetClient(GBEngine engine);
GBDECL GBServer GBEngineGetServer(GBEngine engine);
GBDECL int	GBEngineRegisterResource(GBEngine engine, const char* name, const char* path);

#ifdef __cplusplus
}
#endif

#endif
