/* 
 * Name: cl_newton.c
 * 
 * Description: Newtonian Gravity Simulation Demo
 *
 * Copyright: (c) 2011 Jason Hall <jbhall78@gmail.com>
 *
 */
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <math.h>

#include <glib.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include "scs.h"
#include "shared.h"
#include "client.h"
#include "sys_gl.h"

#include "config.h"

#include "mesh.h"
#include "sys_gl.h"

static widget_t *root = NULL;
static widget_sizev_t ScreenSize;

static void
demo_exit(widget_t *w, void *data)
{
    cl_mode(&menu_callbacks, NULL);
}


static gboolean
demo_key(SDL_KeyboardEvent *k, GError **err)
{
    GError *tmp = NULL;
    gboolean handled = FALSE;
    int ret;

    if ((ret = ui_key(root, k, &handled, &tmp)) != OK) {
        g_propagate_error(err, tmp);
        return ret;
    }

    if (handled == TRUE)
	return OK;

    if (k->type == SDL_KEYUP)
	return OK;

    switch (k->keysym.sym) {
	case SDLK_ESCAPE:
	    demo_exit(NULL, NULL);
	    break;
	default:
	    break;
    }

    return OK;
}

static gboolean
demo_mbutton(SDL_MouseButtonEvent *b, GError **err)
{
    return ui_mbutton(root, b, err);
}

static gboolean
demo_mmotion(SDL_MouseMotionEvent *m, GError **err)
{
    return ui_mmotion(root, m, err);
}

static gboolean
demo_update(GError **err)
{
    return ui_update(root, err);
}

static void
demo_draw_grid(void)
{
    vec3_t color;

    color[R] = 0.4;
    color[G] = 0.4;
    color[B] = 0.4;

    glDisable(GL_POINT_SMOOTH);
    glLineWidth(1.0);
    glPointSize(1.0);
    glColor3v(color);

#if 0 /* opengl line stipple sucks, it can't handle patterns which arent divisible by 2 */
//    uint16_t pattern = 43690; /* 1010101010101010*/
//    uint16_t pattern = 4369; /* 0001000100010001 */
//    uint16_t pattern = 257; /* 0001000100010001 */
//    uint16_t pattern = 33825; /* 0001000100010001 */
    uint16_t pattern = 13107;

    glLineStipple(0,pattern);
    glEnable(GL_LINE_STIPPLE);
    glBegin(GL_LINES);
    x = 0;
    for (y = 0; y < ScreenSize[HEIGHT]; y += 50) {
        glVertex2f(x, y);
        glVertex2f(ScreenSize[WIDTH], y);
    }
    y = 0;
    for (x = 0; x < ScreenSize[WIDTH]; x += 50) {
        glVertex2f(x, y);
        glVertex2f(x, ScreenSize[HEIGHT]);
    }
    glEnd();
    glDisable(GL_LINE_STIPPLE);
#endif

    real x, y, w, h, grid = 30, stipple = 5;
    w = ScreenSize[WIDTH];
    h = ScreenSize[HEIGHT];

    glBegin(GL_POINTS);
    for (x = 0; x < w; x += grid) {
        for (y = 0; y < h; y += stipple) {
            glVertex2f(x, y);
        }
    }
    for (y = 0; y < h; y += grid) {
        for (x = 0; x < w; x += stipple) {
            glVertex2f(x, y);
        }
    }
    glEnd();

    vec3_t axis;
    axis[X] = ScreenSize[WIDTH] / 2;
    axis[Y] = ScreenSize[HEIGHT] / 2;
    axis[Z] = 0;

    ui_draw_axis(axis, ScreenSize[WIDTH]);
}

static gboolean
demo_draw(GError **err)
{
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glDisable(GL_DEPTH_TEST);
    lighting_disable();

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
	glLoadIdentity();

	glOrtho(0.0, ScreenSize[WIDTH],
		0.0, ScreenSize[HEIGHT],
		-1.0, 100.0);

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	    glLoadIdentity();

	    // draw background grid
	    demo_draw_grid();

	    // clear the depth buffer so the grid just appears to float
	    // in the background and not intersect with other objects
    	    glClear(GL_DEPTH_BUFFER_BIT);
	    glEnable(GL_DEPTH_TEST);
	    lighting_enable();

	    vec3_t color_long, color_lat;
	    vec3_set(color_long, 0.25, 0, 0);
	    vec3_set(color_lat,  0.25, 0, 0);
	    glPushMatrix();
	    glTranslatef(ScreenSize[X]/2, ScreenSize[Y]/2, -125);
	    ui_draw_earth(ScreenSize[X]/2, ScreenSize[Y]/2, -125, 125, 2, 20.0, 20.0, color_long, color_lat);
	    glPopMatrix();

	    // rinse, repeat
    	    glClear(GL_DEPTH_BUFFER_BIT);
	    glDisable(GL_DEPTH_TEST);

	    lighting_disable();

	    // draw user interface
	    ui_draw(root, NULL);

	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    return OK;
}

static gboolean
demo_ui_load(GError **err)
{
    GError *tmp = NULL;
    tex_t *tex;
    widget_t *w;
    widget_posv_t pos;
    widget_sizev_t size;
    int step = 32;

    vec2_zero(pos);

    // initialize user interface subsystem
    root = ui_init(pos, ScreenSize, NULL);

    // configure mouse cursor
    tex = tex_load("cursor.png");
    ui_widget_root_set_cursor(root, tex);

    // draw a button
    vec2_set(pos, step, (1 * step));
    vec2_set(size, 150, step);
    w = ui_widget_button_new(root, "Back", pos, size, demo_exit);

    // texture select
    int height = 20;

    vec2_set(size, 200, height);
    vec2_set(pos, ScreenSize[WIDTH] - size[WIDTH] - height, ScreenSize[HEIGHT] - (2 * height));
    w = ui_widget_select_new(root, pos, size);

    GList *files = getdatafilelist(FILE_TEXTURE, "earth", FALSE, &tmp);
    if (files == NULL) {
	g_propagate_error(err, tmp);
	return FAIL;
    }
    GList *file;

    for (file = files; file; file = file->next) {
	old_file_t *f = file->data;
	ui_widget_select_append(w, f->name, file->data);
//	print("adding: %s\n", f->name);
    }

    return OK;
}

static gboolean
demo_load(GError **err)
{
    GError *tmp = NULL;

    // make SDL happy
    SDL_EnableKeyRepeat(250, 30);
    SDL_WarpMouse(0, 0);

    // set our screen size
    vec2_cp(client.res, ScreenSize);

//    tex_test = tex_load("testmap.png");
//    tex_test = tex_load("earth/lores.png");
//    tex_test = tex_load("earth/earth.png");

    if (! demo_ui_load(&tmp)) {
	g_propagate_error(err, tmp);
	return FAIL;
    }

    lighting_init();

    client.initialized = TRUE;

    return OK;
}

static gboolean
demo_unload(GError **err)
{
    SDL_EnableKeyRepeat(0,0);

    client.initialized = FALSE;

    return OK;
}

cl_callbacks_t globe_demo_callbacks = { 
    demo_load,
    demo_unload,
    demo_update,
    demo_draw,
    demo_key,
    demo_mbutton,
    demo_mmotion,
    NULL, /* .jbutton */
    NULL, /* .jmotion */
    NULL, /* .jhat */
};
