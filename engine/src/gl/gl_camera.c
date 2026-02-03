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

	if(gl->shadow_use_shader) {
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

	GSGLCameraFrustum(gl);
}

static void frustum_normalize(GSGL gl, int plane) {
	GSVector3 v;
	GSNumber  l;
	int	  i;

	v[0] = gl->frustum_planes[plane][0];
	v[1] = gl->frustum_planes[plane][1];
	v[2] = gl->frustum_planes[plane][2];

	l = GSMathLength3(v);

	for(i = 0; i < 4; i++) {
		gl->frustum_planes[plane][i] /= l;
	}
}

void GSGLCameraFrustum(GSGL gl) {
	glGetDoublev(GL_PROJECTION_MATRIX, gl->frustum_projection);
	glGetDoublev(GL_MODELVIEW_MATRIX, gl->frustum_modelview);

	gl->frustum_clip[0x0]	 = gl->frustum_modelview[0x0] * gl->frustum_projection[0x0] + gl->frustum_modelview[0x1] * gl->frustum_projection[0x4] + gl->frustum_modelview[0x2] * gl->frustum_projection[0x8] + gl->frustum_modelview[0x3] * gl->frustum_projection[0xc];
	gl->frustum_clip[0x1]	 = gl->frustum_modelview[0x0] * gl->frustum_projection[0x1] + gl->frustum_modelview[0x1] * gl->frustum_projection[0x5] + gl->frustum_modelview[0x2] * gl->frustum_projection[0x9] + gl->frustum_modelview[0x3] * gl->frustum_projection[0xd];
	gl->frustum_clip[0x2]	 = gl->frustum_modelview[0x0] * gl->frustum_projection[0x2] + gl->frustum_modelview[0x1] * gl->frustum_projection[0x6] + gl->frustum_modelview[0x2] * gl->frustum_projection[0xa] + gl->frustum_modelview[0x3] * gl->frustum_projection[0xe];
	gl->frustum_clip[0x3]	 = gl->frustum_modelview[0x0] * gl->frustum_projection[0x3] + gl->frustum_modelview[0x1] * gl->frustum_projection[0x7] + gl->frustum_modelview[0x2] * gl->frustum_projection[0xb] + gl->frustum_modelview[0x3] * gl->frustum_projection[0xf];
	gl->frustum_clip[0x4]	 = gl->frustum_modelview[0x4] * gl->frustum_projection[0x0] + gl->frustum_modelview[0x5] * gl->frustum_projection[0x4] + gl->frustum_modelview[0x6] * gl->frustum_projection[0x8] + gl->frustum_modelview[0x7] * gl->frustum_projection[0xc];
	gl->frustum_clip[0x5]	 = gl->frustum_modelview[0x4] * gl->frustum_projection[0x1] + gl->frustum_modelview[0x5] * gl->frustum_projection[0x5] + gl->frustum_modelview[0x6] * gl->frustum_projection[0x9] + gl->frustum_modelview[0x7] * gl->frustum_projection[0xd];
	gl->frustum_clip[0x6]	 = gl->frustum_modelview[0x4] * gl->frustum_projection[0x2] + gl->frustum_modelview[0x5] * gl->frustum_projection[0x6] + gl->frustum_modelview[0x6] * gl->frustum_projection[0xa] + gl->frustum_modelview[0x7] * gl->frustum_projection[0xe];
	gl->frustum_clip[0x7]	 = gl->frustum_modelview[0x4] * gl->frustum_projection[0x3] + gl->frustum_modelview[0x5] * gl->frustum_projection[0x7] + gl->frustum_modelview[0x6] * gl->frustum_projection[0xb] + gl->frustum_modelview[0x7] * gl->frustum_projection[0xf];
	gl->frustum_clip[0x8]	 = gl->frustum_modelview[0x8] * gl->frustum_projection[0x0] + gl->frustum_modelview[0x9] * gl->frustum_projection[0x4] + gl->frustum_modelview[0xa] * gl->frustum_projection[0x8] + gl->frustum_modelview[0xb] * gl->frustum_projection[0xc];
	gl->frustum_clip[0x9]	 = gl->frustum_modelview[0x8] * gl->frustum_projection[0x1] + gl->frustum_modelview[0x9] * gl->frustum_projection[0x5] + gl->frustum_modelview[0xa] * gl->frustum_projection[0x9] + gl->frustum_modelview[0xb] * gl->frustum_projection[0xd];
	gl->frustum_clip[0xa]	 = gl->frustum_modelview[0x8] * gl->frustum_projection[0x2] + gl->frustum_modelview[0x9] * gl->frustum_projection[0x6] + gl->frustum_modelview[0xa] * gl->frustum_projection[0xa] + gl->frustum_modelview[0xb] * gl->frustum_projection[0xe];
	gl->frustum_clip[0xb]	 = gl->frustum_modelview[0x8] * gl->frustum_projection[0x3] + gl->frustum_modelview[0x9] * gl->frustum_projection[0x7] + gl->frustum_modelview[0xa] * gl->frustum_projection[0xb] + gl->frustum_modelview[0xb] * gl->frustum_projection[0xf];
	gl->frustum_clip[0xc]	 = gl->frustum_modelview[0xc] * gl->frustum_projection[0x0] + gl->frustum_modelview[0xd] * gl->frustum_projection[0x4] + gl->frustum_modelview[0xe] * gl->frustum_projection[0x8] + gl->frustum_modelview[0xf] * gl->frustum_projection[0xc];
	gl->frustum_clip[0xd]	 = gl->frustum_modelview[0xc] * gl->frustum_projection[0x1] + gl->frustum_modelview[0xd] * gl->frustum_projection[0x5] + gl->frustum_modelview[0xe] * gl->frustum_projection[0x9] + gl->frustum_modelview[0xf] * gl->frustum_projection[0xd];
	gl->frustum_clip[0xe]	 = gl->frustum_modelview[0xc] * gl->frustum_projection[0x2] + gl->frustum_modelview[0xd] * gl->frustum_projection[0x6] + gl->frustum_modelview[0xe] * gl->frustum_projection[0xa] + gl->frustum_modelview[0xf] * gl->frustum_projection[0xe];
	gl->frustum_clip[0xf]	 = gl->frustum_modelview[0xc] * gl->frustum_projection[0x3] + gl->frustum_modelview[0xd] * gl->frustum_projection[0x7] + gl->frustum_modelview[0xe] * gl->frustum_projection[0xb] + gl->frustum_modelview[0xf] * gl->frustum_projection[0xf];
	gl->frustum_planes[0][0] = gl->frustum_clip[0x3] - gl->frustum_clip[0x0];
	gl->frustum_planes[0][1] = gl->frustum_clip[0x7] - gl->frustum_clip[0x4];
	gl->frustum_planes[0][2] = gl->frustum_clip[0xb] - gl->frustum_clip[0x8];
	gl->frustum_planes[0][3] = gl->frustum_clip[0xf] - gl->frustum_clip[0xc];
	frustum_normalize(gl, 0);
	gl->frustum_planes[1][0] = gl->frustum_clip[0x3] + gl->frustum_clip[0x0];
	gl->frustum_planes[1][1] = gl->frustum_clip[0x7] + gl->frustum_clip[0x4];
	gl->frustum_planes[1][2] = gl->frustum_clip[0xb] + gl->frustum_clip[0x8];
	gl->frustum_planes[1][3] = gl->frustum_clip[0xf] + gl->frustum_clip[0xc];
	frustum_normalize(gl, 1);
	gl->frustum_planes[2][0] = gl->frustum_clip[0x3] - gl->frustum_clip[0x1];
	gl->frustum_planes[2][1] = gl->frustum_clip[0x7] - gl->frustum_clip[0x5];
	gl->frustum_planes[2][2] = gl->frustum_clip[0xb] - gl->frustum_clip[0x9];
	gl->frustum_planes[2][3] = gl->frustum_clip[0xf] - gl->frustum_clip[0xd];
	frustum_normalize(gl, 2);
	gl->frustum_planes[3][0] = gl->frustum_clip[0x3] + gl->frustum_clip[0x1];
	gl->frustum_planes[3][1] = gl->frustum_clip[0x7] + gl->frustum_clip[0x5];
	gl->frustum_planes[3][2] = gl->frustum_clip[0xb] + gl->frustum_clip[0x9];
	gl->frustum_planes[3][3] = gl->frustum_clip[0xf] + gl->frustum_clip[0xd];
	frustum_normalize(gl, 3);
	gl->frustum_planes[4][0] = gl->frustum_clip[0x3] - gl->frustum_clip[0x2];
	gl->frustum_planes[4][1] = gl->frustum_clip[0x7] - gl->frustum_clip[0x6];
	gl->frustum_planes[4][2] = gl->frustum_clip[0xb] - gl->frustum_clip[0xa];
	gl->frustum_planes[4][3] = gl->frustum_clip[0xf] - gl->frustum_clip[0xe];
	frustum_normalize(gl, 4);
	gl->frustum_planes[5][0] = gl->frustum_clip[0x3] + gl->frustum_clip[0x2];
	gl->frustum_planes[5][1] = gl->frustum_clip[0x7] + gl->frustum_clip[0x6];
	gl->frustum_planes[5][2] = gl->frustum_clip[0xb] + gl->frustum_clip[0xa];
	gl->frustum_planes[5][3] = gl->frustum_clip[0xf] + gl->frustum_clip[0xe];
	frustum_normalize(gl, 5);
}

GSBool GSGLCameraFrustumContain(GSGL gl, GSVector3 a, GSVector3 b) {
	int i;
	for(i = 0; i < 6; i++) {
		GSBool bv = GSTrue;

		bv = bv && gl->frustum_planes[i][0] * a[0] + gl->frustum_planes[i][1] * a[1] + gl->frustum_planes[i][2] * a[2] + gl->frustum_planes[i][3] <= 0.0;
		bv = bv && gl->frustum_planes[i][0] * b[0] + gl->frustum_planes[i][1] * a[1] + gl->frustum_planes[i][2] * a[2] + gl->frustum_planes[i][3] <= 0.0;
		bv = bv && gl->frustum_planes[i][0] * a[0] + gl->frustum_planes[i][1] * b[1] + gl->frustum_planes[i][2] * a[2] + gl->frustum_planes[i][3] <= 0.0;
		bv = bv && gl->frustum_planes[i][0] * b[0] + gl->frustum_planes[i][1] * b[1] + gl->frustum_planes[i][2] * a[2] + gl->frustum_planes[i][3] <= 0.0;
		bv = bv && gl->frustum_planes[i][0] * a[0] + gl->frustum_planes[i][1] * a[1] + gl->frustum_planes[i][2] * b[2] + gl->frustum_planes[i][3] <= 0.0;
		bv = bv && gl->frustum_planes[i][0] * b[0] + gl->frustum_planes[i][1] * a[1] + gl->frustum_planes[i][2] * b[2] + gl->frustum_planes[i][3] <= 0.0;
		bv = bv && gl->frustum_planes[i][0] * a[0] + gl->frustum_planes[i][1] * b[1] + gl->frustum_planes[i][2] * b[2] + gl->frustum_planes[i][3] <= 0.0;
		bv = bv && gl->frustum_planes[i][0] * b[0] + gl->frustum_planes[i][1] * b[1] + gl->frustum_planes[i][2] * b[2] + gl->frustum_planes[i][3] <= 0.0;
		if(bv) return GSFalse;
	}

	return GSTrue;
}
