#ifndef __GEARSRC_SOUND_H__
#define __GEARSRC_SOUND_H__

#include <GearSrc/MachDep.h>
#include <GearSrc/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

#define GSSoundRate 48000

GSDECL GSSound GSSoundOpen(GSClient client);
GSDECL void    GSSoundClose(GSSound sound);

#ifdef __cplusplus
}
#endif

#endif
