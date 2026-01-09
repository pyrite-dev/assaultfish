#include <GearBox/File.h>

#include <GearBox/String.h>
#include <GearBox/Log.h>

GBFile GBFileOpen(GBEngine engine, const char* path) {
	GBFile file = malloc(sizeof(*file));
	char*  s;
	char*  nam;

	memset(file, 0, sizeof(*file));

	GBLog(GBLogInfo, "Opening %s", path);

	s = GBStringDuplicate(path);
	if(engine != NULL && ((nam = strstr(s, ":/")) != NULL || (nam = strstr(s, ":\\")) != NULL)) {
		nam[0] = 0;
		nam[1] = 0;
		nam += 2;

		printf("%s\n", nam);
	}
	free(s);

	if((file->fp = fopen(path, "rb")) != NULL) {
		return file;
	}

	GBLog(GBLogWarn, "Could not open %s", path);
	GBFileClose(file);
	return NULL;
}

int GBFileRead(GBFile file, void* out, int size) {
	if(file->fp != NULL) return fread(out, 1, size, file->fp);
}

void GBFileClose(GBFile file) {
	if(file->fp != NULL) fclose(file->fp);

	free(file);
}
