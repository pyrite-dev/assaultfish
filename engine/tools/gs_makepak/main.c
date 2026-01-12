#include <stb_ds.h>
#include <lz4.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/stat.h>
#include <dirent.h>
#endif
#include <stdio.h>

#include <GearSrc/Endian.h>
#include <GearSrc/CRC.h>
#include <GearSrc/String.h>

typedef struct file file_t;

struct file {
	char*	       name;
	int	       dir;
	int	       before;
	int	       after;
	unsigned char* data;
	file_t*	       parent;
	file_t**       files;
};

static FILE*	    out;
static unsigned int total    = 0;
static unsigned int totalact = 0, totalcmp = 0;
static file_t*	    root;
static GSU32	    seek = 0;

static file_t* new_file(file_t* parent, const char* name) {
	file_t* n = malloc(sizeof(*n));

	n->name	  = GSStringDuplicate(name);
	n->dir	  = 0;
	n->before = 0;
	n->after  = 0;
	n->data	  = NULL;
	n->parent = parent;
	n->files  = NULL;

	if(parent != NULL) {
		arrput(parent->files, n);
	}

	return n;
}

static void scan(file_t* parent, const char* path, const char* pre);

static void pack(file_t* parent, const char* fullpath, const char* path, const char* name) {
	int	       m;
	FILE*	       f = fopen(fullpath, "rb");
	int	       insz;
	int	       outsz;
	unsigned char* input;
	unsigned char* output;

	printf("%32s ", path);
	fflush(stdout);

	fseek(f, 0, SEEK_END);
	insz = ftell(f);
	fseek(f, 0, SEEK_SET);

	m = LZ4_compressBound(insz);

	input  = malloc(insz);
	output = malloc(m);

	fread(input, 1, m, f);

	outsz = LZ4_compress_default(input, output, insz, m);

	if(outsz > 0) {
		file_t* n = new_file(parent, name);

		printf("done (%8d -> %8d)\n", insz, outsz);

		total++;
		totalact += insz;
		totalcmp += outsz;

		free(input);

		n->before = insz;
		n->after  = outsz;
		n->data	  = output;

		fclose(f);
		return;
	} else {
		printf("failed\n");
	}

	free(input);
	free(output);

	fclose(f);
}

static int is_dir(const char* path) {
#ifdef _WIN32
	DWORD dw;

	if((dw = GetFileAttributes(path)) == INVALID_FILE_ATTRIBUTES || !(dw & FILE_ATTRIBUTE_DIRECTORY)) return 0;

	return 1;
#else
	struct stat s;

	if(stat(path, &s) == 0 && S_ISDIR(s.st_mode)) return 1;

	return 0;
#endif
}

static void found(file_t* parent, const char* path, const char* pre, const char* input) {
	char* p	 = malloc(strlen(pre) + strlen(input) + 1 + 1);
	char* fp = malloc(strlen(path) + 1 + strlen(input) + 1);

	p[0] = 0;
	strcat(p, pre);
	strcat(p, input);

	fp[0] = 0;
	strcat(fp, path);
	strcat(fp, "/");
	strcat(fp, input);

	if(is_dir(fp)) {
		file_t* d = new_file(parent, input);

		d->dir = 1;

		strcat(p, "/");

		scan(d, fp, p);
	} else {
		pack(parent, fp, p, input);
	}

	free(fp);

	free(p);
}

static void scan(file_t* parent, const char* path, const char* pre) {
#ifdef _WIN32
	WIN32_FIND_DATA ffd;
	HANDLE		hFind;
	char*		w = malloc(strlen(path) + 2 + 1);

	w[0] = 0;
	strcat(w, path);
	strcat(w, "/*");

	if((hFind = FindFirstFile(w, &ffd)) == INVALID_HANDLE_VALUE) {
		free(w);
		return;
	}

	do {
		if(strcmp(ffd.cFileName, ".") != 0 && strcmp(ffd.cFileName, "..") != 0) {
			found(parent, path, pre, ffd.cFileName);
		}
	} while(FindNextFile(hFind, &ffd) != 0);
	FindClose(hFind);
#else
	DIR* dir;
	if((dir = opendir(path)) != NULL) {
		struct dirent* d;
		while((d = readdir(dir)) != NULL) {
			if(strcmp(d->d_name, "..") != 0 && strcmp(d->d_name, ".") != 0) {
				found(parent, path, pre, d->d_name);
			}
		}

		closedir(dir);
	}
#endif
}

static void recursive_write(file_t* parent) {
	int i;

	if(parent->dir) {
		for(i = 0; i < arrlen(parent->files); i++) {
			recursive_write(parent->files[i]);
		}
	} else if(parent->data != NULL && parent->after > 0) {
		fwrite(parent->data, 1, parent->after, out);
	}
}

static int total_size(file_t* parent, int level) {
	int sz = level == 0 ? 0 : (1 + 4 + 1 + strlen(parent->name) + 2 + 4);
	int i;

	for(i = 0; i < arrlen(parent->files); i++) {
		if(parent->files[i]->dir) {
			sz += total_size(parent->files[i], level + 1);
		} else {
			sz += 1 + 4 + 1 + strlen(parent->files[i]->name) + 4 + 4 + 4;
		}
	}

	return sz;
}

static void recursive(file_t* parent) {
	int	      i;
	unsigned char c;
	GSU16	      u16 = 0;
	GSU32	      u32 = 0;
	GSU32	      crc;

	crc = GSCRC32(parent->name, strlen(parent->name));

	c = parent->dir ? 1 : 0;
	fwrite(&c, 1, 1, out);

	u32 = GSEndianSwapU32BE(crc);
	fwrite(&u32, 1, 4, out);

	c = strlen(parent->name);
	fwrite(&c, 1, 1, out);
	fwrite(parent->name, 1, c, out);

	if(parent->dir) {
		u16 = GSEndianSwapU16BE(arrlen(parent->files));
		fwrite(&u16, 1, 2, out);

		u32 = GSEndianSwapU32BE(total_size(parent, 0));
		fwrite(&u32, 1, 4, out);

		for(i = 0; i < arrlen(parent->files); i++) {
			recursive(parent->files[i]);
		}
	} else {
		u32 = GSEndianSwapU32BE(parent->before);
		fwrite(&u32, 1, 4, out);

		u32 = GSEndianSwapU32BE(parent->after);
		fwrite(&u32, 1, 4, out);

		u32 = GSEndianSwapU32BE(seek);
		fwrite(&u32, 1, 4, out);

		seek += parent->after;
	}
}

int main(int argc, char** argv) {
	unsigned char dat[128];

	printf("GearSrc Engine resource packer\n");
	if(argc >= 2) {
		out = fopen(argv[1], "wb");

		dat[0] = 0x7f;
		dat[1] = 'P';
		dat[2] = 'A';
		dat[3] = 'K';

		fwrite(dat, 1, 4, out);

		root	  = new_file(NULL, "");
		root->dir = 1;
	}
	if(argc == 2) {
		scan(root, ".", "");
	} else if(argc > 2) {
		int i;

		for(i = 1; i < argc; i++) scan(root, argv[i], "");
	} else {
		printf("Usage: %s output [input]\n", argv[0]);

		return 1;
	}

	recursive(root);
	recursive_write(root);

	printf("total %u files added, compressed %u -> %u bytes\n", total, totalact, totalcmp);

	fclose(out);
}
