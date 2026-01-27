#include <GearSrc/Sound.h>

#include <GearSrc/SoundDriver.h>
#include <GearSrc/File.h>

#include <jar_xm.h>

static void snd_reset(GSSound sound) {
	jar_xm_reset(sound->opaque1);
}

static int snd_read(GSSound sound, GSI16* out, int frame) {
	int f = jar_xm_get_last(sound->opaque1);

	if(f == -1) f = frame;

	jar_xm_generate_samples_16bit(sound->opaque1, out, frame);

	return f;
}

static void snd_close(GSSound sound) {
	if(sound->opaque1 != NULL) jar_xm_free_context(sound->opaque1);
	free(sound->opaque2);
}

GSSound GSSoundOpenXM(GSSoundEngine sengine, GSFile file) {
	GSSound sound = GSSoundNew(sengine);
	int	size;

	GSFileSeek(file, 0);

	size	       = GSFileSize(file);
	sound->opaque2 = malloc(size);
	GSFileRead(file, sound->opaque2, size);

	sound->reset = snd_reset;
	sound->read  = snd_read;
	sound->close = snd_close;

	if(size < 37 || memcmp(sound->opaque2, "Extended Module: ", 17) != 0 || ((unsigned char*)sound->opaque2)[37] != 0x1a) {
		GSSoundClose(sound);

		return NULL;
	}

	if(jar_xm_create_context_safe((jar_xm_context_t**)&sound->opaque1, sound->opaque2, size, GSSoundDriverRate) != 0) {
		GSSoundClose(sound);

		return NULL;
	}

	return sound;
}
