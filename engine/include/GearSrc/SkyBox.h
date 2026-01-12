#ifndef __GEARSRC_SKYBOX_H__
#define __GEARSRC_SKYBOX_H__

#include <GearSrc/MachDep.h>
#include <GearSrc/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

GSDECL GSSkyBox GSSkyBoxOpen(GSClient client, const char* base);
GSDECL GSSkyBox GSSkyBoxTry(GSClient client, const char* name);
GSDECL void	GSSkyBoxDraw(GSSkyBox skybox);
GSDECL void	GSSkyBoxClose(GSSkyBox skybox);

#ifdef __cplusplus
}
#endif

#endif
