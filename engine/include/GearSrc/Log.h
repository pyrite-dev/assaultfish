#ifndef __GEARSRC_LOG_H__
#define __GEARSRC_LOG_H__

#include <GearSrc/MachDep.h>
#include <GearSrc/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

enum GSLogLevel {
	GSLogInfo = 0,
	GSLogWarn,
	GSLogError,
	GSLogDebug
};

GSDECL void GSLog(int level, const char* fmt, ...);

#ifdef __cplusplus
}
#endif

#endif
