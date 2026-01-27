#include <GearSrc/SoundDriver.h>

#include <GearSrc/Log.h>

#include <miniaudio.h>

static void data_callback(ma_device* device, void* output, const void* input, ma_uint32 frame) {
	GSSoundDriver driver = device->pUserData;

	driver->callback(driver->opaque, output, frame);
}

GSSoundDriver GSSoundDriverOpen(GSClient client, GSSoundDriverReadCallback callback, void* opaque) {
	GSSoundDriver driver = malloc(sizeof(*driver));

	memset(driver, 0, sizeof(*driver));

	driver->engine = client->engine;

	driver->callback = callback;
	driver->opaque	 = opaque;

	driver->config			 = ma_device_config_init(ma_device_type_playback);
	driver->config.playback.format	 = ma_format_s16;
	driver->config.playback.channels = 2;
	driver->config.sampleRate	 = GSSoundDriverRate;
	driver->config.dataCallback	 = data_callback;
	driver->config.pUserData	 = driver;

	driver->ready = 0;

	if(ma_device_init(NULL, &driver->config, &driver->device) != MA_SUCCESS) {
		GSLog(client->engine, GSLogInfo, "Failed to initialize sound driver");

		GSSoundDriverClose(driver);

		return NULL;
	}

	driver->ready = 1;

	if(ma_device_start(&driver->device) != MA_SUCCESS) {
		GSLog(client->engine, GSLogInfo, "Failed to start sound driver");

		GSSoundDriverClose(driver);

		return NULL;
	}

	GSLog(client->engine, GSLogInfo, "Opened sound driver");

	return driver;
}

void GSSoundDriverClose(GSSoundDriver driver) {
	if(driver->ready) ma_device_uninit(&driver->device);

	GSLog(driver->engine, GSLogInfo, "Closed sound driver");

	free(driver);
}
