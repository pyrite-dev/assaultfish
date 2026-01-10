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

/* gl.c */
GBDECL GBGL GBGLCreate(GBClient client);
GBDECL void GBGLDestroy(GBGL gl);

#ifdef __cplusplus
}
#endif

#endif
