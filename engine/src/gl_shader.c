#include <GearSrc/GL.h>

#include <GearSrc/File.h>
#include <GearSrc/Log.h>

static void err(const char* path, GLuint shader) {
	GLint len, ret;
	char* e;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

	e      = malloc(len);
	e[len] = 0;

	glGetShaderInfoLog(shader, len, &ret, e);

	GSLog(GSLogError, "%s: %s\n", path, e);

	free(e);
}

GSBool GSGLShaderPrepare(GSGL gl, GLuint* shader, const char* vs, const char* fs) {
	GLuint vsi;
	GLuint fsi;
	GSFile f;
	int    vsl = 0;
	int    fsl = 0;
	char*  vss;
	char*  fss;
	GLint  st;

	if(glUseProgram == NULL) return GSFalse;

	if((f = GSFileOpen(gl->engine, vs)) != NULL) {
		int sz = GSFileSize(f);

		vss = malloc(sz + 1);
		GSFileRead(f, vss, sz);
		vss[sz] = 0;

		GSFileClose(f);
	} else {
		return GSFalse;
	}

	if((f = GSFileOpen(gl->engine, fs)) != NULL) {
		int sz = GSFileSize(f);

		fss = malloc(sz + 1);
		GSFileRead(f, fss, sz);
		fss[sz] = 0;

		GSFileClose(f);
	} else {
		free(vss);

		return GSFalse;
	}

	vsi = glCreateShader(GL_VERTEX_SHADER);
	fsi = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vsi, 1, (const GLchar**)&vss, NULL);
	glShaderSource(fsi, 1, (const GLchar**)&fss, NULL);

	glCompileShader(vsi);
	glGetShaderiv(vsi, GL_COMPILE_STATUS, &st);
	if(!st) {
		err(vs, vsi);

		glDeleteShader(vsi);
		glDeleteShader(fsi);
		free(vss);
		free(fss);
		return GSFalse;
	}

	glCompileShader(fsi);
	glGetShaderiv(fsi, GL_COMPILE_STATUS, &st);
	if(!st) {
		err(fs, fsi);

		glDeleteShader(vsi);
		glDeleteShader(fsi);
		free(vss);
		free(fss);
		return GSFalse;
	}

	free(vss);
	free(fss);

	*shader = glCreateProgram();

	glAttachShader(*shader, vsi);
	glAttachShader(*shader, fsi);

	glDeleteShader(vsi);
	glDeleteShader(fsi);

	glLinkProgram(*shader);
	glGetProgramiv(*shader, GL_LINK_STATUS, &st);
	if(!st) {
		glDeleteProgram(*shader);
		return GSFalse;
	}

	return GSTrue;
}
