#include <GearBox/GL.h>

#include <GearBox/Math.h>

void GBGLCameraPerspective(GBGL gl, GBNumber width, GBNumber height) {
	GBNumber aspect;
	GBNumber f;
	GLdouble matrix[16];
	int	 i;
	GBNumber fovy  = 60.0;
	GBNumber znear = 0.01;
	GBNumber zfar  = GBGLMaxDistance;

	aspect = width / height;
	f      = GBMathCot(fovy / 180 * GBMathPi / 2);

	for(i = 0; i < 16; i++) matrix[i] = 0;
	matrix[4 * 0 + 0] = f / aspect;
	matrix[4 * 1 + 1] = f;
	matrix[4 * 2 + 2] = (zfar + znear) / (znear - zfar);
	matrix[4 * 3 + 2] = ((GLdouble)2 * zfar * znear) / (znear - zfar);
	matrix[4 * 2 + 3] = -1;

	glMultMatrixd(matrix);
}

void GBGLCameraSet(GBGL gl) {
	GBGLCameraLookAt(gl, gl->engine->client->camera, gl->engine->client->look_at);
}

void GBGLCameraLookAt(GBGL gl, GBVector3 camera, GBVector3 look_at) {
	GLdouble  matrix[16];
	GBVector3 f;
	GBVector3 up;
	GBVector3 s;
	GBVector3 u;
	int	  i;

	GBMathSubtract3(f, look_at, camera);
	GBMathNormalize3(f);

	up[0] = 0;
	up[1] = 1;
	up[2] = 0;
	GBMathNormalize3(up);

	GBMathCross3(s, f, up);
	GBMathNormalize3(s);

	GBMathCross3(u, s, f);

	for(i = 0; i < 16; i++) matrix[i] = 0;
	matrix[4 * 0 + 0] = s[0];
	matrix[4 * 1 + 0] = s[1];
	matrix[4 * 2 + 0] = s[2];
	matrix[4 * 0 + 1] = u[0];
	matrix[4 * 1 + 1] = u[1];
	matrix[4 * 2 + 1] = u[2];
	matrix[4 * 0 + 2] = -f[0];
	matrix[4 * 1 + 2] = -f[1];
	matrix[4 * 2 + 2] = -f[2];
	matrix[4 * 3 + 3] = 1;

	glMultMatrixd(matrix);
	glTranslated(-camera[0], -camera[1], -camera[2]);
}

void GBGLCameraSetup(GBGL gl) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GBGLCameraPerspective(gl, gl->engine->width, gl->engine->height);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	GBGLCameraSet(gl);
}
