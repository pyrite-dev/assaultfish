#include <GearBox/String.h>

char* GBStringDuplicate(const char* str) {
	char* s = malloc(strlen(str) + 1);

	strcpy(s, str);

	return s;
}

char* GBStringConcat(const char* str1, const char* str2) {
	char* r = malloc(strlen(str1) + strlen(str2) + 1);

	strcpy(r, str1);
	strcat(r, str2);

	return r;
}
