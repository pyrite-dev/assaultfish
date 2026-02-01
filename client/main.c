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

GSPhysicsObject obj[128];

static void render(GSEngine self){
	GSGL gl = GSClientGetGL(GSEngineGetClient(self));
	int i;
	GSVector4 col1 = {1, 0, 0, 1};
	GSVector4 col2 = {1, 1, 1, 1};
	GSVector3 v[] = {
		{-10, -1, 10},
		{-10, -1, -10},
		{10, -1, -10},
		{10, -1, 10}
	};
	GSVector3 pos = {0, 0, 0};
	GSVector3 n = {0, 1, 0};

	for(i = 0; i < sizeof(obj) / sizeof(obj[0]); i++){
		GSVector3 rv;
		GSMatrix3x3 rm;

		GSPhysicsGetPosition(obj[i], rv);
		GSPhysicsGetRotation(obj[i], rm);

		GSGLPushMatrix(gl);
		GSGLSetPosition(gl, rv);
		GSGLSetRotation3x3(gl, rm);
		//GSGLTetrakis(gl, 0.5, col1, col2);
		GSGLPopMatrix(gl);
	}

	GSGLPushMatrix(gl);
	GSGLSetPosition(gl, pos);
	GSGLTetrakis(gl, 0.5, col1, col2);
	GSGLPopMatrix(gl);

	GSGLSetColor(gl, col2);
	GSGLPolygon(gl, 4, v, NULL, n);
}

static void after_render(GSEngine self){
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
#ifndef _WIN32
		signal(SIGINT, shutdown_engine);
		signal(SIGTERM, shutdown_engine);
#endif

		int i;
		GSVector3 pos = {0, 1, 0};
		GSVector3 sz = {1, 1, 1};
		for(i = 0; i < sizeof(obj) / sizeof(obj[0]); i++){
			obj[i] = GSPhysicsCreateSphere(GSServerGetPhysics(GSEngineGetServer(engine)), 1, 0.5);

			pos[0] = ((GSNumber)rand() / RAND_MAX - 0.5) * 20;
			pos[1] = 10;
			pos[2] = ((GSNumber)rand() / RAND_MAX - 0.5) * 20;

			GSPhysicsSetPosition(obj[i], pos);
		}

		GSClientSetSkybox(GSEngineGetClient(engine), GSTrue);
		GSEngineLoop(engine);
		GSEngineDestroy(engine);
	}
}
