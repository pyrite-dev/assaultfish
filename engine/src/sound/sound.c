#include <GearSrc/Sound.h>

#include <GearSrc/File.h>
#include <GearSrc/SoundDriver.h>
#include <GearSrc/SoundEngine.h>
#include <GearSrc/Sound.h>
#include <GearSrc/Log.h>

#include <stb_ds.h>
#include <miniaudio.h>

GSSound GSSoundNew(GSSoundEngine sengine) {
	GSSound sound = malloc(sizeof(*sound));

	memset(sound, 0, sizeof(*sound));

	sound->engine = sengine->engine;

	sound->paused = 1;
	sound->loop   = 0;

	sound->from_samplerate = 0;
	sound->from_channel    = 2;

	ma_mutex_init(&sound->mutex);

	GSSoundEngineLock(sengine);
	arrput(sengine->sound, sound);
	GSSoundEngineUnlock(sengine);

	return sound;
}

static void after(GSSound s) {
	GSSoundLock(s);
	if(s->from_samplerate > 0) {
		s->resampler_config = ma_resampler_config_init(ma_format_s16, s->from_channel, s->from_samplerate, GSSoundDriverRate, ma_resample_algorithm_linear);
		ma_resampler_init(&s->resampler_config, NULL, &s->resampler);
	}
	if(s->from_channel != 2) {
		s->converter_config = ma_channel_converter_config_init(ma_format_s16, s->from_channel, NULL, 2, NULL, ma_channel_mix_mode_default);
		ma_channel_converter_init(&s->converter_config, NULL, &s->converter);
	}
	GSSoundUnlock(s);
}

typedef struct pair {
	const char* name;
	GSSound (*callback)(GSSoundEngine sengine, GSFile file);
} pair_t;

GSSound GSSoundOpen(GSSoundEngine sengine, const char* path) {
	GSSound s;
	GSFile	f;
	int	i;
	pair_t	formats[] = {
	     {"XM", GSSoundOpenXM},	    /**/
	     {"MOD", GSSoundOpenMOD},	    /**/
	     {"MP3", GSSoundOpenMP3},	    /**/
	     {"FLAC", GSSoundOpenFLAC},	    /**/
	     {"Vorbis", GSSoundOpenVorbis}, /**/
	     {"WAV", GSSoundOpenWAV}	    /**/
	 };

	if((f = GSFileOpen(sengine->engine, path)) == NULL) return NULL;

	for(i = 0; i < sizeof(formats) / sizeof(formats[0]); i++) {
		GSFileSeek(f, 0);

		if((s = formats[i].callback(sengine, f)) != NULL) {
			s->file = f;
			GSLog(sengine->engine, GSLogInfo, "%s: Opened as %s", path, formats[i].name);
			after(s);
			return s;
		}
	}

	GSLog(sengine->engine, GSLogError, "%s: Unrecognized format", path);

	GSFileClose(f);

	return NULL;
}

void GSSoundClose(GSSound sound) {
	int	      i;
	GSSoundEngine sengine = sound->engine->client->sengine;

	GSSoundEngineLock(sengine);
	for(i = 0; i < arrlen(sengine->sound); i++) {
		if(sengine->sound[i] == sound) {
			arrdel(sengine->sound, i);
			break;
		}
	}
	GSSoundEngineUnlock(sengine);

	if(sound->close != NULL) sound->close(sound);

	ma_mutex_uninit(&sound->mutex);

	if(sound->file != NULL) GSFileClose(sound->file);

	if(sound->from_samplerate > 0) ma_resampler_uninit(&sound->resampler, NULL);

	if(sound->from_channel != 2) ma_channel_converter_uninit(&sound->converter, NULL);

	free(sound);
}

void GSSoundToggleLoop(GSSound sound, GSBool toggle) {
	GSSoundLock(sound);
	sound->loop = toggle;
	GSSoundUnlock(sound);
}

void GSSoundStart(GSSound sound) {
	GSSoundLock(sound);
	if(sound->reset != NULL) sound->reset(sound);
	GSSoundUnlock(sound);

	GSSoundResume(sound);
}

void GSSoundResume(GSSound sound) {
	GSSoundLock(sound);
	sound->paused = 0;
	GSSoundUnlock(sound);
}

void GSSoundPause(GSSound sound) {
	GSSoundLock(sound);
	sound->paused = 1;
	GSSoundUnlock(sound);
}

void GSSoundLock(GSSound sound) {
	ma_mutex_lock(&sound->mutex);
}

void GSSoundUnlock(GSSound sound) {
	ma_mutex_unlock(&sound->mutex);
}

GSBool GSSoundIsPaused(GSSound sound) {
	GSBool b;

	GSSoundLock(sound);
	b = sound->paused;
	GSSoundUnlock(sound);

	return b;
}
