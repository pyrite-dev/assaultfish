#include <GearSrc/Sound.h>

#include <GearSrc/File.h>
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

	ma_mutex_init(&sound->mutex);

	GSSoundEngineLock(sengine);
	arrput(sengine->sound, sound);
	GSSoundEngineUnlock(sengine);

	return sound;
}

GSSound GSSoundOpen(GSSoundEngine sengine, const char* path) {
	GSSound s;
	GSFile	f;

	if((f = GSFileOpen(sengine->engine, path)) == NULL) return NULL;
	if((s = GSSoundOpenXM(sengine, f)) != NULL) {
		s->file = f;
		GSLog(sengine->engine, GSLogInfo, "%s: Opened as XM", path);
		return s;
	}

	if((s = GSSoundOpenMOD(sengine, f)) != NULL) {
		s->file = f;
		GSLog(sengine->engine, GSLogInfo, "%s: Opened as MOD", path);
		return s;
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
