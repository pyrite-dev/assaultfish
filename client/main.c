#include <af_common.h>

MwWidget root, window, opengl;

static void resize(MwWidget handle, void* user, void* client){
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

	MwAddUserHandler(window, MwNresizeHandler, resize, NULL);

	MwLoop(root);
}
