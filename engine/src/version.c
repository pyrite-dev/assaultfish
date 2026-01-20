#include <GearSrc/Version.h>

void GSVersionGet(GSVersion* version) {
	version->majorlevel = 0;
	version->minorlevel = 0;
	version->patchlevel = 0;

	sprintf(version->string, "%d.%d.%d", version->majorlevel, version->minorlevel, version->patchlevel);
	strcpy(version->copyright, "Copyright (C) 2026 Pyrite development team");
}
