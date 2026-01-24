#include <GearSrc/File.h>

#define GETLINE_MINSIZE 16

int GSFileGetLine(GSFile file, char** lineptr, int* n) {
	char  ch;
	int   i		  = 0;
	char  free_on_err = 0;
	char* p;

	if(lineptr == NULL || n == NULL || file == NULL) {
		return -1;
	}
	if(*lineptr == NULL) {
		*n	 = GETLINE_MINSIZE;
		*lineptr = (char*)malloc(sizeof(char) * (*n));
		if(*lineptr == NULL) {
			return -1;
		}
		free_on_err = 1;
	}

	for(i = 0;; i++) {
		int r = GSFileRead(file, &ch, 1);
		while(i >= (*n) - 2) {
			*n *= 2;
			p = realloc(*lineptr, sizeof(char) * (*n));
			if(p == NULL) {
				if(free_on_err)
					free(*lineptr);
				return -1;
			}
			*lineptr = p;
		}
		if(r < 1) {
			if(i == 0) {
				if(free_on_err)
					free(*lineptr);
				return -1;
			}
			(*lineptr)[i] = '\0';
			*n	      = i;
			return i;
		}

		if(ch == '\n') {
			(*lineptr)[i]	  = '\n';
			(*lineptr)[i + 1] = '\0';
			*n		  = i + 1;
			return i + 1;
		}
		(*lineptr)[i] = (char)ch;
	}
}
