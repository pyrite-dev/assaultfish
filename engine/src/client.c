#include <GearSrc/Client.h>

#include <GearSrc/Log.h>
#include <GearSrc/GL.h>
#include <GearSrc/SkyBox.h>
#include <GearSrc/Math.h>
#include <GearSrc/Model.h>
#include <GearSrc/Version.h>

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

	if(GSGLTextureTry(client->gl, &client->font, &client->font_width, &client->font_height, "game:/font")){
	}else if(GSGLTextureTry(client->gl, &client->font, &client->font_width, &client->font_height, "base:/font")){
	}else{
		client->font = 0;
	}

	GSLog(GSLogInfo, "Created client");

	return client;
}

void GSClientDestroy(GSClient client) {
	if(client->font != 0) GSGLTextureDelete(client->gl, client->font);

	GSGLDestroy(client->gl);

	free(client);

	GSLog(GSLogInfo, "Destroyed client");
}

static void scene(GSClient client) {
	if(client->engine->param->render != NULL) client->engine->param->render(client->engine);
}

void GSClientStep(GSClient client) {
	int tw, th;
	int tw2, th2;
	int sw, sh;
	char buf[512];
	char buf2[512];
	GSVersion ver;
	GSVector4 white = {1, 1, 1, 1};
	GSVector4 half = {0, 0, 0, 0.5};
	GSVector2 sq[4];

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


	GSVersionGet(&ver);
	
	sprintf(buf, "GearSrc Engine %s", ver.string);
	
	tw = GSGLTextWidth(client->gl, buf);
	th = GSGLTextHeight(client->gl, buf);

	sprintf(buf2, "%s", ver.copyright);
	
	tw2 = GSGLTextWidth(client->gl, buf2);
	th2 = GSGLTextHeight(client->gl, buf2);

	sw = tw > tw2 ? tw : tw2;
	sh = th + th2;

	sq[0][0] = client->engine->width - sw, sq[0][1] = client->engine->height - sh;
	sq[1][0] = client->engine->width - sw, sq[1][1] = client->engine->height;
	sq[2][0] = client->engine->width, sq[2][1] = client->engine->height;
	sq[3][0] = client->engine->width, sq[3][1] = client->engine->height - sh;

	GSGLSetColor(client->gl, half);
	GSGLBegin2D(client->gl);
	GSGLPolygon2D(client->gl, 4, sq, NULL);
	GSGLEnd2D(client->gl);

	GSGLSetColor(client->gl, white);
	GSGLText(client->gl, client->engine->width - tw, client->engine->height - th * 2, buf);
	GSGLText(client->gl, client->engine->width - tw2, client->engine->height - th2, buf2);
	
	client->engine->param->gl_swapbuffer();

	if(client->engine->param->after_render != NULL) client->engine->param->after_render(client->engine);
}

GSGL GSClientGetGL(GSClient client){
	return client->gl;
}

void GSClientToggleSkybox(GSClient client, GSBool toggle){
	client->skybox_enabled = toggle;
}
