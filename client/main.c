#include <af_common.h>

MwWidget root, window, opengl;

static const MwRect sizes[] = {
    {0, 0, 640, 480},
    {0, 0, 800, 600},
    {0, 0, 1024, 768},
    {0, 0, 1280, 1024},
};

static void resize(MwWidget handle, void* user, void* client) {
	int ww = MwGetInteger(window, MwNwidth);
	int wh = MwGetInteger(window, MwNheight);

	MwVaApply(opengl,
		  MwNwidth, ww,
		  MwNheight, wh,
		  NULL);

	gl_resize(ww, wh);
}

static void tick(MwWidget handle, void* user, void* client) {
	gl_render();

	MwOpenGLSwapBuffer(opengl);
}

static void* gl_load(const char* name) {
	return MwOpenGLGetProcAddress(opengl, name);
}

void scene_change(int scene) {
	gl_scene = scene;

	ui_scene();
}

int main(int argc, char** argv) {
	MwSizeHints sh;
	MwRect	    rc;
	int	    i;

	MwLibraryInit();

	root = MwVaCreateWidget(NULL, "root", NULL, 0, 0, 0, 0,
				MwNwaitMS, 15,
				NULL);

	window = MwVaCreateWidget(MwWindowClass, "main", root, 0, 0, 640, 480,
				  MwNtitle, "AssaultFish",
				  NULL);

	MwGetScreenSize(window, &rc);

	for(i = sizeof(sizes) / sizeof(sizes[0]) - 1; i >= 0; i--) {
		if(sizes[i].width < rc.width && sizes[i].height < rc.height) {
			sh.min_width = sh.max_width = sizes[i].width;
			sh.min_height = sh.max_height = sizes[i].height;
			break;
		}
	}

	for(i = 1; i < argc; i++) {
		if(strcmp(argv[i], "--width") == 0 && (i + 1) < argc) {
			sh.min_width = sh.max_width = atoi(argv[i + 1]);
		} else if(strcmp(argv[i], "--height") == 0 && (i + 1) < argc) {
			sh.min_height = sh.max_height = atoi(argv[i + 1]);
		}
	}

	MwVaApply(window,
		  MwNsizeHints, &sh,
		  MwNx, (rc.width - sh.max_width) / 2,
		  MwNy, (rc.height - sh.max_height) / 2,
		  MwNwidth, sh.max_width,
		  MwNheight, sh.max_height,
		  NULL);

	opengl = MwCreateWidget(MwOpenGLClass, "opengl", window, 0, 0, sh.max_width, sh.max_height);

	MwOpenGLMakeCurrent(opengl);

	gladLoadGLLoader(gl_load);

	resize(window, NULL, NULL);

	gl_init();
	ui_init();

	scene_change(AF_SCENE_MAIN);

	MwAddUserHandler(window, MwNresizeHandler, resize, NULL);
	MwAddUserHandler(root, MwNtickHandler, tick, NULL);

	MwLoop(root);
}
