#include <GearSrc/Sound.h>

#include <GearSrc/SoundDriver.h>
#include <GearSrc/File.h>

#include <dr_mp3.h>

static void snd_reset(GSSound sound) {
	drmp3_seek_to_pcm_frame(sound->opaque1, 0);
}

static int snd_read(GSSound sound, GSI16* out, int frame) {
	return drmp3_read_pcm_frames_s16(sound->opaque1, frame, out);
}

static void snd_close(GSSound sound) {
	if(sound->opaque1 != NULL) {
		drmp3_uninit(sound->opaque1);
		free(sound->opaque1);
	}
	free(sound->opaque2);
}

GSSound GSSoundOpenMP3(GSSoundEngine sengine, GSFile file) {
	GSSound	       sound = GSSoundNew(sengine);
	int	       size;
	unsigned char* buf;

	size	       = GSFileSize(file);
	sound->opaque2 = malloc(size);
	GSFileRead(file, sound->opaque2, size);

	sound->reset = snd_reset;
	sound->read  = snd_read;
	sound->close = snd_close;

	buf = sound->opaque2;
	if(size > 2 && buf[0] == 0xff && (buf[1] == 0xfb || buf[1] == 0xf3 || buf[2] == 0xf2)) {
	} else if(size > 3 && memcmp(buf, "ID3", 3) == 0) {
	} else {
		GSSoundClose(sound);

		return NULL;
	}

	sound->opaque1 = malloc(sizeof(drmp3));
	if(!drmp3_init_memory(sound->opaque1, sound->opaque2, size, NULL)) {
		free(sound->opaque1);
		sound->opaque1 = NULL;

		GSSoundClose(sound);

		return NULL;
	}

	sound->from_samplerate = ((drmp3*)sound->opaque1)->sampleRate;

	return sound;
}
