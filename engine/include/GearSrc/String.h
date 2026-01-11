#ifndef __GEARBOX_STRING_H__
#define __GEARBOX_STRING_H__

#include <GearSrc/MachDep.h>
#include <GearSrc/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

GSDECL char* GSStringDuplicate(const char* str);
GSDECL char* GSStringConcat(const char* str1, const char* str2);

#ifdef __cplusplus
}
#endif

#endif
