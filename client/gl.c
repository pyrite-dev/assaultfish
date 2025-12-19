#include <af_common.h>

double gl_cam_lr = 0;
double gl_cam_ud = 0;
double gl_cam_x;
double gl_cam_y;
double gl_cam_z;

int gl_scene = AF_SCENE_MAIN;

gl_scene_t gl_scenes[] = {
    {gl_main_changed, gl_main_init, gl_main_draw},		    /**/
    {gl_connecting_changed, gl_connecting_init, gl_connecting_draw} /**/
};

void gl_resize(int width, int height) {
	glViewport(0, 0, width, height);
}

void gl_init(void) {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	gl_font_init();
}

void gl_render(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gl_scenes[gl_scene].init();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (double)MwGetInteger(opengl, MwNwidth) / MwGetInteger(opengl, MwNheight), 0.01, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(gl_cam_x, gl_cam_y, gl_cam_z, gl_cam_x + cos(gl_cam_lr / 180.0 * M_PI), gl_cam_y + sin(gl_cam_ud / 180.0 * M_PI), gl_cam_z + sin(gl_cam_lr / 180.0 * M_PI), 0, 1, 0);

	gl_scenes[gl_scene].draw();
}
