#ifndef __GEARBOX_MATH_H__
#define __GEARBOX_MATH_H__

#include <GearBox/MachDep.h>
#include <GearBox/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

#define GBMathPi 3.14159265358979323846

GBDECL void   GBMathCross3(GBVector3 r, GBVector3 v0, GBVector3 v1);
GBDECL void   GBMathNormalize3(GBVector3 vec);
GBDECL void   GBMathSubtract3(GBVector3 r, GBVector3 v0, GBVector3 v1);
GBDECL void   GBMathAdd3(GBVector3 r, GBVector3 v0, GBVector3 v1);
GBDECL void   GBMathNormal3x3(GBVector3 r, GBVector3 v0, GBVector3 v1, GBVector3 v2);
GBDECL void   GBMathNormal3x4(GBVector3 r, GBVector3 v0, GBVector3 v1, GBVector3 v2, GBVector3 v3);
GBDECL void   GBMathInvert4x4(GBMatrix4x4 out, GBMatrix4x4 in);
GBDECL double GBMathCot(double x);

#ifdef __cplusplus
}
#endif

#endif
