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

#ifdef _GEARBOX
struct _GBClient {
};

struct _GBServer {
};

struct _GBEngine {
	GBClient client;
	GBServer server;
};
#endif

struct _GBVersion {
	char string[64];
	int  majorlevel;
	int  minorlevel;
	int  patchlevel;
};

struct _GBEngineParam {
	int client;
	int server;
};

#endif
