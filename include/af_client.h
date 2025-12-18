#ifndef __AF_CLIENT_H__
#define __AF_CLIENT_H__

#include <glad/glad.h>
#include <Mw/Milsko.h>
#include <Mw/Widget/OpenGL.h>

/* main.c */
extern MwWidget root, window, opengl;

/* gl.c */
void gl_init(void);
void gl_render(void);

/* ui.c */
MwWidget ui_notice(int width, int height);
void ui_notice_destroy(MwWidget widget);
void ui_init(void);

#endif
