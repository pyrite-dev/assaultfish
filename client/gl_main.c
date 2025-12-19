#include <af_common.h>

void gl_main_init(void) {
	gl_cam_x = 0;
	gl_cam_y = 0;
	gl_cam_z = -1;

	gl_cam_lr = 90;
	gl_cam_ud = 0;
}

void gl_main_draw(void) {
	int	      i;
	static double r = 0;

	glDisable(GL_DEPTH_TEST);
	glPushMatrix();
	glRotatef(r, 1, 0, 0);
	glRotatef(r, 0, 1, 0);
	glRotatef(r, 0, 0, 1);
	glBegin(GL_TRIANGLES);
	for(i = 0; i < 8; i++) {
		double r = i * 90 / 180.0 * M_PI;
		int    c = ((i + (i / 4)) % 2) == 0 ? 1 : 0;
		glColor4f(c ? 0.75 : 0, 0, c ? 0 : 0.75, 0.5);
		glVertex3f(0, (i > 3 ? -1 : 1) * 0.5, 0);
		glVertex3f(cos(r) / 2, 0, sin(r) / 2);
		glVertex3f(cos(r + 90 / 180.0 * M_PI) / 2, 0, sin(r + 90 / 180.0 * M_PI) / 2);
	}
	glEnd();
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);

	r += 60.0 / (1000.0 / AF_WAIT_MS);
}
