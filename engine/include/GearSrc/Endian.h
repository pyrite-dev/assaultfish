#ifndef __GEARSRC_ENDIAN_H__
#define __GEARSRC_ENDIAN_H__

#include <GearSrc/MachDep.h>
#include <GearSrc/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_M_IX86) || defined(__x86_64__) || defined(__i386__)
#define GSEndianHostLittle
#else
#define GSEndianHostBig
#endif

#ifdef GSEndianHostLittle
GSDECL GSI16 GSEndianSwapI16BE(GSI16 in);
GSDECL GSI32 GSEndianSwapI32BE(GSI32 in);
GSDECL GSI64 GSEndianSwapI64BE(GSI64 in);

GSDECL GSU16 GSEndianSwapU16BE(GSU16 in);
GSDECL GSU32 GSEndianSwapU32BE(GSU32 in);
GSDECL GSU64 GSEndianSwapU64BE(GSU64 in);

GSDECL GSNumber GSEndianSwapNumberBE(GSNumber in);
#else
#define GSEndianSwapI16BE(in) (in)
#define GSEndianSwapI32BE(in) (in)
#define GSEndianSwapI64BE(in) (in)

#define GSEndianSwapU16BE(in) (in)
#define GSEndianSwapU32BE(in) (in)
#define GSEndianSwapU64BE(in) (in)

#define GSEndianSwapNumberBE(in) (in)
#endif

#ifdef __cplusplus
}
#endif

#endif
