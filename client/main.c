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
	GSVector3 v[] = {
		{-5, -2, 5},
		{-5, -2, -5},
		{5, -2, -5},
		{5, -2, 5}
	};
	GSVector3 n = {0, 1, 0};
	GSVector4 col = {1, 1, 1, 1};
	GSVector4 col2 = {1, 0, 0, 1};
	int i, j;

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

	for(i = -1; i <= 1; i++){
		for(j = -1; j <= 1; j++){
			GSVector3 pos2;
			double s = fabs(sin(rot[0] / 180 * GSMathPi));
			int n = (i + 1) * 3 + (j + 1);

			pos2[0] = i * s * 2;
			pos2[1] = 0;
			pos2[2] = j * s * 2;

			col2[2] = n & 1;
			col2[1] = (n & 2) ? 1 : 0;
			col2[0] = (n & 4) ? 1 : 0;

			GSGLPushMatrix(gl);
			GSGLSetPosition(gl, pos2);
			GSGLSetRotation(gl, rot);
			GSGLTetrakis(gl, s, col, col2);
			GSGLPopMatrix(gl);
		}
	}

	GSGLSetColor(gl, col);
	GSGLPolygon(gl, 4, v, NULL, n);
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
	param.server = 1;

	GSInit();

	if((engine = GSEngineCreate(&param)) != NULL){
		GSSound sound;

#ifndef _WIN32
		signal(SIGINT, shutdown_engine);
		signal(SIGTERM, shutdown_engine);
#endif

		model = GSModelOpen(engine, "game:/mdl/fish.gsm");

		GSClientSetSkybox(GSEngineGetClient(engine), GSTrue);
		
		if((sound = GSSoundOpen(GSClientGetSoundEngine(GSEngineGetClient(engine)), "game:/desert2.mod")) != NULL){
			GSSoundSetLoop(sound, 1);
			GSSoundStart(sound);
		}

		GSEngineLoop(engine);

		GSEngineDestroy(engine);
	}
}
