#include <GearSrc/Client.h>

#include <GearSrc/Log.h>
#include <GearSrc/GL.h>
#include <GearSrc/SkyBox.h>
#include <GearSrc/Math.h>
#include <GearSrc/Model.h>
#include <GearSrc/Version.h>
#include <GearSrc/SoundEngine.h>

#include <stb_ds.h>

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
	client->look_at[2] = 0;

	client->light0[0] = 2.5;
	client->light0[1] = 5;
	client->light0[2] = 5;
	client->light0[3] = 0;

	client->skybox_enabled = GSFalse;

	if((client->sengine = GSSoundEngineCreate(client)) == NULL) {
		GSLog(engine, GSLogError, "Failed to create sound engine");
		GSClientDestroy(client);

		return NULL;
	}

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

	GSLog(engine, GSLogInfo, "Created client");

	return client;
}

void GSClientDestroy(GSClient client) {
	if(client->font_normal != 0) GSGLTextureDelete(client->gl, client->font_normal);
	if(client->font_bold != 0) GSGLTextureDelete(client->gl, client->font_bold);

	GSGLDestroy(client->gl);

	if(client->sengine != NULL) GSSoundEngineDestroy(client->sengine);

	GSLog(client->engine, GSLogInfo, "Destroyed client");

	free(client);
}

static GSVector4 white = {1, 1, 1, 1};

static void scene(GSClient client) {
	GSGLSetColor(client->gl, white);

	if(client->engine->param->render != NULL) client->engine->param->render(client->engine);

	GSGLSetColor(client->gl, white);
}

void GSClientStep(GSClient client) {
	int	  tw, th;
	int	  tw2, th2;
	int	  sw, sh;
	char	  buf[512];
	char	  buf2[512];
	GSVersion ver;
	GSVector4 yellow = {1, 1, 0, 1};
	GSVector4 half	 = {0, 0, 0, 0.5};
	GSVector2 sq[4];
	int	  i, j, k, c, t;

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
	sq[1][0] = sq[0][0], sq[1][1] = client->engine->height;
	sq[2][0] = client->engine->width, sq[2][1] = sq[1][1];
	sq[3][0] = sq[2][0], sq[3][1] = sq[0][1];

	GSGLSetColor(client->gl, half);
	GSGLBegin2D(client->gl);
	GSGLPolygon2D(client->gl, 4, sq, NULL);
	GSGLEnd2D(client->gl);

	GSGLSetColor(client->gl, white);
	GSGLText(client->gl, client->engine->width - tw, client->engine->height - th * 2, buf);
	GSGLText(client->gl, client->engine->width - tw2, client->engine->height - th2, buf2);

	for(k = 0; k < 2; k++) {
		c = 0;
		for(i = arrlen(client->engine->log) - 1, j = 0; i >= 0 && j < 5; i--, j++) {
			if(client->engine->param->get_tick != NULL) {
				if((client->engine->param->get_tick() - client->engine->log[i].tick) > 3000) continue;
			}

			if(k == 1) {
				GSNumber  y = (t - c - 1) * GSGLTextHeight(client->gl, "M");
				GSNumber  w = 0;
				char	  str[6];
				int	  level = client->engine->log[i].level;
				GSVector4 col	= {0, 0, 0, 1};

				str[0] = 0;

				if(level == GSLogInfo) {
					strcpy(str, "INFO");

					col[0] = col[1] = col[2] = 1;
				} else if(level == GSLogWarn) {
					strcpy(str, "WARN");

					col[0] = col[2] = 1;
				} else if(level == GSLogError) {
					strcpy(str, "ERROR");

					col[0] = 1;
				} else if(level == GSLogDebug) {
					strcpy(str, "DEBUG");

					col[2] = 1;
				}
				GSGLSetColor(client->gl, col);

				GSGLTextBold(client->gl, 1);
				GSGLText(client->gl, (5 - strlen(str)) * GSGLTextWidth(client->gl, "M") / 2, y, str);
				GSGLTextBold(client->gl, 0);

				GSGLSetColor(client->gl, white);
				GSGLText(client->gl, 0, y, "     |");
				GSGLText(client->gl, GSGLTextWidth(client->gl, "M") * 6, y, client->engine->log[i].log);
			}

			c++;
		}

		t = c;

		if(k == 0) {
			sq[0][0] = 0, sq[0][1] = 0;
			sq[1][0] = 0, sq[1][1] = c * GSGLTextHeight(client->gl, "M");
			sq[2][0] = client->engine->width, sq[2][1] = sq[1][1];
			sq[3][0] = sq[2][0], sq[3][1] = 0;

			GSGLSetColor(client->gl, half);
			GSGLBegin2D(client->gl);
			GSGLPolygon2D(client->gl, 4, sq, NULL);
			GSGLEnd2D(client->gl);
		}
	}

	sprintf(buf, "%.1f FPS", client->engine->tps_sampled);
	sprintf(buf2, "%s%s", client->engine->tps_sampled < 10 ? " " : "", buf);

	tw = GSGLTextWidth(client->gl, buf2);
	th = GSGLTextHeight(client->gl, buf2);

	sq[0][0] = client->engine->width - tw, sq[0][1] = 0;
	sq[1][0] = sq[0][0], sq[1][1] = th;
	sq[2][0] = client->engine->width, sq[2][1] = sq[1][1];
	sq[3][0] = sq[2][0], sq[3][1] = 0;

	GSGLSetColor(client->gl, half);
	GSGLBegin2D(client->gl);
	GSGLPolygon2D(client->gl, 4, sq, NULL);
	GSGLEnd2D(client->gl);

	GSGLSetColor(client->gl, yellow);
	GSGLTextBold(client->gl, 1);
	GSGLText(client->gl, client->engine->width - tw, 0, buf2);
	GSGLTextBold(client->gl, 0);

	client->engine->param->gl_swapbuffer();

	if(client->engine->param->after_render != NULL) client->engine->param->after_render(client->engine);
}

GSGL GSClientGetGL(GSClient client) {
	return client->gl;
}

GSSoundEngine GSClientGetSoundEngine(GSClient client) {
	return client->sengine;
}

void GSClientSetSkybox(GSClient client, GSBool toggle) {
	client->skybox_enabled = toggle;
}
