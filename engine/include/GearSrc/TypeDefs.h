#ifndef __GEARSRC_TYPEDEFS_H__
#define __GEARSRC_TYPEDEFS_H__

#include <GearSrc/MachDep.h>

#if defined(_MSC_VER) || defined(__WATCOMC__)
typedef __int8	GSI8;
typedef __int16 GSI16;
typedef __int32 GSI32;
typedef __int64 GSI64;

typedef unsigned __int8	 GSU8;
typedef unsigned __int16 GSU16;
typedef unsigned __int32 GSU32;
typedef unsigned __int64 GSU64;
#elif __STDC_VERSION__ >= 199901L
#include <stdint.h>

typedef int8_t	GSI8;
typedef int16_t GSI16;
typedef int32_t GSI32;
typedef int64_t GSI64;

typedef uint8_t	 GSU8;
typedef uint16_t GSU16;
typedef uint32_t GSU32;
typedef uint64_t GSU64;
#else
typedef signed char GSI8;
typedef short	    GSI16;
typedef int	    GSI32;
typedef long	    GSI64;

typedef unsigned char  GSU8;
typedef unsigned short GSU16;
typedef unsigned int   GSU32;
typedef unsigned long  GSU64;
#endif

typedef struct _GSVersion     GSVersion;
typedef struct _GSEngineParam GSEngineParam;
typedef struct _GSResourceKV  GSResourceKV;
#ifdef _GEARSRC
typedef struct _GSClient*   GSClient;
typedef struct _GSServer*   GSServer;
typedef struct _GSEngine*   GSEngine;
typedef struct _GSFile*	    GSFile;
typedef struct _GSResource* GSResource;
typedef struct _GSGL*	    GSGL;
typedef struct _GSSkyBox*   GSSkyBox;
#else
typedef void* GSClient;
typedef void* GSServer;
typedef void* GSEngine;
typedef void* GSFile;
typedef void* GSResource;
typedef void* GSGL;
typedef void* GSSkyBox;
#endif
typedef float	 GSNumber;
typedef GSNumber GSVector2[2];
typedef GSNumber GSVector3[3];
typedef GSNumber GSVector4[4];
typedef GSNumber GSMatrix4x4[16];

typedef void (*GSGLSwapBufferCallback)(void);
typedef void (*GSReadyCallback)(int width, int height);
typedef void (*GSTickCallback)(void);
typedef long (*GSGetTickCallback)(void);
typedef void (*GSSleepCallback)(int ms);

struct _GSResourceKV {
	char*	   key;
	GSResource value;
};

#ifdef _GEARSRC
#include <GearSrc/GL/GL.h>

struct _GSGL {
	GSEngine engine;

	int	 shadow_use_shader;
	GLuint	 shadow_texture;
	GLuint	 shadow_shader;
	GLdouble shadow_modelview[16];
	GLdouble shadow_projection[16];
	GLdouble shadow_old_projection[16];
	GLdouble shadow_old_modelview[16];
};

struct _GSClient {
	GSEngine engine;

	GSGL	  gl;
	GSVector3 look_at;
	GSVector3 camera;
	GSVector4 light0;
	GSSkyBox  skybox;
};

struct _GSSkyBox {
	GSEngine engine;

	GLuint left;
	GLuint right;
	GLuint front;
	GLuint back;
	GLuint up;
	GLuint down;
};

struct _GSServer {
	GSEngine engine;
};

struct _GSEngine {
	GSEngineParam* param;
	GSClient       client;
	GSServer       server;
	GSResourceKV*  resource;
	int	       width;
	int	       height;
};

struct _GSFile {
	FILE*	       fp;
	unsigned char* data;
	unsigned int   size;
	unsigned int   seek;
};

struct _GSResource {
	GSFile file;
};
#endif

struct _GSVersion {
	char string[64];
	int  majorlevel;
	int  minorlevel;
	int  patchlevel;
};

struct _GSEngineParam {
	int		       client;
	int		       server;
	GSGLSwapBufferCallback gl_swapbuffer;
	GSReadyCallback	       ready;
	GSTickCallback	       tick;
	GSGetTickCallback      get_tick;
	GSSleepCallback	       sleep;
};

#endif
