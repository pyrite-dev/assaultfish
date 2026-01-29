#include <GearSrc/Resource.h>

#include <GearSrc/File.h>
#include <GearSrc/Log.h>
#include <GearSrc/Endian.h>
#include <GearSrc/CRC.h>
#include <GearSrc/String.h>

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

	resource->engine = engine;

	if((resource->file = GSFileOpen(engine, path)) == NULL) {
		GSResourceClose(resource);
		return NULL;
	}

	if(GSFileRead(resource->file, sig, 4) < 4) {
		GSResourceClose(resource);
		return NULL;
	}

	if(memcmp(sig, res_sig, 4) != 0) {
		GSLog(engine, GSLogError, "%s: Resource has wrong signature", path);
		GSResourceClose(resource);
		return NULL;
	}

	return resource;
}

int find_entry(GSEngine engine, GSFile f, const char* name, int max, GSU32* totsz, GSU32* actsz, GSU32* cmpsz, GSU32* seekpos) {
	GSU32 crc = GSEndianSwapU32BE(GSCRC32((void*)name, strlen(name)));
	int   i;

	for(i = 0; i < max; i++) {
		unsigned char dir;
		unsigned char len;
		char*	      fn;
		GSU32	      t;

		if(GSFileRead(f, &dir, 1) < 1) return -1;
		if(GSFileRead(f, &t, 4) < 4) return -1;
		if(GSFileRead(f, &len, 1) < 1) return -1;

		fn = malloc(len + 1);
		if(len > 0) {
			if(GSFileRead(f, fn, len) < len) {
				free(fn);
				return -1;
			}
		}
		fn[len] = 0;

		if(dir) {
			GSU16 n;
			GSU32 tsz;

			if(GSFileRead(f, &n, 2) < 2) {
				free(fn);
				return -1;
			}

			if(GSFileRead(f, &tsz, 4) < 4) {
				free(fn);
				return -1;
			}

			n   = GSEndianSwapU16BE(n);
			tsz = GSEndianSwapU32BE(tsz);

			if(totsz != NULL) *totsz = tsz;

			if(crc == t && strcmp(fn, name) == 0) {
				GSLog(engine, GSLogDebug, "Directory %s/ found", fn);
				free(fn);
				return n;
			}

			GSFileSeekFromCurrent(f, tsz);
		} else {
			if(GSFileRead(f, actsz, 4) < 4) {
				free(fn);
				return -1;
			}

			if(GSFileRead(f, cmpsz, 4) < 4) {
				free(fn);
				return -1;
			}

			if(GSFileRead(f, seekpos, 4) < 4) {
				free(fn);
				return -1;
			}

			*actsz	 = GSEndianSwapU32BE(*actsz);
			*cmpsz	 = GSEndianSwapU32BE(*cmpsz);
			*seekpos = GSEndianSwapU32BE(*seekpos);

			if(crc == t && strcmp(fn, name) == 0) {
				GSLog(engine, GSLogDebug, "File %s found", fn);
				free(fn);
				return -2;
			}
		}

		free(fn);
	}

	return -1;
}

void* GSResourceGet(GSResource resource, const char* name, unsigned int* size) {
	char*	       p = GSStringDuplicate(name);
	char*	       s = p;
	char*	       f = s;
	int	       m = 1;
	GSU32	       totsz;
	GSU32	       actsz;
	GSU32	       cmpsz;
	GSU32	       seekpos;
	unsigned char* d = NULL;

	GSFileSeek(resource->file, 4);

	while(1) {
		s = strchr(s, '/');

		if(s != NULL) s[0] = 0;

		m = find_entry(resource->engine, resource->file, f, m, strlen(f) == 0 ? &totsz : NULL, &actsz, &cmpsz, &seekpos);
		if(m == -1) break;
		if(m == -2) {
			unsigned char* cmp = malloc(cmpsz);

			if(actsz > cmpsz * 255) actsz = cmpsz * 255;

			d = malloc(actsz);

			GSFileSeek(resource->file, 4 + 1 + 4 + 1 + 2 + 4 + totsz + seekpos);
			GSFileRead(resource->file, cmp, cmpsz);

			LZ4_decompress_safe(cmp, d, cmpsz, actsz);

			free(cmp);

			*size = actsz;
			break;
		}

		if(s == NULL) break;
		s++;
		f = s;
	}

	free(p);

	return d;
}

void GSResourceClose(GSResource resource) {
	if(resource->file != NULL) GSFileClose(resource->file);

	free(resource);
}
