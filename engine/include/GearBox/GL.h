#ifndef __GEARBOX_GL_H__
#define __GEARBOX_GL_H__

#include <GearBox/MachDep.h>
#include <GearBox/TypeDefs.h>

#include <GearBox/GL/GL.h>

#ifdef __cplusplus
extern "C" {
#endif

/* gl_shader.c */
GBDECL int GBGLShaderPrepare(GLuint* shader, const char* vs, const char* fs);

#ifdef __cplusplus
}
#endif

#endif
