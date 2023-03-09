#include <SDL_opengl.h>
#include <glib.h>

#include "scs.h"
#include "shared.h"
#include "ui.h"
#include "sys_gl.h"

static void
ui_widget_tooltip_recalculate_size(widget_t *w)
{
    assert(w != NULL);
    assert(w->type == UI_WIDGET_TOOLTIP);

    widget_tooltip_t *wt = w->data;
    assert(wt != NULL);

    if (wt->txt == NULL) {
	w->size[WIDTH]  = 40;
	w->size[HEIGHT] = 10;
    } else {
	w->size[WIDTH]  = ui_font_get_strwidth(wt->font, wt->font_size, strlen(wt->txt));
	w->size[HEIGHT] = wt->font->chrsize[HEIGHT]; 
    }
}

static gboolean
ui_widget_tooltip_update(widget_t *w, GError **err)
{
    assert(w != NULL);
    assert(w->type == UI_WIDGET_TOOLTIP);

    widget_tooltip_t *wt = w->data;
    assert(wt != NULL);

    if (w->visible == TRUE)
	return OK;

    gboolean ready;

    if ((ready = sys_timer_is_ready(wt->timer)) == TRUE) {
	ui_widget_set_visible(w, TRUE, NULL);
    }

    return OK;
}

#if 0
static gboolean
ui_widget_tooltip_set_visible(widget_t *w, gboolean visible, GError **err)
{
    assert(w != NULL);
    assert(w->type == UI_WIDGET_TOOLTIP);

    widget_tooltip_t *wt = w->data;
    assert(wt != NULL);

print("god damn: %d\n", visible);

    if (visible == TRUE) {
	print("visible\n");
	sys_timer_reset(wt->timer);
    } else {
	sys_timer_stop(wt->timer);
    }

    return OK;
}
#endif

static gboolean
ui_widget_tooltip_parent_set_pos(widget_t *w, GError **err)
{
    assert(w != NULL);
    assert(w->type == UI_WIDGET_TOOLTIP);

    widget_tooltip_t *wt = w->data;
    assert(wt != NULL);

    // update with new parent position
    _ui_widget_set_pos(w, w->parent->upos, NULL);
    //vec2_cp(w->parent->upos, w->upos);

    return OK;
}

static gboolean
ui_widget_tooltip_draw(widget_t *w, GError **err)
{
    assert(w != NULL);
    assert(w->type == UI_WIDGET_TOOLTIP);

    widget_tooltip_t *wt = w->data;
    assert(wt != NULL);

    widget_t *root;
    widget_root_t *wr;
    real top, bot, left, right;
    vec3_t *v;
    char *key;

    root = ui_widget_get_root(w);
    wr = root->data;
//    if (wr->active != NULL)
//	print("wtf: 0x%08d 0x%08d %d %d\n", wr->active, w->parent, wr->active->type, w->parent->type);
    if (wr->active != w->parent->parent)
	return OK;

    left  = w->rpos[X];
    right = w->rpos[X] + w->size[WIDTH];
    bot   = w->rpos[Y];
    top   = w->rpos[Y] + w->size[HEIGHT];

    /* draw the tooltip background */
    key = "background";
    v = g_hash_table_lookup(wt->colors, key);
    glColor3v(*v);

    glBegin(GL_TRIANGLES);
        glVertex2f(left,  bot);
        glVertex2f(right, top);
        glVertex2f(left,  top);

        glVertex2f(left,  bot);
        glVertex2f(right, bot);
        glVertex2f(right, top);

        glVertex2f(right, bot);
        glVertex2f(right, top);
        glVertex2f(right, top);
    glEnd();

    /* outline the tooltip */
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

    key = "text";
    v = g_hash_table_lookup(wt->colors, key);
    ui_font_draw_text(wt->font, w->rpos[X], w->rpos[Y], wt->font_size, *v, wt->txt);

    return OK;
}

void
ui_widget_tooltip_set_color(widget_t *w, char *key, vec3_t color)
{
    widget_tooltip_t *wt = w->data;
    vec3_t *v;

    assert(wt != NULL);

    v = g_hash_table_lookup(wt->colors, key);
    if (v == NULL) {
        printerr("no color: %s\n", key);
        return;
    }
    vec3_cp(color, *v);
}

gboolean
ui_widget_tooltip_set_enable(widget_t *w, gboolean enable, GError **err)
{
    assert(w != NULL);
    assert(w->type == UI_WIDGET_TOOLTIP);

    widget_tooltip_t *wt = w->data;
    assert(wt != NULL);

    if (wt->enabled == TRUE && enable == FALSE) {
//    if (enable == FALSE) {
	sys_timer_stop(wt->timer);
	ui_widget_set_visible(w, FALSE, NULL);
    } else if (wt->enabled == FALSE && enable == TRUE) {
//    } else if (enable == TRUE) {
	sys_timer_reset(wt->timer);
	ui_widget_set_visible(w, FALSE, NULL);
    }

    wt->enabled = enable;

    return OK;
}

gboolean
ui_widget_tooltip_set_text(widget_t *w, char *txt, GError **err)
{
    assert(w != NULL);
    assert(w->type == UI_WIDGET_TOOLTIP);

    widget_tooltip_t *wt = w->data;
    assert(wt != NULL);

    if (wt->txt != NULL)
	g_free(wt->txt);

    if (txt != NULL) {
	wt->txt = strdup(txt);
    } else {
	wt->txt = NULL;
    }

    ui_widget_tooltip_recalculate_size(w);

    sys_timer_stop(wt->timer);
    ui_widget_set_visible(w, FALSE, NULL);
    ui_widget_tooltip_set_enable(w, FALSE, NULL);


    return OK;
}

static gboolean
ui_widget_tooltip_destroy(widget_t *w, GError **err)
{
    widget_tooltip_t *wt = w->data;
    assert(wt != NULL);
    g_hash_table_destroy(wt->colors);
    return OK;
}

widget_t *
ui_widget_tooltip_new(widget_t *parent, char *txt)
{
    widget_t *w;
    widget_tooltip_t *wt;
    font_t *font = ui_font_get("small");
    uint16_t font_size = font->chrsize[WIDTH];
    GHashTable *t;

    /* create tooltip */
    w = ui_widget_new(parent, UI_WIDGET_TOOLTIP, NULL);
    wt = w->data;

    /* default event handlers */
    w->destroy		= &ui_widget_tooltip_destroy;
    w->draw		= &ui_widget_tooltip_draw;
    w->parent_set_pos	= &ui_widget_tooltip_parent_set_pos;
//    w->set_visible	= &ui_widget_tooltip_set_visible;
    w->update		= &ui_widget_tooltip_update;

    /* set up colors */
    wt->colors = g_hash_table_new_full(g_str_hash, g_str_equal,
            NULL, (void (*)(gpointer))vec3_destroy);
    t = wt->colors;

    g_hash_table_insert(t, "background",           vec3_new(1.0,  1.0,  0.0));
    g_hash_table_insert(t, "border",               vec3_new(1.0,  1.0,  1.0));
    g_hash_table_insert(t, "text",                 vec3_new(0.0,  0.0,  0.0));

    /* default data */
    wt = w->data;
    wt->font = font;
    wt->font_size = font_size;
    wt->timer = sys_timer_new(scs.clock);
//    wt->ready = FALSE;
    wt->enabled = FALSE;
    //sys_timer_set_interval(wt->timer, 750);
    sys_timer_set_interval_ms(wt->timer, 350);
    sys_timer_set_repeating(wt->timer, FALSE);

    ui_widget_tooltip_set_text(w, txt, NULL);
    w->set_size(w, w->size, NULL);

    w->set_pos(w, parent->upos, NULL);

    ui_widget_set_visible(w, FALSE, NULL);

    return w;
}
