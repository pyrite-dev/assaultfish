#include <GearSrc/Client.h>

#include <GearSrc/Log.h>
#include <GearSrc/GL.h>
#include <GearSrc/SkyBox.h>
#include <GearSrc/Math.h>
#include <GearSrc/Model.h>

GSClient GSClientCreate(GSEngine engine) {
	GSClient client = malloc(sizeof(*client));

	memset(client, 0, sizeof(*client));

	client->engine = engine;

	client->camera[0] = 0;
	client->camera[1] = 0;
	client->camera[2] = 5;

	client->look_at[0] = 0;
	client->look_at[1] = 0;
	client->look_at[2] = 0;

	client->light0[0] = 2.5;
	client->light0[1] = 5;
	client->light0[2] = 5;
	client->light0[3] = 1;

	client->skybox_enabled = GSFalse;

	client->gl = GSGLCreate(client);

	client->skybox = GSSkyBoxTry(client, "default");

	GSLog(GSLogInfo, "Created client");

	return client;
}

void GSClientDestroy(GSClient client) {
	GSGLDestroy(client->gl);

	free(client);

	GSLog(GSLogInfo, "Destroyed client");
}

static void scene(GSClient client) {
	if(client->engine->param->render != NULL) client->engine->param->render(client->engine);
}

void GSClientStep(GSClient client) {
	static double r = 0;

	GSGLClear(client->gl);
	GSGLCameraSetup(client->gl);
	GSGLSetLight(client->gl);

	if(GSGLShadowBeforeMapping(client->gl)) {
		scene(client);
		GSGLShadowAfterMapping(client->gl);
	}

	if(client->skybox_enabled){
		if(client->skybox != NULL) GSSkyBoxDraw(client->skybox);
	}
	scene(client);
	GSGLShadowEnd(client->gl);

	client->engine->param->gl_swapbuffer();

	if(client->engine->param->after_render != NULL) client->engine->param->after_render(client->engine);
}

GSGL GSClientGetGL(GSClient client){
	return client->gl;
}

void GSClientToggleSkybox(GSClient client, GSBool toggle){
	client->skybox_enabled = toggle;
}
