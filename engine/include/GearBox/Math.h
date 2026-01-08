#ifndef __GEARBOX_MATH_H__
#define __GEARBOX_MATH_H__

#include <GearBox/MachDep.h>
#include <GearBox/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

GBDECL void GBMathCross(double* out, double v00, double v01, double v02, double v10, double v11, double v12);
GBDECL void GBMathNormal3(double* out, double v00, double v01, double v02, double v10, double v11, double v12, double v20, double v21, double v22);
GBDECL void GBMathNormal4(double* out, double v00, double v01, double v02, double v10, double v11, double v12, double v20, double v21, double v22, double v30, double v31, double v32);

#ifdef __cplusplus
}
#endif

#endif
