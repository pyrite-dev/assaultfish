#ifndef __GEARBOX_ENDIAN_H__
#define __GEARBOX_ENDIAN_H__

#include <GearBox/MachDep.h>
#include <GearBox/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_M_IX86) || defined(__x86_64__) || defined(__i386__)
#define GBEndianHostLittle
#else
#define GBEndianHostBig
#endif

#ifdef GBEndianHostLittle
GBDECL GBI16 GBEndianSwapI16BE(GBI16 in);
GBDECL GBI32 GBEndianSwapI32BE(GBI32 in);
GBDECL GBI64 GBEndianSwapI64BE(GBI64 in);

GBDECL GBU16 GBEndianSwapU16BE(GBU16 in);
GBDECL GBU32 GBEndianSwapU32BE(GBU32 in);
GBDECL GBU64 GBEndianSwapU64BE(GBU64 in);
#else
#define GBEndianSwapI16BE(in) (in)
#define GBEndianSwapI32BE(in) (in)
#define GBEndianSwapI64BE(in) (in)

#define GBEndianSwapU16BE(in) (in)
#define GBEndianSwapU32BE(in) (in)
#define GBEndianSwapU64BE(in) (in)
#endif

#ifdef __cplusplus
}
#endif

#endif
