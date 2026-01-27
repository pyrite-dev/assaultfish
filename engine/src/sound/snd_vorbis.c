#include <GearSrc/Sound.h>

#include <GearSrc/SoundDriver.h>
#include <GearSrc/File.h>

#include <stb_vorbis.h>

static void snd_reset(GSSound sound) {
	stb_vorbis_seek_frame(sound->opaque1, 0);
}

static int snd_read(GSSound sound, GSI16* out, int frame) {
	return stb_vorbis_get_samples_short_interleaved(sound->opaque1, 2, out, frame * 2);
}

static void snd_close(GSSound sound) {
	if(sound->opaque1 != NULL) {
		stb_vorbis_close(sound->opaque1);
	}
	free(sound->opaque2);
}

GSSound GSSoundOpenVorbis(GSSoundEngine sengine, GSFile file) {
	GSSound		sound = GSSoundNew(sengine);
	int		size;
	stb_vorbis_info info;
	int		err;

	size	       = GSFileSize(file);
	sound->opaque2 = malloc(size);
	GSFileRead(file, sound->opaque2, size);

	sound->reset = snd_reset;
	sound->read  = snd_read;
	sound->close = snd_close;

	if((sound->opaque1 = stb_vorbis_open_memory(sound->opaque2, size, &err, NULL)) == NULL) {
		GSSoundClose(sound);

		return NULL;
	}

	info = stb_vorbis_get_info(sound->opaque1);

	sound->from_samplerate = info.sample_rate;

	return sound;
}
