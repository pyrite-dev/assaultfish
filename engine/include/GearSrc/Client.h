#ifndef __GEARSRC_CLIENT_H__
#define __GEARSRC_CLIENT_H__

#include <GearSrc/MachDep.h>
#include <GearSrc/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

GSDECL GSClient	     GSClientCreate(GSEngine engine);
GSDECL void	     GSClientDestroy(GSClient client);
GSDECL void	     GSClientStep(GSClient client);
GSDECL GSGL	     GSClientGetGL(GSClient client);
GSDECL GSSoundEngine GSClientGetSoundEngine(GSClient client);
GSDECL void	     GSClientSetSkybox(GSClient client, GSBool toggle);

#ifdef __cplusplus
}
#endif

#endif
