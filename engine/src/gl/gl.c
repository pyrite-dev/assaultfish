#include <GearSrc/GL.h>

#include <GearSrc/Math.h>

GSGL GSGLCreate(GSClient client) {
	GLfloat lightamb[] = {0.5, 0.5, 0.5, 1.0};
	GLfloat lightcol[] = {1.0, 1.0, 1.0, 1.0};
	GSGL	gl;

	gl = malloc(sizeof(*client->gl));

	gl->engine = client->engine;

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

	GSGLShadowInit(gl);

	return gl;
}

void GSGLDestroy(GSGL gl) {
	GSGLShadowDeinit(gl);

	free(gl);
}

void GSGLClear(GSGL gl) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GSGLIgnoreDepth(GSGL gl) {
	glDisable(GL_DEPTH_TEST);
}

void GSGLCareDepth(GSGL gl) {
	glEnable(GL_DEPTH_TEST);
}

void GSGLClearDepth(GSGL gl) {
	glClear(GL_DEPTH_BUFFER_BIT);
}

void GSGLSetLight(GSGL gl) {
	GLfloat f[4];
	int	i;

	for(i = 0; i < 4; i++) f[i] = gl->engine->client->light0[i];

	glLightfv(GL_LIGHT0, GL_POSITION, f);
}

void GSGLPolygon(GSGL gl, int pairs, GSVector3* vert, GSVector2* tex, GSVector3 norm) {
	int i;

	glBegin(GL_POLYGON);
	glNormal3f(norm[0], norm[1], norm[2]);
	for(i = 0; i < pairs; i++) {
		if(tex != NULL) glTexCoord2f(tex[i][0], tex[i][1]);
		glVertex3f(vert[i][0], vert[i][1], vert[i][2]);
	}
	glEnd();
}

void GSGLPolygon2D(GSGL gl, int pairs, GSVector2* vert, GSVector2* tex) {
	int i;

	glBegin(GL_POLYGON);
	for(i = 0; i < pairs; i++) {
		if(tex != NULL) glTexCoord2f(tex[i][0], tex[i][1]);
		glVertex2f(vert[i][0], vert[i][1]);
	}
	glEnd();
}

void GSGLBegin2D(GSGL gl) {
	GSGLIgnoreDepth(gl);
	GSGLShadowDisable(gl);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, gl->engine->width, gl->engine->height, 0, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}

void GSGLEnd2D(GSGL gl) {
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);

	GSGLShadowEnable(gl);
	GSGLCareDepth(gl);
}

void GSGLSetPosition(GSGL gl, GSVector3 pos) {
	glTranslatef(pos[0], pos[1], -pos[2]);
}

void GSGLSetRotation(GSGL gl, GSVector3 rot) {
	glRotatef(rot[0], 1, 0, 0);
	glRotatef(rot[1], 0, 1, 0);
	glRotatef(rot[2], 0, 0, 1);
}

void GSGLSetRotation3x3(GSGL gl, GSMatrix3x3 rot) {
	GSMatrix4x4 out, out2;
	GLdouble    mat[16];
	int	    i;

	GSMath3x3To4x4(out, rot);
	GSMathRowToColumn4x4(out2, out);
	for(i = 0; i < 16; i++) mat[i] = out2[i];
	glMultMatrixd(mat);
}

void GSGLPushMatrix(GSGL gl) {
	glPushMatrix();
}

void GSGLPopMatrix(GSGL gl) {
	glPopMatrix();
}

GLuint GSGLBeginList(GSGL gl) {
	GLuint list;

	list = glGenLists(1);
	glNewList(list, GL_COMPILE);

	return list;
}

void GSGLEndList(GSGL gl) {
	glEndList();
}

void GSGLCallList(GSGL gl, GLuint list) {
	glCallList(list);
}

void GSGLSetColor(GSGL gl, GSVector4 color) {
	glColor4f(color[0], color[1], color[2], color[3]);
}

void GSGLInitialTranslation(GSGL gl) {
	GLdouble matrix[16];

	glGetDoublev(GL_MODELVIEW_MATRIX, matrix);

	matrix[4 * 3 + 0] = 0;
	matrix[4 * 3 + 1] = 0;
	matrix[4 * 3 + 2] = 0;

	glLoadMatrixd(matrix);
}
