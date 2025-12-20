#include <af_common.h>

void af_common_version(unsigned char* v_major, unsigned char* v_minor, unsigned char* v_patch) {
	char* mj = AF_VERSION;
	char* mi = strchr(mj, '.') + 1;
	char* pt = strchr(mi, '.') + 1;

	*v_major = atoi(mj);
	*v_minor = atoi(mi);
	*v_patch = atoi(pt);
}
