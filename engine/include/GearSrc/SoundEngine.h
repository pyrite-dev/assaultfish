#ifndef __GEARSRC_SOUNDENGINE_H__
#define __GEARSRC_SOUNDENGINE_H__

#include <GearSrc/MachDep.h>
#include <GearSrc/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

GSDECL GSSoundEngine GSSoundEngineCreate(GSClient client);
GSDECL void	     GSSoundEngineDestroy(GSSoundEngine sengine);
GSDECL void	     GSSoundEngineLock(GSSoundEngine sengine);
GSDECL void	     GSSoundEngineUnlock(GSSoundEngine sengine);

#ifdef __cplusplus
}
#endif

#endif
