#include <string.h>

#include <glib.h>
#include <SDL_opengl.h>

#include "scs.h"
#include "shared.h"
#include "ui.h"
#include "sys_gl.h"

static gboolean
ui_widget_text_draw(widget_t *w, GError **err)
{
    widget_text_t *wt;
    widget_t *r;
    widget_root_t *wr;
    real top, bot, left, right;
    char *key;
    vec3_t *v;

    assert(w != NULL);
    wt = w->data;
    assert(wt != NULL);

    left  = w->rpos[X];
    right = w->rpos[X] + w->size[WIDTH];
    bot   = w->rpos[Y];
    top   = w->rpos[Y] + w->size[HEIGHT];

    v = g_hash_table_lookup(wt->colors, "background");
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

    v = g_hash_table_lookup(wt->colors, key);
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

    /* now draw text */
    v = g_hash_table_lookup(wt->colors, "text");
    glColor3v(*v);
    ui_font_draw_text(wt->font, left+3, bot+3, 8, *v, "%s", wt->buf->data);

    return OK;
}

static gboolean
ui_widget_text_update(widget_t *w, GError **err)
{
    return OK;
}

static gboolean
ui_widget_text_destroy(widget_t *w, GError **err)
{
    widget_text_t *wt = w->data;
    assert(wt != NULL);
    g_hash_table_destroy(wt->colors);
    g_array_free(wt->buf, TRUE);
    return OK;
}

char *
ui_widget_text_get_text(widget_t *w)
{
    widget_text_t *wt;

    assert(w != NULL);
    wt = w->data;
    assert(wt != NULL);

    if (! wt->buf->len)
	return NULL;
    else
	return strdup(wt->buf->data);
}

void
ui_widget_text_set_text(widget_t *w, char *txt)
{
    widget_text_t *wt;

    assert(w != NULL);
    wt = w->data;
    assert(wt != NULL);

    assert(txt != NULL);

    /* clear out current text buffer */
    if (wt->buf->len > 0)
	g_array_remove_range(wt->buf, 0, wt->buf->len);

    /* insert text into buffer */
    g_array_insert_vals(wt->buf, 0, txt, strlen(txt));

    wt->pos = wt->buf->len;
}

void
ui_widget_text_set_color(widget_t *w, char *key, vec3_t color)
{
    widget_button_t *wt = w->data;
    vec3_t *v;

    assert(wt != NULL);

    v = g_hash_table_lookup(wt->colors, key);
    if (v == NULL) {
	printerr("no color: %s\n", key);
	return;
    }
    vec3_cp(color, *v);
}

static void
ui_text_proc_key(widget_t *w, int32_t keysym, int32_t mod, uint16_t unicode)
{
    widget_text_t *wt;
    uint32_t key;

    assert(w != NULL);
    wt = w->data;
    assert(wt != NULL);

    switch (keysym) {
	case SDLK_BACKSPACE:
	    if (wt->pos)
		g_array_remove_index(wt->buf, --wt->pos);
	    break;
	default:
	    // map unicode to ascii
	    if ((unicode & 0xFF80) == 0) {
		key = unicode & 0x7F;
		// make sure it is valid for input
		if (key >= wt->font->range[LO] && key <= wt->font->range[HI]) {
		    g_array_append_val(wt->buf, key);
		    wt->pos++;
		}
	    }
	    break;
    }
}

gboolean
ui_widget_text_key(widget_t *w, SDL_KeyboardEvent *k, gboolean *handled, GError **err)
{

    if (k->type == SDL_KEYUP)
	return OK;

    //ui_text_proc_key(w, k->keysym.sym, k->keysym.mod, k->keysym.unicode);

    *handled = TRUE;

    return OK;
}

widget_t *
ui_widget_text_new(widget_t *parent, widget_posv_t pos, widget_sizev_t size)
{
    widget_t *w;
    widget_text_t *wt;
    GHashTable *t;
    font_t *fnt = ui_font_get("small");

    /* create text widget */
    w = ui_widget_new(parent, UI_WIDGET_TEXT, NULL);

    /* default event handlers */
    w->destroy = &ui_widget_text_destroy;
    w->update  = &ui_widget_text_update;
    w->draw    = &ui_widget_text_draw;
    w->key     = &ui_widget_text_key;

    w->set_pos(w, pos, NULL);
    w->set_size(w, size, NULL);
    wt = w->data;
    wt->font = fnt;
    wt->buf = g_array_new(TRUE, TRUE, sizeof(char));

    /* set up colors */
    wt->colors = g_hash_table_new_full(g_str_hash, g_str_equal,
	    NULL, (void (*)(gpointer))vec3_destroy);
    t = wt->colors;

    g_hash_table_insert(t, "background",           vec3_new(0.0,  0.03, 0.25));
    g_hash_table_insert(t, "border",               vec3_new(0.6,  0.6,  0.6));
    g_hash_table_insert(t, "border_highlight",     vec3_new(0,  1,  0));
    g_hash_table_insert(t, "text",                 vec3_new(1,  1,  1));

    return w;
}
