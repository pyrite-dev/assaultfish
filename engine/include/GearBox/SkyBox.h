#ifndef __GEARBOX_SKYBOX_H__
#define __GEARBOX_SKYBOX_H__

#include <GearBox/MachDep.h>
#include <GearBox/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

GBDECL GBSkyBox GBSkyBoxOpen(GBClient client, const char* base);
GBDECL void	GBSkyBoxDraw(GBSkyBox skybox);
GBDECL void	GBSkyBoxClose(GBSkyBox skybox);

#ifdef __cplusplus
}
#endif

#endif
