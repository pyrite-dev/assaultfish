#ifndef __GEARBOX_TYPEDEFS_H__
#define __GEARBOX_TYPEDEFS_H__

#include <GearBox/MachDep.h>

#if defined(_MSC_VER) || defined(__WATCOMC__)
typedef __int8_t  GBI8;
typedef __int16_t GBI16;
typedef __int32_t GBI32;
typedef __int64_t GBI64;

typedef unsigned __int8_t  GBU8;
typedef unsigned __int16_t GBU16;
typedef unsigned __int32_t GBU32;
typedef unsigned __int64_t GBU64;
#elif __STDC_VERSION__ >= 199901L
#include <stdint.h>

typedef int8_t	GBI8;
typedef int16_t GBI16;
typedef int32_t GBI32;
typedef int64_t GBI64;

typedef uint8_t	 GBU8;
typedef uint16_t GBU16;
typedef uint32_t GBU32;
typedef uint64_t GBU64;
#else
typedef signed char GBI8;
typedef short	    GBI16;
typedef int	    GBI32;
typedef long	    GBI64;

typedef unsigned char  GBU8;
typedef unsigned short GBU16;
typedef unsigned int   GBU32;
typedef unsigned long  GBU64;
#endif

typedef struct _GBVersion     GBVersion;
typedef struct _GBEngineParam GBEngineParam;
typedef struct _GBResourceKV  GBResourceKV;
#ifdef _GEARBOX
typedef struct _GBClient*   GBClient;
typedef struct _GBServer*   GBServer;
typedef struct _GBEngine*   GBEngine;
typedef struct _GBFile*	    GBFile;
typedef struct _GBResource* GBResource;
typedef struct _GBGL*	    GBGL;
#else
typedef void* GBClient;
typedef void* GBServer;
typedef void* GBEngine;
typedef void* GBFile;
typedef void* GBResource;
typedef void* GBGL;
#endif
typedef double GBVector3[3];
typedef double GBVector4[4];
typedef double GBMatrix4x4[16];

typedef void (*GBGLSwapBufferCallback)(void);
typedef void (*GBReadyCallback)(int width, int height);
typedef void (*GBTickCallback)(void);
typedef long (*GBGetTickCallback)(void);
typedef void (*GBSleepCallback)(int ms);

struct _GBResourceKV {
	char*	   key;
	GBResource value;
};

#ifdef _GEARBOX
#include <GearBox/GL/GL.h>

struct _GBGL {
	GBEngine engine;

	GLuint	 shadow_texture;
	GLuint	 shadow_shader;
	GLdouble shadow_modelview[16];
	GLdouble shadow_projection[16];
	GLdouble shadow_old_projection[16];
	GLdouble shadow_old_modelview[16];
};

struct _GBClient {
	GBEngine engine;

	GBGL	  gl;
	GBVector3 look_at;
	GBVector3 camera;
	GBVector4 light0;
};

struct _GBServer {
	GBEngine engine;
};

struct _GBEngine {
	GBEngineParam* param;
	GBClient       client;
	GBServer       server;
	GBResourceKV*  resource;
	int	       width;
	int	       height;
};

struct _GBFile {
	FILE*	       fp;
	unsigned char* data;
	unsigned int   size;
	unsigned int   seek;
};

struct _GBResource {
	GBFile file;
};
#endif

struct _GBVersion {
	char string[64];
	int  majorlevel;
	int  minorlevel;
	int  patchlevel;
};

struct _GBEngineParam {
	int		       client;
	int		       server;
	GBGLSwapBufferCallback gl_swapbuffer;
	GBReadyCallback	       ready;
	GBTickCallback	       tick;
	GBGetTickCallback      get_tick;
	GBSleepCallback	       sleep;
};

#endif
