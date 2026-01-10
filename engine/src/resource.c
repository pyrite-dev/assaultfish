#include <GearBox/Resource.h>

#include <GearBox/File.h>
#include <GearBox/Log.h>
#include <GearBox/Endian.h>

#include <lz4.h>

static unsigned char res_sig[4] = {
    0x7f,
    'P',
    'A',
    'K'};

GBResource GBResourceOpen(GBEngine engine, const char* path) {
	GBResource    resource = malloc(sizeof(*resource));
	unsigned char sig[4];

	memset(resource, 0, sizeof(*resource));

	if((resource->file = GBFileOpen(engine, path)) == NULL) {
		GBResourceClose(resource);
		return NULL;
	}

	if(GBFileRead(resource->file, sig, 4) < 4) {
		GBResourceClose(resource);
		return NULL;
	}

	if(memcmp(sig, res_sig, 4) != 0) {
		GBLog(GBLogError, "%s: Resource has wrong signature", path);
		GBResourceClose(resource);
		return NULL;
	}

	return resource;
}

void* GBResourceGet(GBResource resource, const char* name, unsigned int* size) {
	char fn[129];

	fn[128] = 0;

	GBFileSeek(resource->file, 4);

	while(!(GBFileRead(resource->file, fn, 128) < 128 || fn[0] == 0)) {
		GBU32 actsz = 0;
		GBU32 cmpsz = 0;
		int   i;
		void* d;

		if(GBFileRead(resource->file, &actsz, 4) < 4) return NULL;
		if(GBFileRead(resource->file, &cmpsz, 4) < 4) return NULL;

		actsz = GBEndianSwapU32BE(actsz);
		cmpsz = GBEndianSwapU32BE(cmpsz);

		*size = actsz;

		d = malloc(cmpsz);
		if(GBFileRead(resource->file, d, cmpsz) < cmpsz) {
			free(d);
			return NULL;
		}
		if(strcmp(fn, name) == 0) {
			unsigned char* r = malloc(actsz);
			LZ4_decompress_safe(d, r, cmpsz, actsz);

			free(d);

			return r;
		} else {
			free(d);
		}
	}

	return NULL;
}

void GBResourceClose(GBResource resource) {
	if(resource->file != NULL) GBFileClose(resource->file);

	free(resource);
}
