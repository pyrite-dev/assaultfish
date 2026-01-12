#ifndef __GEARSRC_FILE_H__
#define __GEARSRC_FILE_H__

#include <GearSrc/MachDep.h>
#include <GearSrc/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

/* file.c */
GSDECL GSFile	    GSFileOpen(GSEngine engine, const char* path);
GSDECL int	    GSFileRead(GSFile file, void* out, int size);
GSDECL void	    GSFileSeek(GSFile file, int pos);
GSDECL void	    GSFileSeekFromCurrent(GSFile file, int pos);
GSDECL unsigned int GSFileSize(GSFile file);
GSDECL void	    GSFileClose(GSFile file);

/* getline.c */
GSDECL int GSFileGetLine(char** lineptr, int* n, FILE* fp);

#ifdef __cplusplus
}
#endif

#endif
