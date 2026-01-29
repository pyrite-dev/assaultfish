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
typedef GSU8 GSBool;

#define GSTrue ((GSBool)1)
#define GSFalse ((GSBool)0)

typedef struct _GSVersion	  GSVersion;
typedef struct _GSEngineParam	  GSEngineParam;
typedef struct _GSResourceKV	  GSResourceKV;
typedef struct _GSBox		  GSBox;
typedef struct _GSNetPacketHeader GSNetPacketHeader;
typedef struct _GSNetPacket	  GSNetPacket;
typedef struct _GSNetAddress	  GSNetAddress;
typedef struct _GSBinary	  GSBinary;
typedef struct _GSNetState	  GSNetState;
typedef struct _GSNetKV		  GSNetKV;
#ifdef _GEARSRC
typedef struct _GSClient*      GSClient;
typedef struct _GSServer*      GSServer;
typedef struct _GSEngine*      GSEngine;
typedef struct _GSFile*	       GSFile;
typedef struct _GSResource*    GSResource;
typedef struct _GSGL*	       GSGL;
typedef struct _GSSkyBox*      GSSkyBox;
typedef struct _GSModel*       GSModel;
typedef struct _GSSoundDriver* GSSoundDriver;
typedef struct _GSSoundEngine* GSSoundEngine;
typedef struct _GSSound*       GSSound;
typedef struct _GSNetClient*   GSNetClient;
typedef struct _GSNetServer*   GSNetServer;
typedef struct _GSNetSocket*   GSNetSocket;

typedef struct _GSModelFace	   GSModelFace;
typedef struct _GSModelMaterial	   GSModelMaterial;
typedef struct _GSModelUseMaterial GSModelUseMaterial;
typedef struct _GSEngineLog	   GSEngineLog;
#else
typedef void* GSClient;
typedef void* GSServer;
typedef void* GSEngine;
typedef void* GSFile;
typedef void* GSResource;
typedef void* GSGL;
typedef void* GSSkyBox;
typedef void* GSModel;
typedef void* GSSoundDriver;
typedef void* GSSoundEngine;
typedef void* GSSound;
typedef void* GSNetClient;
typedef void* GSNetServer;
typedef void* GSNetSocket;
#endif
typedef float	 GSNumber;
typedef GSNumber GSVector2[2];
typedef GSNumber GSVector3[3];
typedef GSNumber GSVector4[4];
typedef GSNumber GSMatrix3x3[9];
typedef GSNumber GSMatrix4x4[16];

typedef void (*GSGLSwapBufferCallback)(void);
typedef void (*GSReadyCallback)(int width, int height);
typedef void (*GSTickCallback)(void);
typedef long (*GSGetTickCallback)(void);
typedef void (*GSSleepCallback)(int ms);
typedef void (*GSRenderCallback)(GSEngine engine);
typedef void (*GSAfterRenderCallback)(GSEngine engine);

typedef void (*GSSoundDriverReadCallback)(void* opaque, GSI16* out, int frame);

typedef void (*GSSoundResetCallback)(GSSound audio);
typedef int (*GSSoundReadCallback)(GSSound audio, GSI16* out, int frame);
typedef void (*GSSoundCloseCallback)(GSSound audio);

struct _GSBox {
	GSVector3 a;
	GSVector3 b;
};

struct _GSResourceKV {
	char*	   key;
	GSResource value;
};

#pragma pack(1)
struct _GSNetPacketHeader {
	GSU8  flag;
	GSU32 index;
	GSU32 seq;
};
#pragma pack()

struct _GSNetPacket {
	GSNetPacketHeader header;
	unsigned char	  data[508];
	int		  size;
};

/* i hope someone adds ipv6 support :) */
struct _GSNetAddress {
	GSU32 address;
	GSU16 port;
};

struct _GSBinary {
	void* data;
	int   size;

	int flag;
};

struct _GSNetState {
	GSEngine engine;
	char	 name[16];

	unsigned int rxseq;
	unsigned int rxindex;
	unsigned int rxtotal;

	unsigned int  txseq;
	unsigned int  txindex;
	unsigned int  txtotal;
	unsigned int  txstate;
	unsigned long txtick;

	GSBinary* rx;
	GSBinary* tx;

	void* opaque;
};

struct _GSNetKV {
	GSNetAddress key;
	GSNetState   value;
};

#ifdef _GEARSRC
#include <GearSrc/GL/GL.h>
#include <miniaudio.h>

struct _GSGL {
	GSEngine engine;

	int	 shadow_use_shader;
	GLuint	 shadow_texture;
	GLuint	 shadow_shader;
	GLdouble shadow_modelview[16];
	GLdouble shadow_projection[16];
	GLdouble shadow_old_projection[16];
	GLdouble shadow_old_modelview[16];

	GSBool bold;
};

struct _GSSkyBox {
	GSEngine engine;

	GLuint left;
	GLuint right;
	GLuint front;
	GLuint back;
	GLuint up;
	GLuint down;

	GLuint cloud0;
	GLuint cloud1;

	GSNumber tick;
};

struct _GSModelFace {
	int	   count;
	GSVector3  normal;
	GSVector3* vertex;
	GSVector2* texcoord;
};

struct _GSModelMaterial {
	char*  key;
	GLuint value;
};

struct _GSModelUseMaterial {
	int   key;
	char* value;
};

struct _GSModel {
	GSEngine engine;

	GLuint call_list;

	GSModelMaterial*    material;
	GSModelUseMaterial* usemtl;

	float*	     vertex;
	float*	     texcoord;
	GSModelFace* face;
};

struct _GSSound {
	GSEngine engine;

	GSBool paused;
	GSBool loop;

	ma_mutex mutex;

	GSFile file;

	void* opaque1;
	void* opaque2;

	int			    from_channel;
	ma_channel_converter_config converter_config;
	ma_channel_converter	    converter;

	int		    from_samplerate;
	ma_resampler_config resampler_config;
	ma_resampler	    resampler;

	GSSoundResetCallback reset;
	GSSoundReadCallback  read;
	GSSoundCloseCallback close;
};

struct _GSSoundDriver {
	GSEngine engine;

	ma_device	 device;
	ma_device_config config;

	int ready;

	GSSoundDriverReadCallback callback;
	void*			  opaque;
};

struct _GSSoundEngine {
	GSEngine engine;

	GSSoundDriver driver;

	ma_mutex mutex;

	GSSound* sound;
};

struct _GSNetClient {
	GSEngine engine;

	GSNetSocket sock;

	GSNetAddress address;
	GSNetState   state;
};

struct _GSNetServer {
	GSEngine engine;

	GSNetSocket sock;

	GSNetKV* client;
};

struct _GSNetSocket {
	int    fd;
	GSBool client;
};

struct _GSClient {
	GSEngine engine;

	GSGL gl;

	GLuint font_normal;
	int    font_normal_width;
	int    font_normal_height;
	int    glyph_normal_width;
	int    glyph_normal_height;

	GLuint font_bold;
	int    font_bold_width;
	int    font_bold_height;
	int    glyph_bold_width;
	int    glyph_bold_height;

	GSVector3 look_at;
	GSVector3 camera;
	GSVector4 light0;
	GSSkyBox  skybox;
	GSBool	  skybox_enabled;

	GSSoundEngine sengine;

	GSNetClient net;
};

struct _GSServer {
	GSEngine engine;

	GSNetServer net;
};

struct _GSEngineLog {
	char log[1024];
	int  level;
	long tick;
};

struct _GSEngine {
	GSEngineParam* param;
	GSClient       client;
	GSServer       server;
	GSResourceKV*  resource;
	int	       width;
	int	       height;
	GSNumber       tps;
	GSNumber       tps_sampled;

	GSEngineLog* log;
	int	     shutdown;
};

struct _GSFile {
	FILE*	       fp;
	unsigned char* data;
	unsigned int   size;
	unsigned int   seek;
};

struct _GSResource {
	GSEngine engine;

	GSFile file;
};
#endif

struct _GSVersion {
	char string[64];
	char copyright[256];
	int  majorlevel;
	int  minorlevel;
	int  patchlevel;
};

struct _GSEngineParam {
	int		       client;
	int		       server;
	int		       port;
	GSGLSwapBufferCallback gl_swapbuffer;
	GSReadyCallback	       ready;
	GSTickCallback	       tick;
	GSGetTickCallback      get_tick;
	GSSleepCallback	       sleep;
	GSRenderCallback       render;
	GSAfterRenderCallback  after_render;
};

#endif
