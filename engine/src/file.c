#include <GearSrc/File.h>

#include <GearSrc/String.h>
#include <GearSrc/Log.h>
#include <GearSrc/Resource.h>

#include <stb_ds.h>

GSFile GSFileOpen(GSEngine engine, const char* path) {
	GSFile file = malloc(sizeof(*file));
	char*  s;
	char*  nam;

	memset(file, 0, sizeof(*file));

	file->seek = 0;

	if((file->fp = fopen(path, "rb")) != NULL) {
		fseek(file->fp, 0, SEEK_END);
		file->size = ftell(file->fp);
		fseek(file->fp, 0, SEEK_SET);

		GSLog(GSLogInfo, "Opened %s", path);

		return file;
	}

	s = GSStringDuplicate(path);
	if(engine != NULL && ((nam = strstr(s, ":/")) != NULL || (nam = strstr(s, ":\\")) != NULL)) {
		int i;

		nam[0] = 0;
		nam += 1;

		for(i = 0; i < shlen(engine->resource); i++) {
			if(strcmp(engine->resource[i].key, s) == 0) {
				if((file->data = GSResourceGet(engine->resource[i].value, nam, &file->size)) != NULL) {
					free(s);

					GSLog(GSLogInfo, "Opened %s", path);
					return file;
				}
				break;
			}
		}
	}
	free(s);

	GSLog(GSLogError, "Could not open %s", path);
	GSFileClose(file);
	return NULL;
}

int GSFileRead(GSFile file, void* out, int size) {
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

		return len;
	}

	return 0;
}

void GSFileSeek(GSFile file, int pos) {
	if(file->fp != NULL) fseek(file->fp, pos, SEEK_SET);
	file->seek = pos;
}

void GSFileSeekFromCurrent(GSFile file, int pos) {
	if(file->fp != NULL) fseek(file->fp, pos, SEEK_CUR);
	file->seek += pos;
}

unsigned int GSFileSize(GSFile file) {
	return file->size;
}

void GSFileClose(GSFile file) {
	if(file->fp != NULL) fclose(file->fp);
	if(file->data != NULL) free(file->data);

	free(file);
}
