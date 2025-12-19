#ifndef __AF_CLIENT_H__
#define __AF_CLIENT_H__

#include <glad/glad.h>
#include <Mw/Milsko.h>
#include <Mw/Widget/OpenGL.h>

#include <GL/glu.h>

#define AF_WAIT_MS 15

enum AF_SCENES {
	AF_SCENE_MAIN = 0,
	AF_SCENE_CONNECTING
};

/* main.c */
extern MwWidget root, window, opengl;

void scene_change(int scene);

/* gl.c */
typedef struct gl_scene {
	void (*changed)(void);
	void (*init)(void);
	void (*draw)(void);
} gl_scene_t;

extern int	  gl_scene;
extern gl_scene_t gl_scenes[];
extern double	  gl_cam_lr;
extern double	  gl_cam_ud;
extern double	  gl_cam_x;
extern double	  gl_cam_y;
extern double	  gl_cam_z;

void gl_init(void);
void gl_render(void);
void gl_resize(int width, int height);

/* gl_font.c */
void gl_font_init(void);
void gl_font_text(const char* text, int x, int y, double scale);
int  gl_font_width(const char* text);
int  gl_font_height(const char* text);

/* gl_main.c */
void gl_main_changed(void);
void gl_main_init(void);
void gl_main_draw(void);

/* gl_connecting.c */
void gl_connecting_changed(void);
void gl_connecting_init(void);
void gl_connecting_draw(void);

/* net.c */
void net_connect(const char* username, const char* hostname, int port);
void net_poll(void);

/* ui.c */
MwWidget ui_notice(int width, int height);
void	 ui_notice_destroy(MwWidget widget);
void	 ui_init(void);
void	 ui_scene(void);
MwWidget ui_message(const char* message);

#endif
