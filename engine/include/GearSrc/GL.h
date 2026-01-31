#ifndef __GEARSRC_GL_H__
#define __GEARSRC_GL_H__

#include <GearSrc/MachDep.h>
#include <GearSrc/TypeDefs.h>

#include <GearSrc/GL/GL.h>

#ifdef __cplusplus
extern "C" {
#endif

#define GSGLMaxDistance 100.0
#define GSGLMaxShadowDistance 1024

/* gl_shader.c */
GSDECL GSBool GSGLShaderPrepare(GSGL gl, GLuint* shader, const char* vs, const char* fs);

/* gl_texture.c */
GSDECL void GSGLTextureLoad(GSGL gl, GLuint* texture, unsigned char* rgba, int width, int height); /* you probably don't want this. use GSGLTexturePrepare */
GSDECL void GSGLTextureSet(GSGL gl, GLuint texture);
GSDECL void GSGLTextureDelete(GSGL gl, GLuint texture);

/* gl_common.c */
GSDECL void   GSGLTexturePrepareEx(GSGL gl, GLuint* texture, unsigned char* rgba, int width, int height, int expand);
GSDECL void   GSGLTexturePrepare(GSGL gl, GLuint* texture, unsigned char* rgba, int width, int height);
GSDECL GSBool GSGLTextureLoadFile(GSGL gl, GLuint* texture, int* width, int* height, const char* filename);
GSDECL GSBool GSGLTextureLoadFileEx(GSGL gl, GLuint* texture, int* width, int* height, const char* filename, int expand);
GSDECL GSBool GSGLTextureTry(GSGL gl, GLuint* texture, int* width, int* height, const char* prefix);
GSDECL GSBool GSGLTextureTryEx(GSGL gl, GLuint* texture, int* width, int* height, const char* prefix, int expand);
GSDECL void   GSGLTextBold(GSGL gl, GSBool bold);
GSDECL void   GSGLTextEx(GSGL gl, GSNumber x, GSNumber y, const char* text, GSNumber sx, GSNumber sy);
GSDECL void   GSGLText(GSGL gl, GSNumber x, GSNumber y, const char* text);
GSDECL double GSGLTextWidth(GSGL gl, const char* text);
GSDECL double GSGLTextHeight(GSGL gl, const char* text);
GSDECL void   GSGLTetrakis(GSGL gl, GSNumber scale, GSVector4 col1, GSVector4 col2);

/* gl_shadow.c */
GSDECL void   GSGLShadowInit(GSGL gl);
GSDECL GSBool GSGLShadowBeforeMapping(GSGL gl);
GSDECL void   GSGLShadowAfterMapping(GSGL gl);
GSDECL void   GSGLShadowEnd(GSGL gl);
GSDECL void   GSGLShadowDeinit(GSGL gl);
GSDECL void   GSGLShadowDisable(GSGL gl);
GSDECL void   GSGLShadowEnable(GSGL gl);

/* gl.c */
GSDECL GSGL   GSGLCreate(GSClient client);
GSDECL void   GSGLDestroy(GSGL gl);
GSDECL void   GSGLClear(GSGL gl);
GSDECL void   GSGLIgnoreDepth(GSGL gl);
GSDECL void   GSGLCareDepth(GSGL gl);
GSDECL void   GSGLSetLight(GSGL gl);
GSDECL void   GSGLPolygon(GSGL gl, int pairs, GSVector3* vert, GSVector2* tex, GSVector3 norm);
GSDECL void   GSGLPolygon2D(GSGL gl, int pairs, GSVector2* vert, GSVector2* tex);
GSDECL void   GSGLBegin2D(GSGL gl);
GSDECL void   GSGLEnd2D(GSGL gl);
GSDECL void   GSGLSetPosition(GSGL gl, GSVector3 pos);
GSDECL void   GSGLSetRotation(GSGL gl, GSVector3 rot);
GSDECL void   GSGLSetRotation3x3(GSGL gl, GSMatrix3x3 rot);
GSDECL void   GSGLPushMatrix(GSGL gl);
GSDECL void   GSGLPopMatrix(GSGL gl);
GSDECL GLuint GSGLBeginList(GSGL gl);
GSDECL void   GSGLEndList(GSGL gl);
GSDECL void   GSGLCallList(GSGL gl, GLuint list);
GSDECL void   GSGLSetColor(GSGL gl, GSVector4 color);
GSDECL void   GSGLClearDepth(GSGL gl);
GSDECL void   GSGLInitialTranslation(GSGL gl);

/* gl_camera.c */
GSDECL void GSGLCameraPerspective(GSGL gl, GSNumber width, GSNumber height);
GSDECL void GSGLCameraSet(GSGL gl);
GSDECL void GSGLCameraLookAt(GSGL gl, GSVector3 camera, GSVector3 look_at);
GSDECL void GSGLCameraSetup(GSGL gl);

#ifdef __cplusplus
}
#endif

#endif
