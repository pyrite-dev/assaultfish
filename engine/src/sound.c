#include <GearSrc/Sound.h>

#include <GearSrc/Log.h>

#include <miniaudio.h>

static void data_callback(ma_device* device, void* output, const void* input, ma_uint32 frame) {
}

GSSound GSSoundOpen(GSClient client) {
	GSSound sound = malloc(sizeof(*sound));

	memset(sound, 0, sizeof(*sound));

	sound->engine = client->engine;

	sound->config			= ma_device_config_init(ma_device_type_playback);
	sound->config.playback.format	= ma_format_s16;
	sound->config.playback.channels = 2;
	sound->config.sampleRate	= GSSoundRate;
	sound->config.dataCallback	= data_callback;
	sound->config.pUserData		= sound;

	if(ma_device_init(NULL, &sound->config, &sound->device) != MA_SUCCESS) {
		free(sound);
		GSLog(client->engine, GSLogInfo, "Failed to open sound");
		return NULL;
	}

	if(ma_device_start(&sound->device) != MA_SUCCESS) {
		ma_device_uninit(&sound->device);
		free(sound);
		GSLog(client->engine, GSLogInfo, "Failed to open sound");
		return NULL;
	}

	GSLog(client->engine, GSLogInfo, "Opened sound");

	return sound;
}

void GSSoundClose(GSSound sound) {
	ma_device_uninit(&sound->device);

	GSLog(sound->engine, GSLogInfo, "Closed sound");

	free(sound);
}
