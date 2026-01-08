#ifndef __GEARBOX_TYPEDEFS_H__
#define __GEARBOX_TYPEDEFS_H__

typedef struct _GBVersion     GBVersion;
typedef struct _GBEngineParam GBEngineParam;
#ifdef _GEARBOX
typedef struct _GBClient* GBClient;
typedef struct _GBServer* GBServer;
typedef struct _GBEngine* GBEngine;
#else
typedef void* GBClient;
typedef void* GBServer;
typedef void* GBEngine;
#endif

typedef void (*GBGLSwapBufferCallback)(void);
typedef void (*GBReadyCallback)(int width, int height);
typedef void (*GBTickCallback)(void);
typedef long (*GBGetTickCallback)(void);
typedef void (*GBSleepCallback)(int ms);

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
