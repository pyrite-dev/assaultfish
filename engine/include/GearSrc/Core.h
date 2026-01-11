#ifndef __GEARSRC_CORE_H__
#define __GEARSRC_CORE_H__

#include <GearSrc/MachDep.h>
#include <GearSrc/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

GSDECL void GSInit(void);

GSDECL GSEngine GSEngineCreate(GSEngineParam* param);
GSDECL void	GSEngineDestroy(GSEngine engine);
GSDECL void	GSEngineParamInit(GSEngineParam* param);
GSDECL void	GSEngineLoop(GSEngine engine);
GSDECL GSClient GSEngineGetClient(GSEngine engine);
GSDECL GSServer GSEngineGetServer(GSEngine engine);
GSDECL int	GSEngineRegisterResource(GSEngine engine, const char* name, const char* path);

#ifdef __cplusplus
}
#endif

#endif
