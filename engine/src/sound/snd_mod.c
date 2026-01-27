#include <GearSrc/Sound.h>

#include <GearSrc/SoundDriver.h>
#include <GearSrc/File.h>

#include <jar_mod.h>

static const char* sig[] = {"M!K!", "M.K.", "FLT4", "FLT8", "4CHN", "6CHN", "8CHN", "10CH", "12CH", "14CH", "16CH", "18CH", "20CH", "22CH", "24CH", "26CH", "28CH", "30CH", "32CH"};

static void snd_reset(GSSound sound) {
	jar_mod_seek_start(sound->opaque1);
}

static int snd_read(GSSound sound, GSI16* out, int frame) {
	int f = ((jar_mod_context_t*)sound->opaque1)->last;

	if(f == -1) f = frame;

	jar_mod_fillbuffer(sound->opaque1, out, frame, NULL);

	return f;
}

static void snd_close(GSSound sound) {
	if(sound->opaque1 != NULL) {
		jar_mod_unload(sound->opaque1);
		free(sound->opaque1);
	}
	free(sound->opaque2);
}

static GSBool compare(void* buf) {
	int i;
	for(i = 0; i < sizeof(sig) / sizeof(sig[0]); i++) {
		if(memcmp(((unsigned char*)buf) + 1080, sig[i], 4) == 0) return GSTrue;
	}

	return GSFalse;
}

GSSound GSSoundOpenMOD(GSSoundEngine sengine, GSFile file) {
	GSSound sound = GSSoundNew(sengine);
	int	size;

	GSFileSeek(file, 0);

	size	       = GSFileSize(file);
	sound->opaque2 = malloc(size);
	GSFileRead(file, sound->opaque2, size);

	sound->reset = snd_reset;
	sound->read  = snd_read;
	sound->close = snd_close;

	if(size < 1084 || !compare(sound->opaque2)) {
		GSSoundClose(sound);

		return NULL;
	}

	sound->opaque1 = malloc(sizeof(jar_mod_context_t));
	jar_mod_init(sound->opaque1);

	if(!jar_mod_load(sound->opaque1, sound->opaque2, size)) {
		free(sound->opaque1);
		sound->opaque1 = NULL;

		GSSoundClose(sound);

		return NULL;
	}

	jar_mod_setcfg(sound->opaque1, GSSoundDriverRate, 16, 1, 1, 0);

	return sound;
}
