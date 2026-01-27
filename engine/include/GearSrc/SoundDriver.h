#ifndef __GEARSRC_SOUNDDRIVER_H__
#define __GEARSRC_SOUNDDRIVER_H__

#include <GearSrc/MachDep.h>
#include <GearSrc/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

#define GSSoundDriverRate 48000

GSDECL GSSoundDriver GSSoundDriverOpen(GSClient client, GSSoundDriverReadCallback callback, void* opaque);
GSDECL void	     GSSoundDriverClose(GSSoundDriver sound);

#ifdef __cplusplus
}
#endif

#endif
