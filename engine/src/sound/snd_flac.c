#include <GearSrc/Sound.h>

#include <GearSrc/SoundDriver.h>
#include <GearSrc/File.h>

#include <dr_flac.h>

static void snd_reset(GSSound sound) {
	drflac_seek_to_pcm_frame(sound->opaque1, 0);
}

static int snd_read(GSSound sound, GSI16* out, int frame) {
	return drflac_read_pcm_frames_s16(sound->opaque1, frame, out);
}

static void snd_close(GSSound sound) {
	if(sound->opaque1 != NULL) {
		drflac_free(sound->opaque1, NULL);
	}
	free(sound->opaque2);
}

GSSound GSSoundOpenFLAC(GSSoundEngine sengine, GSFile file) {
	GSSound sound = GSSoundNew(sengine);
	int	size;

	size	       = GSFileSize(file);
	sound->opaque2 = malloc(size);
	GSFileRead(file, sound->opaque2, size);

	sound->reset = snd_reset;
	sound->read  = snd_read;
	sound->close = snd_close;

	if(size < 4 || memcmp(sound->opaque2, "fLaC", 4) != 0) {
		GSSoundClose(sound);

		return NULL;
	}

	if((sound->opaque1 = drflac_open_memory(sound->opaque2, size, NULL)) == NULL) {
		GSSoundClose(sound);

		return NULL;
	}

	sound->from_samplerate = ((drflac*)sound->opaque1)->sampleRate;

	return sound;
}
