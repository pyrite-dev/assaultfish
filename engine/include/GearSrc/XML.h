#ifndef __GEARSRC_XML_H__
#define __GEARSRC_XML_H__

#include <GearSrc/MachDep.h>
#include <GearSrc/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

GSDECL GSXML GSXMLOpen(GSEngine engine, const char* path);
GSDECL void  GSXMLClose(GSXML xml);

#ifdef __cplusplus
}
#endif

#endif
