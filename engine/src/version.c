#include <GearBox/Version.h>

void GBVersionGet(GBVersion* version) {
	version->majorlevel = 0;
	version->minorlevel = 0;
	version->patchlevel = 0;

	sprintf(version->string, "%d.%d.%d", version->majorlevel, version->minorlevel, version->patchlevel);
}
