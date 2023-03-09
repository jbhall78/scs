#include <glib.h>

#include <SDL.h>
#include <SDL_opengl.h>

#include "scs.h"
#include "ui.h"
#include "client.h"
#include "sys_gl.h"

static gboolean
ui_widget_root_screen_resize(widget_t *root, int32_t width, int32_t height, GError **err)
{
    GError *tmp = NULL;
    widget_sizev_t size;
    gboolean ret;

    size[WIDTH]  = width;
    size[HEIGHT] = height;

    if ((ret = root->set_size(root, size, &tmp)) != OK) {
	g_propagate_error(err, tmp);
	return ret;
    }

    return OK;
}

static gboolean
ui_widget_root_update(widget_t *w, GError **err)
{
    widget_root_t *wr = w->data;
    widget_t *active;

    assert(wr != NULL);

    /* update the mouse cursor position */
    if (! client.console.enabled) {
        SDL_GetMouseState((int *)&wr->mpos[X], (int *)&wr->mpos[Y]);
        wr->mpos[Y] = w->size[HEIGHT] - wr->mpos[Y];
    }
//    print("[0x%08d] mouse position: %d %d\n", w, wr->mpos[X], wr->mpos[Y]);

    /* update the active widget */
    if ((active = ui_widget_get_active(wr->mpos, w)) != NULL)
        wr->active = active;

#if 0
    if (wr->active && wr->active->type == UI_WIDGET_SELECT)
    {
	print("asdasfas\n");
    }
    if (wr->active == NULL) {
	print("wtf no active\n");
    }
#endif

    return OK;
}

widget_t *
ui_widget_root_new(widget_posv_t pos, widget_sizev_t size)
{
    widget_t *root;
    widget_root_t *wr;

    root = ui_widget_new(NULL, UI_WIDGET_ROOT, NULL);
    wr = root->data;

    root->set_pos(root, pos, NULL);
    root->set_size(root, size, NULL);

    root->update = &ui_widget_root_update;
    root->screen_resize = &ui_widget_root_screen_resize;

    return root;
}

