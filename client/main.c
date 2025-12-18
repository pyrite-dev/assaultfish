#include <af_common.h>

MwWidget root, window, opengl;

static void resize(MwWidget handle, void* user, void* client){
	int ww = MwGetInteger(window, MwNwidth);
	int wh = MwGetInteger(window, MwNheight);

	MwVaApply(opengl,
		MwNwidth, ww,
		MwNheight, wh,
	NULL);

	glViewport(0, 0, ww, wh);
}

static void tick(MwWidget handle, void* user, void* client){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	MwOpenGLSwapBuffer(opengl);
}

static void* gl_load(const char* name){
	return MwOpenGLGetProcAddress(opengl, name);
}

int main(){
	MwLibraryInit();

	root = MwVaCreateWidget(NULL, "root", NULL, 0, 0, 0, 0,
		MwNwaitMS, 15,
	NULL);

	window = MwVaCreateWidget(MwWindowClass, "main", root, MwDEFAULT, MwDEFAULT, 800, 600,
		MwNtitle, "AssaultFish",
	NULL);

	opengl = MwCreateWidget(MwOpenGLClass, "opengl", window, 0, 0, 800, 600);

	MwOpenGLMakeCurrent(opengl);

	gladLoadGLLoader(gl_load);

	resize(window, NULL, NULL);

	MwAddUserHandler(window, MwNresizeHandler, resize, NULL);
	MwAddUserHandler(root, MwNtickHandler, tick, NULL);

	MwLoop(root);
}
