#ifndef __GEARBOX_TYPEDEFS_H__
#define __GEARBOX_TYPEDEFS_H__

#include <GearBox/MachDep.h>

typedef struct _GBVersion     GBVersion;
typedef struct _GBEngineParam GBEngineParam;
typedef struct _GBResourceKV  GBResourceKV;
#ifdef _GEARBOX
typedef struct _GBClient*   GBClient;
typedef struct _GBServer*   GBServer;
typedef struct _GBEngine*   GBEngine;
typedef struct _GBFile*	    GBFile;
typedef struct _GBResource* GBResource;
#else
typedef void* GBClient;
typedef void* GBServer;
typedef void* GBEngine;
typedef void* GBFile;
typedef void* GBResource;
#endif

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
struct _GBClient {
	GBEngine engine;
};

struct _GBServer {
	GBEngine engine;
};

struct _GBEngine {
	GBEngineParam* param;
	GBClient       client;
	GBServer       server;
	GBResourceKV*  resource;
};

struct _GBFile {
	FILE* fp;
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
