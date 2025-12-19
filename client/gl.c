#include <af_common.h>

double gl_cam_lr = 0;
double gl_cam_ud = 0;
double gl_cam_x;
double gl_cam_y;
double gl_cam_z;

int gl_scene = AF_SCENE_MAIN;

gl_scene_t gl_scenes[] = {
    {gl_main_changed, gl_main_init, gl_main_draw}, /**/
    {NULL, NULL, NULL}				   /**/
};

void gl_resize(int width, int height) {
	glViewport(0, 0, width, height);
}

void gl_init(void) {
	GLfloat	       lightamb[]   = {0.1, 0.1, 0.1, 1.0};
	GLfloat	       lightcol[]   = {1.0, 1.0, 1.0, 1.0};
	const GLdouble genfunc[][4] = {
	    {1.0, 0.0, 0.0, 0.0},
	    {0.0, 1.0, 0.0, 0.0},
	    {0.0, 0.0, 1.0, 0.0},
	    {0.0, 0.0, 0.0, 1.0},
	};

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_BLEND);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);

	glLightfv(GL_LIGHT0, GL_AMBIENT, lightamb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightcol);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightcol);

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	gl_font_init();
}

static void gl_common(void) {
	char buf[128];
	glColor3f(1, 1, 1);

	sprintf(buf, "AssaultFish %s", AF_VERSION);
	gl_font_text(buf, MwGetInteger(opengl, MwNwidth) - gl_font_width(buf), MwGetInteger(opengl, MwNheight) - gl_font_height(buf) * 2, 1);

	sprintf(buf, "%s", AF_COPYRIGHT);
	gl_font_text(buf, MwGetInteger(opengl, MwNwidth) - gl_font_width(buf), MwGetInteger(opengl, MwNheight) - gl_font_height(buf), 1);
}

void gl_render(void) {
	GLfloat	 lightpos[4] = {0, 5, 0, 1};
	GLdouble modelview[16];

	if(gl_scenes[gl_scene].init != NULL) gl_scenes[gl_scene].init();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (double)MwGetInteger(opengl, MwNwidth) / MwGetInteger(opengl, MwNheight), 0.01, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(gl_cam_x, gl_cam_y, gl_cam_z, gl_cam_x + cos(gl_cam_lr / 180.0 * M_PI), gl_cam_y + sin(gl_cam_ud / 180.0 * M_PI), gl_cam_z + sin(gl_cam_lr / 180.0 * M_PI), 0, 1, 0);

	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

	if(gl_scenes[gl_scene].draw != NULL) gl_scenes[gl_scene].draw();

	gl_common();
}

void gl_scene_change(void) {
	if(gl_scenes[gl_scene].changed != NULL) gl_scenes[gl_scene].changed();
}

static void cross(double* out, double v00, double v01, double v02, double v10, double v11, double v12) {
	out[0] = v01 * v12 - v02 * v11;
	out[1] = v02 * v10 - v00 * v12;
	out[2] = v00 * v11 - v01 * v10;
}

void gl_calc_normal(double* out, double v00, double v01, double v02, double v10, double v11, double v12, double v20, double v21, double v22) {
	double v[3];
	double l = 0;
	int    i;

	cross(&v[0],			       /**/
	      v10 - v00, v11 - v01, v12 - v02, /**/
	      v20 - v00, v21 - v01, v22 - v02  /**/
	);

	l = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);

	for(i = 0; i < 3; i++) {
		out[i] = v[i] / l;
	}
}
