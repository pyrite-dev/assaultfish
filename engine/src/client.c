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
	client->camera[1] = 1;
	client->camera[2] = 5;

	client->look_at[0] = 0;
	client->look_at[1] = 0;
	client->look_at[2] = 0;

	client->light0[0] = 2.5;
	client->light0[1] = 5;
	client->light0[2] = 5;
	client->light0[3] = 1;

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

GSModel m = NULL;

static void scene(GSClient client) {
	static double r	  = 0;
	GSVector3     pos = {0, 0, 0};
	GSVector3     rot = {0, 0, 0};
	int	      i;
	int	      a = 1;

	if(m == NULL) m = GSModelOpen(client->engine, "game:/mdl/tetopear.mdl");

	rot[1] = r;
	for(i = -a; i <= a; i++) {
		int j;

		pos[0] = i * 2;
		for(j = -a; j <= a; j++) {
			pos[2] = j * 2;
			GSModelDraw(m, pos, rot);
		}
	}
	r++;
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

	if(client->skybox != NULL) GSSkyBoxDraw(client->skybox);
	scene(client);
	GSGLShadowEnd(client->gl);

	client->engine->param->gl_swapbuffer();

	client->look_at[0] = 5 + cos(r) * 5;
	client->look_at[2] = 5 + sin(r) * 5;

	client->look_at[0] = 0;
	client->look_at[2] = 0;

	r += 0.1;
}
