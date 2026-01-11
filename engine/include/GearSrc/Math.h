#ifndef __GEARBOX_MATH_H__
#define __GEARBOX_MATH_H__

#include <GearSrc/MachDep.h>
#include <GearSrc/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

#define GSMathPi 3.14159265358979323846

GSDECL void	GSMathCross3(GSVector3 r, GSVector3 v0, GSVector3 v1);
GSDECL void	GSMathNormalize3(GSVector3 vec);
GSDECL void	GSMathSubtract3(GSVector3 r, GSVector3 v0, GSVector3 v1);
GSDECL void	GSMathAdd3(GSVector3 r, GSVector3 v0, GSVector3 v1);
GSDECL void	GSMathNormal3x3(GSVector3 r, GSVector3 v0, GSVector3 v1, GSVector3 v2);
GSDECL void	GSMathNormal3x4(GSVector3 r, GSVector3 v0, GSVector3 v1, GSVector3 v2, GSVector3 v3);
GSDECL void	GSMathInvert4x4(GSMatrix4x4 out, GSMatrix4x4 in);
GSDECL GSNumber GSMathCot(GSNumber x);

#ifdef __cplusplus
}
#endif

#endif
