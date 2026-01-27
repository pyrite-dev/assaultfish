#include <GearSrc/SoundEngine.h>

#include <GearSrc/SoundDriver.h>
#include <GearSrc/Log.h>
#include <GearSrc/Sound.h>

#include <miniaudio.h>
#include <stb_ds.h>

static void read_callback(void* opaque, GSI16* out, int frame) {
	GSSoundEngine sengine = opaque;
	int	      i;
	GSNumber*     buffer = malloc(sizeof(*buffer) * frame * 2);
	GSI16*	      result = malloc(sizeof(*result) * frame * 2);

	for(i = 0; i < frame * 2; i++) buffer[i] = 0;

	GSSoundEngineLock(sengine);
	for(i = 0; i < arrlen(sengine->sound); i++) {
		int s;

		GSSoundLock(sengine->sound[i]);
		if(sengine->sound[i]->paused || sengine->sound[i]->read == NULL) {
			GSSoundUnlock(sengine->sound[i]);
			continue;
		}

		memset(result, 0, sizeof(*result) * frame * 2);
		s = sengine->sound[i]->read(sengine->sound[i], result, frame);
		if(s > 0) {
			int j;
			for(j = 0; j < s * 2; j++) buffer[j] += (GSNumber)result[j] / 32767;
		} else if(sengine->sound[i]->loop && sengine->sound[i]->reset != NULL) {
			sengine->sound[i]->reset(sengine->sound[i]);
		} else {
			sengine->sound[i]->paused = GSTrue;
		}
		GSSoundUnlock(sengine->sound[i]);
	}
	GSSoundEngineUnlock(sengine);

	for(i = 0; i < frame * 2; i++) out[i] = buffer[i] * 32767;

	free(result);
	free(buffer);
}

GSSoundEngine GSSoundEngineCreate(GSClient client) {
	GSSoundEngine sengine = malloc(sizeof(*sengine));

	memset(sengine, 0, sizeof(*sengine));

	sengine->engine = client->engine;

	ma_mutex_init(&sengine->mutex);

	sengine->sound = NULL;

	if((sengine->driver = GSSoundDriverOpen(client, read_callback, sengine)) == NULL) {
		GSLog(client->engine, GSLogError, "Failed to open sound driver");
		GSSoundEngineDestroy(sengine);

		return NULL;
	}

	GSLog(client->engine, GSLogInfo, "Created sound engine");

	return sengine;
}

void GSSoundEngineDestroy(GSSoundEngine sengine) {
	int i;

	if(sengine->driver != NULL) GSSoundDriverClose(sengine->driver);

	ma_mutex_uninit(&sengine->mutex);

	for(i = 0; i < arrlen(sengine->sound); i++) {
		GSSoundClose(sengine->sound[i]);
	}
	arrfree(sengine->sound);

	GSLog(sengine->engine, GSLogInfo, "Destroyed sound engine");

	free(sengine);
}

void GSSoundEngineLock(GSSoundEngine sengine) {
	ma_mutex_lock(&sengine->mutex);
}

void GSSoundEngineUnlock(GSSoundEngine sengine) {
	ma_mutex_unlock(&sengine->mutex);
}
