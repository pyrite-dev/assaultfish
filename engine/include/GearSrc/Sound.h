#ifndef __GEARSRC_SOUND_H__
#define __GEARSRC_SOUND_H__

#include <GearSrc/MachDep.h>
#include <GearSrc/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

/* src/sound/sound.c */
GSDECL GSSound GSSoundNew(GSSoundEngine sengine);
GSDECL GSSound GSSoundOpen(GSSoundEngine sengine, const char* path);
GSDECL void    GSSoundClose(GSSound sound);
GSDECL void    GSSoundSetLoop(GSSound sound, GSBool toggle);
GSDECL void    GSSoundSet3D(GSSound sound, GSBool toggle);
GSDECL void    GSSoundSetPosition(GSSound sound, GSVector3 position);
GSDECL void    GSSoundStart(GSSound sound);
GSDECL void    GSSoundResume(GSSound sound);
GSDECL void    GSSoundPause(GSSound audio);
GSDECL void    GSSoundLock(GSSound sound);
GSDECL void    GSSoundUnlock(GSSound sound);
GSDECL GSBool  GSSoundIsPaused(GSSound sound);

/* you can guess where they are */
GSDECL GSSound GSSoundOpenXM(GSSoundEngine sengine, GSFile file);
GSDECL GSSound GSSoundOpenMOD(GSSoundEngine sengine, GSFile file);
GSDECL GSSound GSSoundOpenMP3(GSSoundEngine sengine, GSFile file);
GSDECL GSSound GSSoundOpenFLAC(GSSoundEngine sengine, GSFile file);
GSDECL GSSound GSSoundOpenVorbis(GSSoundEngine sengine, GSFile file);
GSDECL GSSound GSSoundOpenWAV(GSSoundEngine sengine, GSFile file);

#ifdef __cplusplus
}
#endif

#endif
