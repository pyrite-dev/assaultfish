#include <af_common.h>

static double rad;

void gl_main_changed(void) {
	rad = 0;
}

void gl_main_init(void) {
	gl_cam_x = 0;
	gl_cam_y = 0;
	gl_cam_z = -1;

	gl_cam_lr = 90;
	gl_cam_ud = 0;
}

void gl_main_draw(void) {
	int i;

	glPushMatrix();
	glRotatef(rad, 1, 0, 0);
	glRotatef(rad, 0, 1, 0);
	glRotatef(rad, 0, 0, 1);
	for(i = 0; i < 8; i++) {
		int    c = (i % 2) == 0 ? 1 : 0;
		double n[3];
		double u = 0.3;

		gl_calc_normal(&n[0], 0, u * 1.4, 0, -u, 0, u, u, 0, u);

		glColor3f(c ? 0.75 : 0, 0, c ? 0 : 0.75);
		glBegin(GL_TRIANGLES);
		glNormal3f(n[0], n[1], n[2]);
		glVertex3f(0, u * 1.4, 0);
		glVertex3f(-u, 0, u);
		glVertex3f(u, 0, u);
		glEnd();
		glRotatef(90, 0, 1, 0);
		if(i == 3) {
			glRotatef(180, 0, 0, 1);
			glRotatef(90, 0, 1, 0);
		}
	}

	glPopMatrix();

	rad += 60.0 / (1000.0 / AF_WAIT_MS);
}
