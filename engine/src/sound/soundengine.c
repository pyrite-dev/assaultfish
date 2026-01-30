#include <GearSrc/SoundEngine.h>

#include <GearSrc/SoundDriver.h>
#include <GearSrc/Log.h>
#include <GearSrc/Sound.h>
#include <GearSrc/Math.h>

#include <miniaudio.h>
#include <stb_ds.h>

static void read_callback(void* opaque, GSI16* out, int frame) {
	GSSoundEngine sengine = opaque;
	int	      i;
	GSNumber*     buffer = malloc(sizeof(*buffer) * frame * 2);

	for(i = 0; i < frame * 2; i++) buffer[i] = 0;

	GSSoundEngineLock(sengine);
	for(i = 0; i < arrlen(sengine->sound); i++) {
		int    s, f;
		GSI16* result;
		GSI16* result2;
		GSI16* result3;

		GSSoundLock(sengine->sound[i]);
		if(sengine->sound[i]->paused || sengine->sound[i]->read == NULL) {
			GSSoundUnlock(sengine->sound[i]);
			continue;
		}

		if(sengine->sound[i]->from_samplerate > 0) {
			f = frame * sengine->sound[i]->from_samplerate / GSSoundDriverRate;
		} else {
			f = frame;
		}

		result = malloc(sizeof(*result) * f * sengine->sound[i]->from_channel);
		memset(result, 0, sizeof(*result) * f * sengine->sound[i]->from_channel);
		s = sengine->sound[i]->read(sengine->sound[i], result, f);
		if(s > 0) {
			int	 j;
			GSNumber la = 1, ra = 1;

			if(sengine->sound[i]->is_3d) {
				GSNumber  angle;
				GSNumber  v;
				GSNumber  d;
				GSVector3 r;
				GSNumber  lf, rf;

				GSMathSubtract3(r, sengine->sound[i]->position, sengine->engine->client->position);

				angle = atan2(r[2], r[0]);
				v     = cos(angle);

				lf = 1.0 - v;
				rf = 1.0 + v;

				d = GSMathLength3(r) / 4;

				la = 1.0 / (1.0 + lf + d * d);
				ra = 1.0 / (1.0 + rf + d * d);
			}

			if(sengine->sound[i]->from_samplerate > 0) {
				ma_uint64 fin  = f;
				ma_uint64 fout = frame;

				result2 = malloc(sizeof(*result2) * frame * sengine->sound[i]->from_channel);
				memset(result2, 0, sizeof(*result2) * frame * sengine->sound[i]->from_channel);

				ma_resampler_process_pcm_frames(&sengine->sound[i]->resampler, result, &fin, result2, &fout);

				s = s * GSSoundDriverRate / sengine->sound[i]->from_samplerate;
			} else {
				result2 = result;
			}

			if(sengine->sound[i]->from_channel == 2) {
				result3 = result2;
			} else {
				result3 = malloc(sizeof(*result3) * frame * 2);
				memset(result3, 0, sizeof(*result3) * frame * 2);

				ma_channel_converter_process_pcm_frames(&sengine->sound[i]->converter, result3, result2, frame);
			}

			for(j = 0; j < s * 2; j += 2) {
				buffer[j + 0] += (GSNumber)result3[j + 0] / 32767 * la * sengine->sound[i]->volume;
				buffer[j + 1] += (GSNumber)result3[j + 1] / 32767 * ra * sengine->sound[i]->volume;
			}

			if(result3 != result2) free(result3);
			if(result2 != result) free(result2);
		} else if(sengine->sound[i]->loop && sengine->sound[i]->reset != NULL) {
			sengine->sound[i]->reset(sengine->sound[i]);
		} else {
			sengine->sound[i]->paused = GSTrue;
		}
		free(result);
		GSSoundUnlock(sengine->sound[i]);
	}
	GSSoundEngineUnlock(sengine);

	for(i = 0; i < frame * 2; i++) out[i] = buffer[i] * 32767;

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
