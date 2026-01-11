#ifndef __GEARBOX_STRING_H__
#define __GEARBOX_STRING_H__

#include <GearBox/MachDep.h>
#include <GearBox/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

GBDECL char* GBStringDuplicate(const char* str);
GBDECL char* GBStringConcat(const char* str1, const char* str2);

#ifdef __cplusplus
}
#endif

#endif
