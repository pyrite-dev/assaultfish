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

/* gl.c */
GBDECL GBGL GBGLCreate(GBClient client);
GBDECL void GBGLDestroy(GBGL gl);

/* gl_camera.c */
GBDECL void GBGLPerspective(GBGL gl, double width, double height);
GBDECL void GBGLSetCamera(GBGL gl);
GBDECL void GBGLLookAt(GBGL gl, GBVector3 camera, GBVector3 look_at);

#ifdef __cplusplus
}
#endif

#endif
