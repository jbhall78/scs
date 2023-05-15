#include <SDL_opengl.h>
#include <glib.h>

#include "scs.h"
#include "shared.h"
#include "ui.h"
#include "sys_gl.h"

static gboolean
ui_widget_skel_draw(widget_t *w, GError **err)
{
    widget_skel_t *ws = w->data;

    return OK;
}

static gboolean
ui_widget_skel_update(widget_t *w, GError **err)
{
    widget_skel_t *ws = w->data;

    return OK;
}

static gboolean
ui_widget_skel_mbutton(widget_t *w, SDL_MouseButtonEvent *ev, GError **err)
{
    widget_skel_t *ws = w->data;
    assert(ws != NULL);

    return OK;
}

static gboolean
ui_widget_skel_destroy(widget_t *w, GError **err)
{
    widget_skel_t *ws = w->data;
    assert(ws != NULL);
    return OK;
}

widget_t *
ui_widget_skel_new(widget_t *parent, widget_posv_t pos, widget_sizev_t size)
{
    widget_t *w;
    widget_skel_t *ws;

    /* create skel */
    w = ui_widget_new(parent, UI_WIDGET_SKEL, NULL);

    /* default event handlers */
    w->mbutton  = &ui_widget_skel_mbutton;
    w->destroy  = &ui_widget_skel_destroy;
    w->update   = &ui_widget_skel_update;
    w->draw     = &ui_widget_skel_draw;

    w->set_size(w, size, NULL);
    w->set_pos(w, pos, NULL);

    /* default data */
    ws = w->data;

    return w;
}
