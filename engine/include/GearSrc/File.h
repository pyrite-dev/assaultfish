#ifndef __GEARBOX_FILE_H__
#define __GEARBOX_FILE_H__

#include <GearSrc/MachDep.h>
#include <GearSrc/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

GSDECL GSFile	    GSFileOpen(GSEngine engine, const char* path);
GSDECL int	    GSFileRead(GSFile file, void* out, int size);
GSDECL void	    GSFileSeek(GSFile file, int pos);
GSDECL unsigned int GSFileSize(GSFile file);
GSDECL void	    GSFileClose(GSFile file);

#ifdef __cplusplus
}
#endif

#endif
