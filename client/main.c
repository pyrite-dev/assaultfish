#define MW_OPENGL_NO_INCLUDE
#define _MILSKO

#include <GearSrc/Foundation.h>

#include <Mw/Milsko.h>
#include <Mw/Widget/OpenGL.h>

#ifndef _WIN32
#include <signal.h>
#endif

static MwWidget window, opengl;
static GSEngine engine;

static void gl_swapbuffer(void){
	MwOpenGLSwapBuffer(opengl);
}

static void ready(int width, int height){
	MwSizeHints sh;

	sh.min_width = sh.max_width = width;
	sh.min_height = sh.max_height = height;

	MwLibraryInit();

	window = MwVaCreateWidget(MwWindowClass, "main", NULL, MwDEFAULT, MwDEFAULT, width, height,
		MwNtitle, "AssaultFish",
		MwNsizeHints, &sh,
	NULL);
	opengl = MwCreateWidget(MwOpenGLClass, "opengl", window, 0, 0, width, height);

	MwOpenGLMakeCurrent(opengl);
}

static void tick(void){
	while(!window->close && MwPending(window)) MwStep(window);
}

static GSVector3 rot = {0, 0, 0};
static GSVector3 pos = {0, 0, 0};
static GSModel model = NULL;

static void render(GSEngine self){
	GSGL gl = GSClientGetGL(GSEngineGetClient(self));

	pos[1] = 0;

	GSGLPushMatrix(gl);
	GSGLSetPosition(gl, pos);
	GSGLSetRotation(gl, rot);
	if(model != NULL) GSModelDraw(model);
	GSGLPopMatrix(gl);

	pos[1] = 1;

	GSGLPushMatrix(gl);
	GSGLSetPosition(gl, pos);
	GSGLSetRotation(gl, rot);
	if(model != NULL) GSModelDraw(model);
	GSGLPopMatrix(gl);
}

static void after_render(GSEngine self){
	rot[0] = rot[1] = rot[2] += (1.0 / GSEngineGetTPS(self)) * 90;

	pos[0] = cos(rot[0] / 180 * GSMathPi);
	pos[2] = sin(rot[0] / 180 * GSMathPi);
}

#ifndef _WIN32
static void shutdown_engine(int sig){
	GSEngineShutdown(engine);
}
#endif

int main(int argc, char** argv){
	GSEngineParam param;

	GSEngineParamInit(&param);
	param.ready = ready;
	param.tick = tick;
	param.get_tick = MwTimeGetTick;
	param.sleep = MwTimeSleep;
	param.gl_swapbuffer = gl_swapbuffer;
	param.render = render;
	param.after_render = after_render;

	GSInit();

	if((engine = GSEngineCreate(&param)) != NULL){
		GSSound sound;

#ifndef _WIN32
		signal(SIGINT, shutdown_engine);
		signal(SIGTERM, shutdown_engine);
#endif

		model = GSModelOpen(engine, "game:/mdl/fish.gsm");

		GSClientSetSkybox(GSEngineGetClient(engine), GSTrue);
		
		if((sound = GSSoundOpen(GSClientGetSoundEngine(GSEngineGetClient(engine)), "game:/df97.xm")) != NULL){
			GSSoundSetLoop(sound, 1);
			GSSoundStart(sound);
		}

		GSEngineLoop(engine);

		GSEngineDestroy(engine);
	}
}
