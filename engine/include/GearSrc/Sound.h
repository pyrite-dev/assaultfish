#ifndef __GEARSRC_SOUND_H__
#define __GEARSRC_SOUND_H__

#include <GearSrc/MachDep.h>
#include <GearSrc/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

/* sound.c */
GSDECL GSSound GSSoundNew(GSSoundEngine sengine);
GSDECL GSSound GSSoundOpen(GSSoundEngine sengine, const char* path);
GSDECL void    GSSoundClose(GSSound sound);
GSDECL void    GSSoundToggleLoop(GSSound sound, GSBool toggle);
GSDECL void    GSSoundStart(GSSound sound);
GSDECL void    GSSoundResume(GSSound sound);
GSDECL void    GSSoundPause(GSSound audio);
GSDECL void    GSSoundLock(GSSound sound);
GSDECL void    GSSoundUnlock(GSSound sound);

/* snd_xm.c */
GSDECL GSSound GSSoundOpenXM(GSSoundEngine sengine, GSFile file);

/* snd_mod.c */
GSDECL GSSound GSSoundOpenMOD(GSSoundEngine sengine, GSFile file);

#ifdef __cplusplus
}
#endif

#endif
