#ifndef __GEARSRC_CRC_H__
#define __GEARSRC_CRC_H__

#include <GearSrc/MachDep.h>
#include <GearSrc/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

GSDECL GSU32 GSCRC32(void* input, unsigned int len);

#ifdef __cplusplus
}
#endif

#endif
