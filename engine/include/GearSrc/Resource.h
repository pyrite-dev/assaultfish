#ifndef __GEARSRC_RESOURCE_H__
#define __GEARSRC_RESOURCE_H__

#include <GearSrc/MachDep.h>
#include <GearSrc/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

GSDECL GSResource GSResourceOpen(GSEngine engine, const char* path);
GSDECL void*	  GSResourceGet(GSResource resource, const char* name, unsigned int* size);
GSDECL void	  GSResourceClose(GSResource resource);

#ifdef __cplusplus
}
#endif

#endif
