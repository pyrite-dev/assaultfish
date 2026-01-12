#ifndef __GEARSRC_MODEL_H__
#define __GEARSRC_MODEL_H__

#include <GearSrc/MachDep.h>
#include <GearSrc/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

GSDECL GSModel GSModelOpen(GSEngine engine, const char* path);
GSDECL void    GSModelDraw(GSModel model, GSVector3 pos, GSVector3 rot);
GSDECL void    GSModelClose(GSModel model);

#ifdef __cplusplus
}
#endif

#endif
