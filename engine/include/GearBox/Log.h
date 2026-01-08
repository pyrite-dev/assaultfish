#ifndef __GEARBOX_LOG_H__
#define __GEARBOX_LOG_H__

#include <GearBox/MachDep.h>
#include <GearBox/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

enum GBLogLevel {
	GBLogInfo = 0,
	GBLogWarn,
	GBLogError
};

GBDECL void GBLog(int level, const char* fmt, ...);

#ifdef __cplusplus
}
#endif

#endif
