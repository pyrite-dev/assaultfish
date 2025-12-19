#include <af_common.h>

MwWidget ui_notice(int width, int height) {
	if(width == 0 && height == 0) {
		width  = 400;
		height = 300;
	}

	return MwVaCreateWidget(MwFrameClass, "notice", window, (MwGetInteger(window, MwNwidth) - width) / 2, (MwGetInteger(window, MwNheight) - height) / 2, width, height,
				MwNhasBorder, 1,
				MwNinverted, 0,
				NULL);
}

void ui_notice_destroy(MwWidget widget) {
	MwDestroyWidget(widget);
}

static void ui_login_join(MwWidget handle, void* user, void* client) {
	MwLLDestroyPixmap(MwGetVoid(user, MwNpixmap));
	ui_notice_destroy(user);
}

static void ui_login(void) {
	MwWidget    wnd	 = ui_notice(0, 0);
	int	    bw	 = MwDefaultBorderWidth(wnd);
	MwWidget    join = MwVaCreateWidget(MwButtonClass, "join", wnd, MwGetInteger(wnd, MwNwidth) - bw - 10 - 96, MwGetInteger(wnd, MwNheight) - bw - 10 - 24, 96, 24,
					    MwNtext, "Join",
					    NULL);
	MwWidget    box	 = MwVaCreateWidget(MwBoxClass, "box", wnd, bw, bw, MwGetInteger(wnd, MwNwidth) - bw * 2, MwGetInteger(wnd, MwNheight) - bw * 2 - 10 - 24,
					    MwNorientation, MwVERTICAL,
					    MwNmargin, 10,
					    MwNpadding, 10,
					    NULL);
	MwWidget    icon = MwVaCreateWidget(MwImageClass, "icon", box, 0, 0, 0, 0,
					    MwNfillArea, 0,
					    MwNfixedSize, 100,
					    NULL);
	MwWidget    entrybox[3];
	MwLLPixmap  px	      = MwLoadImage(wnd, DATAROOTDIR "/assaultfish/logo.png");
	const char* labels[3] = {
	    "Username",
	    "Hostname",
	    "Port"};
	const char* entries[3] = {
	    "JohnDoe",
	    "127.0.0.1",
	    "",
	};
	int  i;
	char buf[16];

	sprintf(buf, "%d", AF_DEFAULT_PORT);

	entries[2] = buf;

	for(i = 0; i < 3; i++) {
		entrybox[i] = MwVaCreateWidget(MwBoxClass, "entrybox", box, 0, 0, 0, 0,
					       MwNfixedSize, 24,
					       MwNmargin, 10,
					       NULL);
		MwVaCreateWidget(MwLabelClass, "label", entrybox[i], 0, 0, 0, 0,
				 MwNfixedSize, 100,
				 MwNtext, labels[i],
				 MwNalignment, MwALIGNMENT_END,
				 NULL);
		MwVaCreateWidget(MwEntryClass, "entry", entrybox[i], 0, 0, 0, 0,
				 MwNtext, entries[i],
				 NULL);
	}

	MwVaApply(icon,
		  MwNpixmap, px,
		  NULL);

	MwVaApply(wnd,
		  MwNpixmap, px,
		  NULL);

	MwAddUserHandler(join, MwNactivateHandler, ui_login_join, wnd);
}

void ui_scene(void) {
	if(gl_scene == AF_SCENE_MAIN) {
		ui_login();
	}
}

void ui_init(void) {
	MwToggleDarkTheme(root, 1);
}
