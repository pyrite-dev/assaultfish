#include <af_common.h>

int gl_shader(GLuint* r, const char* vs, const char* fs) {
	GLuint vsi;
	GLuint fsi;
	FILE*  f;
	int    vsl = 0;
	int    fsl = 0;
	char*  vss;
	char*  fss;
	GLint  st;

	if((f = fopen(vs, "rb")) != NULL) {
		fseek(f, 0, SEEK_END);
		vsl = ftell(f);
		fseek(f, 0, SEEK_SET);

		vss = malloc(vsl + 1);
		fread(vss, vsl, 1, f);
		vss[vsl] = 0;

		fclose(f);
	} else {
		return 0;
	}

	if((f = fopen(fs, "rb")) != NULL) {
		fseek(f, 0, SEEK_END);
		fsl = ftell(f);
		fseek(f, 0, SEEK_SET);

		fss = malloc(fsl + 1);
		fread(fss, fsl, 1, f);
		fss[fsl] = 0;

		fclose(f);
	} else {
		free(vss);

		return 0;
	}

	vsi = glCreateShader(GL_VERTEX_SHADER);
	fsi = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vsi, 1, (const GLchar**)&vss, NULL);
	glShaderSource(fsi, 1, (const GLchar**)&fss, NULL);

	glCompileShader(vsi);
	glGetShaderiv(vsi, GL_COMPILE_STATUS, &st);
	if(!st) {
		glDeleteShader(vsi);
		glDeleteShader(fsi);
		free(vss);
		free(fss);
		return 0;
	}

	glCompileShader(fsi);
	glGetShaderiv(fsi, GL_COMPILE_STATUS, &st);
	if(!st) {
		glDeleteShader(vsi);
		glDeleteShader(fsi);
		free(vss);
		free(fss);
		return 0;
	}

	free(vss);
	free(fss);

	*r = glCreateProgram();

	glAttachShader(*r, vsi);
	glAttachShader(*r, fsi);

	glDeleteShader(vsi);
	glDeleteShader(fsi);

	glLinkProgram(*r);
	glGetProgramiv(*r, GL_LINK_STATUS, &st);
	if(!st) {
		glDeleteProgram(*r);
		return 0;
	}

	return 1;
}
