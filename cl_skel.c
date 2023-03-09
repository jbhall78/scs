#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <glib.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include "scs.h"
#include "shared.h"
#include "client.h"
#include "sys_gl.h"

#include "config.h"

void
menu_key(SDL_KeyboardEvent *k)
{
    if (k->type == SDL_KEYUP)
	return;

    switch (k->keysym.sym) {
	case SDLK_ESCAPE:
    	    cl_shutdown(0);
	    break;
	default:
	    break;
    }
}

static void
menu_mbutton(SDL_MouseButtonEvent *b)
{
    return;
}

static void
menu_mmotion(SDL_MouseMotionEvent *m)
{
    return;
}

static void
menu_update(void)
{
    return;
}

static void
menu_draw(void)
{
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
	glLoadIdentity();

	/* we want everything to appear the same size in all resolutions 
	   so we dont use our screen size here. */
	glOrtho(0.0, client.ortho[WIDTH],
		0.0, client.ortho[HEIGHT],
		-1.0, 100.0);

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	    glLoadIdentity();

	    // draw stuff
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}

static gboolean
menu_load(GError **err)
{
    // make SDL happy
    SDL_EnableKeyRepeat(250, 30);
    SDL_WarpMouse(0, 0);

    // make OpenGL happy
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH);

    client.initialized = TRUE;

    return OK;
}

static gboolean
menu_unload(GError **err)
{
    SDL_EnableKeyRepeat(0,0);

    client.initialized = FALSE;

    return OK;
}

cl_callbacks_t menu_callbacks = { 
    menu_load,
    menu_unload,
    menu_update,
    menu_draw,
    menu_key,
    menu_mbutton,
    menu_mmotion,
    NULL, /* .jbutton */
    NULL, /* .jmotion */
    NULL, /* .jhat */
};
