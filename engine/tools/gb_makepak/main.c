#include <stb_ds.h>
#include <lz4.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/stat.h>
#include <dirent.h>
#endif
#include <stdio.h>

typedef struct kv {
	char* key;

} kv_t;

static FILE* out;

static void scan(const char* path, const char* pre);

static void pack(const char* fullpath, const char* path) {
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
		int  sz = outsz;
		int  i;
		char fn[128];

		memset(fn, 0, 128);
		memcpy(fn, path, strlen(path));

		fwrite(fn, 1, 128, out);

		for(i = 0; i < 4; i++) {
			unsigned char n = (sz >> 24) & 0xff;

			fwrite(&n, 1, 1, out);

			sz = sz << 8;
		}

		fwrite(output, 1, outsz, out);

		printf("done (%6d -> %6d)\n", insz, outsz);
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

	if((dw = GetFileAttributes(path)) == INVALID_FILE_ATTRIBUTES && !(dw & FILE_ATTRIBUTE_DIRECTORY)) return 0;

	return 1;
#else
	struct stat s;

	if(stat(path, &s) == 0 && S_ISDIR(s.st_mode)) return 1;

	return 0;
#endif
}

static void found(const char* path, const char* pre, const char* input) {
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
		strcat(p, "/");

		scan(fp, p);
	} else {
		pack(fp, p);
	}

	free(fp);

	free(p);
}

static void scan(const char* path, const char* pre) {
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
			found(path, pre, ffd.cFileName);
		}
	} while(FindNextFile(hFind, &ffd) != 0);
	FindClose(hFind);
#else
	DIR* dir;
	if((dir = opendir(path)) != NULL) {
		struct dirent* d;
		while((d = readdir(dir)) != NULL) {
			if(strcmp(d->d_name, "..") != 0 && strcmp(d->d_name, ".") != 0) {
				found(path, pre, d->d_name);
			}
		}

		closedir(dir);
	}
#endif
}

int main(int argc, char** argv) {
	unsigned char nul[128];

	if(argc >= 2) out = fopen(argv[1], "wb");
	if(argc == 2) {
		scan(".", "");
	} else if(argc > 2) {
		int i;

		for(i = 1; i < argc; i++) scan(argv[i], "");
	} else {
		printf("Usage: %s output [input]\n", argv[0]);

		return 1;
	}

	memset(nul, 0, 128);
	fwrite(nul, 1, 128, out);

	fclose(out);
}
