#ifndef __GEARBOX_FILE_H__
#define __GEARBOX_FILE_H__

#include <GearBox/MachDep.h>
#include <GearBox/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

GBDECL GBFile GBFileOpen(GBEngine engine, const char* path);
GBDECL int    GBFileRead(GBFile file, void* out, int size);
GBDECL void   GBFileClose(GBFile file);

#ifdef __cplusplus
}
#endif

#endif
