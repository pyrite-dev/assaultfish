#include <GearSrc/Sound.h>

#include <GearSrc/SoundDriver.h>
#include <GearSrc/File.h>

#include <dr_wav.h>

static void snd_reset(GSSound sound) {
	drwav_seek_to_pcm_frame(sound->opaque1, 0);
}

static int snd_read(GSSound sound, GSI16* out, int frame) {
	return drwav_read_pcm_frames_s16(sound->opaque1, frame, out);
}

static void snd_close(GSSound sound) {
	if(sound->opaque1 != NULL) {
		drwav_uninit(sound->opaque1);
		free(sound->opaque1);
	}
	free(sound->opaque2);
}

GSSound GSSoundOpenWAV(GSSoundEngine sengine, GSFile file) {
	GSSound sound = GSSoundNew(sengine);
	int	size;

	size	       = GSFileSize(file);
	sound->opaque2 = malloc(size);
	GSFileRead(file, sound->opaque2, size);

	sound->reset = snd_reset;
	sound->read  = snd_read;
	sound->close = snd_close;

	if(size < 4 || memcmp(sound->opaque2, "RIFF", 4) != 0) {
		GSSoundClose(sound);

		return NULL;
	}

	sound->opaque1 = malloc(sizeof(drwav));
	if(!drwav_init_memory(sound->opaque1, sound->opaque2, size, NULL)) {
		free(sound->opaque1);
		sound->opaque1 = NULL;

		GSSoundClose(sound);

		return NULL;
	}

	sound->from_samplerate = ((drwav*)sound->opaque1)->sampleRate;

	return sound;
}
