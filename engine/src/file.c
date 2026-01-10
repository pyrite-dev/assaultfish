#include <GearBox/File.h>

#include <GearBox/String.h>
#include <GearBox/Log.h>
#include <GearBox/Resource.h>

#include <stb_ds.h>

GBFile GBFileOpen(GBEngine engine, const char* path) {
	GBFile file = malloc(sizeof(*file));
	char*  s;
	char*  nam;

	memset(file, 0, sizeof(*file));

	file->seek = 0;

	if((file->fp = fopen(path, "rb")) != NULL) {
		fseek(file->fp, 0, SEEK_END);
		file->size = ftell(file->fp);
		fseek(file->fp, 0, SEEK_SET);

		GBLog(GBLogInfo, "Opened %s", path);

		return file;
	}

	s = GBStringDuplicate(path);
	if(engine != NULL && ((nam = strstr(s, ":/")) != NULL || (nam = strstr(s, ":\\")) != NULL)) {
		int i;

		nam[0] = 0;
		nam[1] = 0;
		nam += 2;

		for(i = 0; i < shlen(engine->resource); i++) {
			if(strcmp(engine->resource[i].key, s) == 0) {
				if((file->data = GBResourceGet(engine->resource[i].value, nam, &file->size)) != NULL) {
					free(s);

					GBLog(GBLogInfo, "Opened %s", path);
					return file;
				}
				break;
			}
		}
	}
	free(s);

	GBLog(GBLogError, "Could not open %s", path);
	GBFileClose(file);
	return NULL;
}

int GBFileRead(GBFile file, void* out, int size) {
	if(file->fp != NULL) return fread(out, 1, size, file->fp);
	if(file->data != NULL) {
		int len = 0;

		if((file->seek + size) > file->size) {
			len = file->size - file->seek;
		} else {
			len = size;
		}

		memcpy(out, file->data + file->seek, len);

		file->seek += len;
	}
}

void GBFileSeek(GBFile file, int pos) {
	if(file->fp != NULL) fseek(file->fp, pos, SEEK_SET);
	file->seek = pos;
}

unsigned int GBFileSize(GBFile file) {
	return file->size;
}

void GBFileClose(GBFile file) {
	if(file->fp != NULL) fclose(file->fp);
	if(file->data != NULL) free(file->data);

	free(file);
}
