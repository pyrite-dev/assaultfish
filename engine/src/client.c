#include <GearSrc/Client.h>

#include <GearSrc/Log.h>
#include <GearSrc/GL.h>
#include <GearSrc/SkyBox.h>
#include <GearSrc/Math.h>
#include <GearSrc/Model.h>
#include <GearSrc/Version.h>

GSClient GSClientCreate(GSEngine engine) {
	GSClient client = malloc(sizeof(*client));
	void*	 ptr[]	= {
		"game:/font/normal",	      /**/
		"base:/font/normal",	      /**/
		&client->font_normal,	      /**/
		&client->glyph_normal_width,  /**/
		&client->glyph_normal_height, /**/
		&client->font_normal_width,   /**/
		&client->font_normal_height,  /**/
		/**/
		"game:/font/bold",	    /**/
		"base:/font/bold",	    /**/
		&client->font_bold,	    /**/
		&client->glyph_bold_width,  /**/
		&client->glyph_bold_height, /**/
		&client->font_bold_width,   /**/
		&client->font_bold_height   /**/
	    };
	int i;

	memset(client, 0, sizeof(*client));

	client->engine = engine;

	client->camera[0] = 0;
	client->camera[1] = 1.5;
	client->camera[2] = 5;

	client->look_at[0] = 0;
	client->look_at[1] = 1;
	client->look_at[2] = 5 - 1;

	client->light0[0] = 2.5;
	client->light0[1] = 5;
	client->light0[2] = 5;
	client->light0[3] = 0;

	client->skybox_enabled = GSFalse;

	client->gl = GSGLCreate(client);

	client->skybox = GSSkyBoxTry(client, "default");

	GSGLTextBold(client->gl, 0);

	for(i = 0; i < sizeof(ptr) / sizeof(ptr[0]); i += 7) {
		if(GSGLTextureTryEx(client->gl, ptr[i + 2], ptr[i + 3], ptr[i + 4], ptr[i], 0)) {
		} else if(GSGLTextureTryEx(client->gl, ptr[i + 2], ptr[i + 3], ptr[i + 4], ptr[i + 1], 0)) {
		} else {
			*((GLuint*)ptr[i + 2]) = 0;
		}

		if(*((GLuint*)ptr[i + 2]) > 0) {
			*((int*)ptr[i + 5]) = GSMathClosestPOT(*((int*)ptr[i + 3]));
			*((int*)ptr[i + 6]) = GSMathClosestPOT(*((int*)ptr[i + 4]));
			*((int*)ptr[i + 3]) /= 16;
			*((int*)ptr[i + 4]) /= 16;
		}
	}

	GSLog(GSLogInfo, "Created client");

	return client;
}

void GSClientDestroy(GSClient client) {
	if(client->font_normal != 0) GSGLTextureDelete(client->gl, client->font_normal);
	if(client->font_bold != 0) GSGLTextureDelete(client->gl, client->font_bold);

	GSGLDestroy(client->gl);

	free(client);

	GSLog(GSLogInfo, "Destroyed client");
}

static void scene(GSClient client) {
	if(client->engine->param->render != NULL) client->engine->param->render(client->engine);
}

void GSClientStep(GSClient client) {
	int	  tw, th;
	int	  tw2, th2;
	int	  sw, sh;
	char	  buf[512];
	char	  buf2[512];
	GSVersion ver;
	GSVector4 white	 = {1, 1, 1, 1};
	GSVector4 yellow = {1, 1, 0, 1};
	GSVector4 half	 = {0, 0, 0, 0.5};
	GSVector2 sq[4];

	GSGLClear(client->gl);
	GSGLCameraSetup(client->gl);
	GSGLSetLight(client->gl);

	GSGLTextBold(client->gl, 0);

	if(GSGLShadowBeforeMapping(client->gl)) {
		scene(client);
		GSGLShadowAfterMapping(client->gl);
	}

	if(client->skybox_enabled) {
		if(client->skybox != NULL) GSSkyBoxDraw(client->skybox);
	}
	scene(client);
	GSGLShadowEnd(client->gl);

	sprintf(buf, "%.1f FPS", client->engine->tps_sampled);
	sprintf(buf2, "%s%s", client->engine->tps_sampled < 10 ? " " : "", buf);

	tw = GSGLTextWidth(client->gl, buf2);
	th = GSGLTextHeight(client->gl, buf2);

	sq[0][0] = client->engine->width - tw, sq[0][1] = 0;
	sq[1][0] = client->engine->width - tw, sq[1][1] = th;
	sq[2][0] = client->engine->width, sq[2][1] = th;
	sq[3][0] = client->engine->width, sq[3][1] = 0;

	GSGLSetColor(client->gl, half);
	GSGLBegin2D(client->gl);
	GSGLPolygon2D(client->gl, 4, sq, NULL);
	GSGLEnd2D(client->gl);

	GSGLSetColor(client->gl, yellow);
	GSGLTextBold(client->gl, 1);
	GSGLText(client->gl, client->engine->width - tw, 0, buf2);
	GSGLTextBold(client->gl, 0);

	GSVersionGet(&ver);

	sprintf(buf, "Engine version %s", ver.string);

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

	static double r = 0;

	r += 0.1;
}

GSGL GSClientGetGL(GSClient client) {
	return client->gl;
}

void GSClientToggleSkybox(GSClient client, GSBool toggle) {
	client->skybox_enabled = toggle;
}
