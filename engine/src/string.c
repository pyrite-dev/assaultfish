#include <GearBox/String.h>

char* GBStringDuplicate(const char* str) {
	char* s = malloc(strlen(str) + 1);

	strcpy(s, str);

	return s;
}
