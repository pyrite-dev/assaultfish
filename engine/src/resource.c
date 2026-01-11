#include <GearSrc/Resource.h>

#include <GearSrc/File.h>
#include <GearSrc/Log.h>
#include <GearSrc/Endian.h>

#include <lz4.h>

static unsigned char res_sig[4] = {
    0x7f,
    'P',
    'A',
    'K'};

GSResource GSResourceOpen(GSEngine engine, const char* path) {
	GSResource    resource = malloc(sizeof(*resource));
	unsigned char sig[4];

	memset(resource, 0, sizeof(*resource));

	if((resource->file = GSFileOpen(engine, path)) == NULL) {
		GSResourceClose(resource);
		return NULL;
	}

	if(GSFileRead(resource->file, sig, 4) < 4) {
		GSResourceClose(resource);
		return NULL;
	}

	if(memcmp(sig, res_sig, 4) != 0) {
		GSLog(GSLogError, "%s: Resource has wrong signature", path);
		GSResourceClose(resource);
		return NULL;
	}

	return resource;
}

void* GSResourceGet(GSResource resource, const char* name, unsigned int* size) {
	char fn[129];

	fn[128] = 0;

	GSFileSeek(resource->file, 4);

	while(!(GSFileRead(resource->file, fn, 128) < 128 || fn[0] == 0)) {
		GSU32 actsz = 0;
		GSU32 cmpsz = 0;
		int   i;
		void* d;

		if(GSFileRead(resource->file, &actsz, 4) < 4) return NULL;
		if(GSFileRead(resource->file, &cmpsz, 4) < 4) return NULL;

		actsz = GSEndianSwapU32BE(actsz);
		cmpsz = GSEndianSwapU32BE(cmpsz);

		*size = actsz;

		d = malloc(cmpsz);
		if(GSFileRead(resource->file, d, cmpsz) < cmpsz) {
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

void GSResourceClose(GSResource resource) {
	if(resource->file != NULL) GSFileClose(resource->file);

	free(resource);
}
