#include <af_common.h>

#define SHADOW_WIDTH 1024
#define SHADOW_HEIGHT 1024

double gl_cam_lr = 0;
double gl_cam_ud = 0;
double gl_cam_x;
double gl_cam_y;
double gl_cam_z;

int gl_scene = AF_SCENE_MAIN;

static GLuint shadow_texture, shadow;

gl_scene_t gl_scenes[] = {
    {gl_main_changed, gl_main_init, gl_main_draw, gl_main_after}, /**/
    {NULL, NULL, NULL, NULL}					  /**/
};

static char gl_status[256];

void gl_resize(int width, int height) {
	glViewport(0, 0, width, height);
}

void gl_init(void) {
	GLfloat lightamb[] = {0.1, 0.1, 0.1, 1.0};
	GLfloat lightcol[] = {1.0, 1.0, 1.0, 1.0};

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

	gl_status[0] = 0;

	glGenTextures(1, &shadow_texture);
	glBindTexture(GL_TEXTURE_2D, shadow_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);

	glBindTexture(GL_TEXTURE_2D, 0);

	if(gl_shader(&shadow, DATAROOTDIR "/assaultfish/shadow.vs", DATAROOTDIR "/assaultfish/shadow.fs")) {
		glUseProgram(shadow);
		glUniform1i(glGetUniformLocation(shadow, "texture"), 1);
	}
}

void gl_cube(double x, double y, double z, double xs, double ys, double zs, double xr, double yr, double zr) {
	int i;
	glPushMatrix();
	glTranslatef(x, y, z);
	glScalef(xs, ys, zs);
	glRotatef(xr, 1, 0, 0);
	glRotatef(yr, 0, 1, 0);
	glRotatef(zr, 0, 0, 1);
	for(i = 0; i < 6; i++) {
		glBegin(GL_QUADS);
		glNormal3f(0, 0, 1);
		glTexCoord2f(0, 0);
		glVertex3f(-0.5, 0.5, 0.5);
		glTexCoord2f(0, 1);
		glVertex3f(-0.5, -0.5, 0.5);
		glTexCoord2f(1, 1);
		glVertex3f(0.5, -0.5, 0.5);
		glTexCoord2f(1, 0);
		glVertex3f(0.5, 0.5, 0.5);
		glEnd();

		if(i == 3) {
			glRotatef(90, 1, 0, 0);
		} else if(i == 4) {
			glRotatef(180, 1, 0, 0);
		} else {
			glRotatef(90, 0, 1, 0);
		}
	}
	glPopMatrix();
}

static void gl_common(void) {
	char buf[128];
	glColor3f(1, 1, 1);

	sprintf(buf, "AssaultFish %s", AF_VERSION);
	gl_font_text(buf, MwGetInteger(opengl, MwNwidth) - gl_font_width(buf), MwGetInteger(opengl, MwNheight) - gl_font_height(buf) * 2, 1);

	sprintf(buf, "%s", AF_COPYRIGHT);
	gl_font_text(buf, MwGetInteger(opengl, MwNwidth) - gl_font_width(buf), MwGetInteger(opengl, MwNheight) - gl_font_height(buf), 1);

	gl_font_text(gl_status, 0, 0, 1);
}

static void scene(void) {
	if(gl_scenes[gl_scene].draw != NULL) gl_scenes[gl_scene].draw();
}

void gl_render(void) {
	GLfloat	 lightpos[4] = {2.5, 5, -2.5, 1};
	GLint	 viewport[4];
	GLdouble modelview[16];
	GLdouble modelview2[16];

	if(gl_scenes[gl_scene].init != NULL) gl_scenes[gl_scene].init();

	glClear(GL_DEPTH_BUFFER_BIT);

	glGetIntegerv(GL_VIEWPORT, &viewport[0]);

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluPerspective(60, (double)SHADOW_WIDTH / SHADOW_HEIGHT, 0.01, 100.0);
	gluLookAt(lightpos[0], lightpos[1], lightpos[2], 0, 0, 0, 0, 1, 0);

	glGetDoublev(GL_MODELVIEW_MATRIX, &modelview[0]);

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	glDisable(GL_LIGHTING);

	glCullFace(GL_FRONT);

	scene();

	glBindTexture(GL_TEXTURE_2D, shadow_texture);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindTexture(GL_TEXTURE_2D, 0);

	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glEnable(GL_LIGHTING);
	glCullFace(GL_BACK);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (double)MwGetInteger(opengl, MwNwidth) / MwGetInteger(opengl, MwNheight), 0.01, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(gl_cam_x, gl_cam_y, gl_cam_z, gl_cam_x + cos(gl_cam_lr / 180.0 * M_PI), gl_cam_y + sin(gl_cam_ud / 180.0 * M_PI), gl_cam_z + sin(gl_cam_lr / 180.0 * M_PI), 0, 1, 0);
	glGetDoublev(GL_MODELVIEW_MATRIX, &modelview2[0]);

	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();

	glTranslated(0.5, 0.5, 0.5);
	glScaled(0.5, 0.5, 0.5);
	glMultMatrixd(modelview);
	glMultMatrixd(modelview2);

	glMatrixMode(GL_MODELVIEW);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, shadow_texture);
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

	scene();

	glActiveTexture(GL_TEXTURE1);
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);

	glUseProgram(0);

	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);

	gl_common();

	glUseProgram(shadow);

	if(gl_scenes[gl_scene].after != NULL) gl_scenes[gl_scene].after();
}

void gl_scene_change(void) {
	if(gl_scenes[gl_scene].changed != NULL) gl_scenes[gl_scene].changed();

	gl_status[0] = 0;
}

void gl_set_status(const char* status) {
	strcpy(gl_status, status);
}

static void cross(double* out, double v00, double v01, double v02, double v10, double v11, double v12) {
	out[0] = v01 * v12 - v02 * v11;
	out[1] = v02 * v10 - v00 * v12;
	out[2] = v00 * v11 - v01 * v10;
}

/*
 * v0-v2
 *  | /
 * v1/
 */
void gl_calc_normal3(double* out, double v00, double v01, double v02, double v10, double v11, double v12, double v20, double v21, double v22) {
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

/*
 * v0-v3
 *  | |
 * v1-v2
 */
void gl_calc_normal4(double* out, double v00, double v01, double v02, double v10, double v11, double v12, double v20, double v21, double v22, double v30, double v31, double v32) {
	double v0[3];
	double v1[3];
	int    i;
	double l = 0;

	gl_calc_normal3(&v0[0], v00, v01, v02, v10, v11, v12, v30, v31, v32);
	gl_calc_normal3(&v1[0], v10, v11, v12, v20, v21, v22, v30, v31, v32);

	for(i = 0; i < 3; i++) {
		out[i] = v0[i] + v1[i];
	}

	l = sqrt(out[0] * out[0] + out[1] * out[1] + out[2] * out[2]);

	for(i = 0; i < 3; i++) {
		out[i] = out[i] / l;
	}
}
