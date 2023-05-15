#include <SDL_opengl.h>
#include <glib.h>

#include "scs.h"
#include "shared.h"
#include "ui.h"
#include "sys_gl.h"

static gboolean
ui_widget_checkbox_draw(widget_t *w, GError **err)
{
    widget_checkbox_t *wcb;
    widget_t *r;
    widget_root_t *wr;
    real top, bot, left, right;
    char *key;
    vec3_t *v;

    assert(w != NULL);
    wcb = w->data;
    assert(wcb != NULL);

    left  = w->rpos[X];
    right = w->rpos[X] + w->size[WIDTH];
    bot   = w->rpos[Y];
    top   = w->rpos[Y] + w->size[HEIGHT];

    v = g_hash_table_lookup(wcb->colors, "background");
    glColor3v(*v);

    glBegin(GL_TRIANGLES);
    	glVertex2f(left,  bot);
	glVertex2f(right, top);
	glVertex2f(left,  top);

    	glVertex2f(left,  bot);
	glVertex2f(right, bot);
	glVertex2f(right, top);
    glEnd();

    r = ui_widget_get_root(w);
    wr = r->data;

    if (wr->active == w)
	key = "border_highlight";
    else
	key = "border";

    v = g_hash_table_lookup(wcb->colors, key);
    glColor3v(*v);
    glLineWidth(1.0);
    glBegin(GL_LINES);
    	glVertex2f(left,  bot);
	glVertex2f(right, bot);

	glVertex2f(right, bot);
	glVertex2f(right, top);

	glVertex2f(right, top);
	glVertex2f(left,  top);

	glVertex2f(left,  top);
    	glVertex2f(left,  bot);

    glEnd();

    /* draw an X if checked */
    if (wcb->checked) {
	v = g_hash_table_lookup(wcb->colors, "x");
	glColor3v(*v);
	glLineWidth(3.0);
	glBegin(GL_LINES);
	    glVertex2f(left+3,  top-3);
	    glVertex2f(right-3,  bot+3);

	    glVertex2f(left+3,  bot+3);
	    glVertex2f(right-3,  top-3);
        glEnd();
    }

    return OK;
}

static gboolean
ui_widget_checkbox_update(widget_t *w, GError **err)
{
    return OK;
}

static gboolean
ui_widget_checkbox_mbutton(widget_t *w, SDL_MouseButtonEvent *ev, widget_t **handled_by, GError **err)
{
    widget_checkbox_t *wcb = w->data;
    assert(wcb != NULL);

    if (ev->type == SDL_MOUSEBUTTONUP) {
	wcb->checked ^= 1;
	*handled_by = w;
    }

    return OK;
}

static gboolean
ui_widget_checkbox_destroy(widget_t *w, GError **err)
{
    widget_checkbox_t *wcb = w->data;
    assert(wcb != NULL);
    g_hash_table_destroy(wcb->colors);
    return OK;
}

gboolean
ui_widget_checkbox_get_checked(widget_t *w, GError **err)
{
    widget_checkbox_t *wcb;

    assert(w != NULL);
    wcb = w->data;
    assert(wcb != NULL);

    return wcb->checked;
}

void
ui_widget_checkbox_set_color(widget_t *w, char *key, vec3_t color)
{
    widget_button_t *wcb = w->data;
    vec3_t *v;

    assert(wcb != NULL);

    v = g_hash_table_lookup(wcb->colors, key);
    if (v == NULL) {
	printerr("no color: %s\n", key);
	return;
    }
    vec3_cp(color, *v);
}

widget_t *
ui_widget_checkbox_new(widget_t *parent, gboolean checked,
			widget_posv_t pos, widget_sizev_t size)
{
    widget_t *w;
    widget_checkbox_t *wcb;
    GHashTable *t;

    /* create checkbox */
    w = ui_widget_new(parent, UI_WIDGET_CHECKBOX, NULL);

    /* default event handlers */
    w->mbutton = &ui_widget_checkbox_mbutton;
    w->destroy = &ui_widget_checkbox_destroy;
    w->update  = &ui_widget_checkbox_update;
    w->draw    = &ui_widget_checkbox_draw;

    w->set_pos(w, pos, NULL);
    w->set_size(w, size, NULL);
    wcb = w->data;
    wcb->checked = checked;

    /* set up colors */
    wcb->colors = g_hash_table_new_full(g_str_hash, g_str_equal,
	    NULL, (void (*)(gpointer))vec3_destroy);
    t = wcb->colors;

    g_hash_table_insert(t, "background",           vec3_new(0.0,  0.03, 0.25));
    g_hash_table_insert(t, "border",               vec3_new(0.6,  0.6,  0.6));
    g_hash_table_insert(t, "border_highlight",     vec3_new(0,  1,  0));
    g_hash_table_insert(t, "x",                    vec3_new(1,  1,  1));

    return w;
}
