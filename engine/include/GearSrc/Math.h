#ifndef __GEARSRC_MATH_H__
#define __GEARSRC_MATH_H__

#include <GearSrc/MachDep.h>
#include <GearSrc/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

#define GSMathPi 3.14159265358979323846

GSDECL void	GSMathCross3(GSVector3 r, GSVector3 v0, GSVector3 v1);
GSDECL GSNumber GSMathInner3(GSVector3 v0, GSVector3 v1);
GSDECL void	GSMathNormalize3(GSVector3 vec);
GSDECL void	GSMathSubtract3(GSVector3 r, GSVector3 v0, GSVector3 v1);
GSDECL void	GSMathAdd3(GSVector3 r, GSVector3 v0, GSVector3 v1);
GSDECL void	GSMathNormalTriangle(GSVector3 r, GSVector3 v0, GSVector3 v1, GSVector3 v2);
GSDECL void	GSMathNormalQuad(GSVector3 r, GSVector3 v0, GSVector3 v1, GSVector3 v2, GSVector3 v3);
GSDECL void	GSMathInvert4x4(GSMatrix4x4 out, GSMatrix4x4 in);
GSDECL void	GSMathRowToColumn4x4(GSMatrix4x4 out, GSMatrix4x4 in);
GSDECL void	GSMathRowToColumn3x3(GSMatrix3x3 out, GSMatrix3x3 in);
GSDECL void	GSMath3x3To4x4(GSMatrix4x4 out, GSMatrix3x3 in);
GSDECL GSNumber GSMathCot(GSNumber x);
GSDECL GSNumber GSMathClosestPOT(GSNumber x);

#define GSMathColumnToRow4x4 GSMathRowToColumn4x4
#define GSMathColumnToRow3x3 GSMathRowToColumn3x3

#ifdef __cplusplus
}
#endif

#endif
