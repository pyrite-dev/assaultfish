#include <GearSrc/GL.h>

#include <GearSrc/Math.h>

void GSGLCameraPerspective(GSGL gl, GSNumber width, GSNumber height) {
	GSNumber aspect;
	GSNumber f;
	GLdouble matrix[16];
	int	 i;
	GSNumber fovy  = 60.0;
	GSNumber znear = 0.01;
	GSNumber zfar  = GSGLMaxDistance;

	aspect = width / height;
	f      = GSMathCot(fovy / 180 * GSMathPi / 2);

	for(i = 0; i < 16; i++) matrix[i] = 0;
	matrix[4 * 0 + 0] = f / aspect;
	matrix[4 * 1 + 1] = f;
	matrix[4 * 2 + 2] = (zfar + znear) / (znear - zfar);
	matrix[4 * 3 + 2] = ((GLdouble)2 * zfar * znear) / (znear - zfar);
	matrix[4 * 2 + 3] = -1;

	glMultMatrixd(matrix);
}

void GSGLCameraSet(GSGL gl) {
	GSGLCameraLookAt(gl, gl->engine->client->position, gl->engine->client->look_at);
}

void GSGLCameraLookAt(GSGL gl, GSVector3 camera, GSVector3 look_at) {
	GLdouble  matrix[16];
	GSVector3 f;
	GSVector3 up;
	GSVector3 s;
	GSVector3 u;
	int	  i;

	GSMathSubtract3(f, look_at, camera);
	GSMathNormalize3(f);

	up[0] = 0;
	up[1] = 1;
	up[2] = 0;
	GSMathNormalize3(up);

	GSMathCross3(s, f, up);
	GSMathNormalize3(s);

	GSMathCross3(u, s, f);

	for(i = 0; i < 16; i++) matrix[i] = 0;
	matrix[4 * 0 + 0] = s[0];
	matrix[4 * 1 + 0] = s[1];
	matrix[4 * 2 + 0] = -s[2];
	matrix[4 * 0 + 1] = u[0];
	matrix[4 * 1 + 1] = u[1];
	matrix[4 * 2 + 1] = -u[2];
	matrix[4 * 0 + 2] = -f[0];
	matrix[4 * 1 + 2] = -f[1];
	matrix[4 * 2 + 2] = f[2];
	matrix[4 * 3 + 3] = 1;

	glMultMatrixd(matrix);
	glTranslated(-camera[0], -camera[1], camera[2]);

	if(gl->shadow_use_shader){
		glUseProgram(gl->shadow_shader);
		glUniform3f(glGetUniformLocation(gl->shadow_shader, "camera"), camera[0], camera[1], camera[2]);
		glUseProgram(0);
	}
}

void GSGLCameraSetup(GSGL gl) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GSGLCameraPerspective(gl, gl->engine->width, gl->engine->height);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	GSGLCameraSet(gl);
}
