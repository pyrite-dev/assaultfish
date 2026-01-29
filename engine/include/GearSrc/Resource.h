#ifndef __GEARSRC_RESOURCE_H__
#define __GEARSRC_RESOURCE_H__

#include <GearSrc/MachDep.h>
#include <GearSrc/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * signature: 0x7F PAK
 *
 * entry:
 *   1 byte: type
 *   4 bytes: name crc32
 *   1 byte: name len
 *   n bytes: name
 *   if file {
 *     4 bytes: actual size
 *     4 bytes: compressed size
 *     4 bytes: seek pos
 *   } else {
 *     2 bytes: number of entries
 *     4 bytes: total size
 *     n bytes: entries
 *   }
 */

GSDECL GSResource GSResourceOpen(GSEngine engine, const char* path);
GSDECL void*	  GSResourceGet(GSResource resource, const char* name, unsigned int* size);
GSDECL void	  GSResourceClose(GSResource resource);

#ifdef __cplusplus
}
#endif

#endif
