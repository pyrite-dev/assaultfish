#ifndef __GEARBOX_GL_H__
#define __GEARBOX_GL_H__

#include <GearBox/MachDep.h>
#include <GearBox/TypeDefs.h>

#include <GearBox/GL/GL.h>

#ifdef __cplusplus
extern "C" {
#endif

/* gl_shader.c */
GBDECL int GBGLShaderPrepare(GBGL gl, GLuint* shader, const char* vs, const char* fs);

/* gl_texture.c */
GBDECL int GBGLTexturePrepare(GBGL gl, GLuint* texture, unsigned char* rgba, int width, int height);

/* gl_shadow.c */
GBDECL void GBGLShadowInit(GBGL gl);
GBDECL int  GBGLShadowBeforeMapping(GBGL gl);
GBDECL void GBGLShadowAfterMapping(GBGL gl);
GBDECL void GBGLShadowEnd(GBGL gl);
GBDECL void GBGLShadowDeinit(GBGL gl);
GBDECL void GBGLShadowDisable(GBGL gl);
GBDECL void GBGLShadowEnable(GBGL gl);

/* gl.c */
GBDECL GBGL GBGLCreate(GBClient client);
GBDECL void GBGLDestroy(GBGL gl);
GBDECL void GBGLClear(GBGL gl);
GBDECL void GBGLIgnoreDepth(GBGL gl);
GBDECL void GBGLCareDepth(GBGL gl);
GBDECL void GBGLSetLight(GBGL gl);

/* gl_camera.c */
GBDECL void GBGLCameraPerspective(GBGL gl, GBNumber width, GBNumber height);
GBDECL void GBGLCameraSet(GBGL gl);
GBDECL void GBGLCameraLookAt(GBGL gl, GBVector3 camera, GBVector3 look_at);
GBDECL void GBGLCameraSetup(GBGL gl);

#ifdef __cplusplus
}
#endif

#endif
