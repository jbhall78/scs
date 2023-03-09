#include <SDL_opengl.h>
#include <glib.h>

#include "scs.h"
#include "shared.h"
#include "ui.h"
#include "timer.h"
#include "sys_gl.h"

gboolean
ui_widget_button_set_text(widget_t *w, char *txt, GError **err)
{
    assert(w != NULL);
    assert(w->type == UI_WIDGET_BUTTON);
    widget_button_t *wb = w->data;
    assert(wb != NULL);

    if (txt == NULL)
	return OK;

    if (wb->txt != NULL) {
	g_free(wb->txt);
	wb->txt = strdup(txt);
    }

    ui_widget_label_set_text(wb->label, txt);

    return OK;
}

static gboolean
ui_widget_button_set_size(widget_t *w, widget_sizev_t size, GError **err)
{
    assert(w != NULL);

    widget_button_t *wb = w->data;
    assert(wb != NULL);

    vec2_cp(size, w->size);

    // resize label
    if (wb->label != NULL) {
	widget_sizev_t fsize;
	fsize[WIDTH] = size[WIDTH] - wb->label_padding*2;
	fsize[HEIGHT] = size[HEIGHT] - wb->label_padding*2;

	CALL(wb->label->set_size)(wb->label, fsize, NULL);
    }

    return OK;
}

#if 0
static gboolean
ui_widget_button_set_pos(widget_t *w, widget_posv_t pos, GError **err)
{
    widget_button_t *wb = w->data;
    assert(wb != NULL);

    vec2_cp(pos, w->pos);

    widget_posv_t fpos;
    fpos[X] = pos[X] + wb->label_padding;
    fpos[Y] = pos[Y] + wb->label_padding;

    return CALL(wb->label->set_pos)(wb->label, fpos, err);
}
#endif

static gboolean
ui_widget_button_draw(widget_t *w, GError **err)
{
    widget_button_t *wb = w->data;
    widget_t *r;
    widget_root_t *wr;
    real top, bot, left, right, edge;
    gboolean depressed;
    char *key;
    vec3_t *v;

    assert(w != NULL);
    assert(wb != NULL);

    if (wb->edge)
	edge = 10.0;
    else
	edge = 0;

/*
    if (w->parent->type == UI_WIDGET_SCROLLBAR) {
	print("grr: [%d,%d]\n", w->rpos[X], w->rpos[Y]);
    }
*/

    left  = w->rpos[X];
    right = w->rpos[X] + w->size[WIDTH];
    bot   = w->rpos[Y];
    top   = w->rpos[Y] + w->size[HEIGHT];

    r = ui_widget_get_root(w);
    wr = r->data;

    depressed = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(1);

    /* draw the button background */
    if (wr->active == w && depressed)
	key = "background_depressed";	
    else if (wr->active == w)
	key = "background_highlight";
    else
	key = "background";

    v = g_hash_table_lookup(wb->colors, key);
    glColor3v(*v);

    glBegin(GL_TRIANGLES);
    	glVertex2f(left,  bot);
	glVertex2f(right, top);
	glVertex2f(left,  top);

    	glVertex2f(left,  bot);
	glVertex2f(right, bot);
	glVertex2f(right, top);

    	glVertex2f(right, bot);
	glVertex2f(right+edge, top);
	glVertex2f(right, top);
    glEnd();

    /* outline the button */
    if (wr->active == w && depressed)
	key = "border_depressed";
    else if (wr->active == w)
	key = "border_highlight";
    else
	key = "border";

    v = g_hash_table_lookup(wb->colors, key);
    glColor3v(*v);
    glLineWidth(1.0);
    glBegin(GL_LINES);

    	glVertex2f(left,  bot);
	glVertex2f(right, bot);

	glVertex2f(right, bot);
	glVertex2f(right+edge, top);

	glVertex2f(right+edge, top);
	glVertex2f(left,  top);

	glVertex2f(left,  top);
    	glVertex2f(left,  bot);
    glEnd();

    if (wb->arrow_direction != NONE) {
	ui_draw_arrow(wb->arrow_direction, wb->arrow_padding, w->rpos, w->size);
    }

    return OK;
}

static gboolean
ui_widget_button_update(widget_t *w, GError **err)
{
    widget_t *r;
    widget_button_t *wb = w->data;
    widget_root_t   *wr;
    gboolean depressed = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(1);
    char *key;
    vec3_t *v;
    gboolean ready;

    assert(wb != NULL);

    r = ui_widget_get_root(w);
    wr = r->data;

    if (wr->active == w && depressed)
	key = "label_depressed";
    else if (wr->active == w)
	key = "label_highlight";
    else
	key = "label";

    v = g_hash_table_lookup(wb->colors, key);

    ui_widget_label_set_color(wb->label, *v);

    if (wb->repeating) {
	if ((ready = sys_timer_is_ready(wb->repeat_timer)) == TRUE) {
	    CALL(wb->callback)(w, wb->data);
	}
    }

    return OK;
}

static gboolean
ui_widget_button_mbutton(widget_t *w, SDL_MouseButtonEvent *ev, widget_t **handled_by, GError **err)
{
    widget_button_t *wb = w->data;
    assert(wb != NULL);

    if (ev->button == SDL_BUTTON_WHEELUP || ev->button == SDL_BUTTON_WHEELDOWN) {
	if (ev->type == SDL_MOUSEBUTTONUP) {
	    // buttons can't scroll so send the event up the chain until someone processes it
	    // or we can no longer propagate the event
	    if (w->parent != NULL && w->parent->mbutton != NULL)
		return CALL(w->parent->mbutton)(w->parent, ev, handled_by, err);
	}
	return OK;
    }

    if (ev->type == SDL_MOUSEBUTTONDOWN) {
       	// trigger the user defined event handler
    	CALL(wb->callback)(w, wb->data);
	if (wb->repeating)
	    sys_timer_reset(wb->repeat_timer);
    } else if (ev->type == SDL_MOUSEBUTTONUP) {
	sys_timer_stop(wb->repeat_timer);
    }

    *handled_by = w;

    return OK;
}

static gboolean
ui_widget_button_destroy(widget_t *w, GError **err)
{
    widget_button_t *wb = w->data;
    assert(wb != NULL);
    g_hash_table_destroy(wb->colors);
    g_free(wb->txt);
    return OK;
}

#if 0
static void
ui_widget_button_set_label_rect(widget_t *label, widget_posv_t pos, widget_sizev_t size)
{
    int padding = 3;
    widget_posv_t fpos;
    widget_sizev_t fsize;

    fpos[X] = pos[X] + padding;
    fpos[Y] = pos[Y] + padding;

    fsize[WIDTH] = size[WIDTH] - padding*2;
    fsize[HEIGHT] = size[HEIGHT] - padding*2;

    label->set_pos(label, fpos);
    label->set_size(label, fsize);
}
#endif

#if 0
deprecated void
ui_widget_button_set_rect(widget_t *w, widget_posv_t pos, widget_sizev_t size)
{
    widget_button_t *wb = w->data;

    w->set_size(size);
    w->set_pos(pos);

    ui_widget_button_set_label_rect(wb->label, pos, size);
}
#endif

void
ui_widget_button_set_font(widget_t *button, font_t *fnt, uint16_t size)
{
    widget_button_t *wb = button->data;
    assert(wb != NULL);
    widget_t *label = wb->label;
    assert(label != NULL);

    ui_widget_label_set_font(label, fnt);
    ui_widget_label_set_font_size(label, size);

    wb->font = fnt;
    wb->font_size = size;
}

void
ui_widget_button_set_arrow(widget_t *w, direction_t direction, uint16_t padding)
{
    widget_button_t *wb = w->data;
    assert(wb);

    wb->arrow_direction = direction;
    wb->arrow_padding   = padding;
}

void
ui_widget_button_set_color(widget_t *w, char *key, vec3_t color)
{
    widget_button_t *wb = w->data;
    vec3_t *v;

    assert(wb != NULL);

    v = g_hash_table_lookup(wb->colors, key);
    if (v == NULL) {
	printerr("no color: %s\n", key);
	return;
    }
    vec3_cp(color, *v);
}

void
ui_widget_button_set_edge(widget_t *w, gboolean edge)
{
    widget_button_t *wb = w->data;

    assert(wb != NULL);
    wb->edge = edge;
}

void
ui_widget_button_set_repeating(widget_t *w, gboolean repeating)
{
    assert(w != NULL);
    assert(w->type == UI_WIDGET_BUTTON);
    widget_button_t *wb = w->data;
    assert(wb != NULL);
    assert(wb->repeat_timer != NULL);

    wb->repeating = repeating;
    sys_timer_set_repeating(wb->repeat_timer, repeating);
    if (repeating == FALSE)
	sys_timer_stop(wb->repeat_timer);
}

widget_t *
ui_widget_button_new(widget_t *parent, char *txt,
			widget_posv_t pos, widget_sizev_t size,
			void (*callback)(widget_t *w, void *data))
{
    widget_t *w;
    widget_button_t *wb;
    font_t *fnt = ui_font_get("medium");
    GHashTable *t;
    vec3_t *v;

    /* create button */
    w = ui_widget_new(parent, UI_WIDGET_BUTTON, NULL);

    /* default event handlers */
    w->mbutton  = &ui_widget_button_mbutton;
    w->destroy  = &ui_widget_button_destroy;
    w->update   = &ui_widget_button_update;
    w->draw     = &ui_widget_button_draw;
    w->set_size = &ui_widget_button_set_size;
//    w->set_pos  = &ui_widget_button_set_pos;
    w->set_pos(w, pos, NULL);

    /* default data */
    wb = w->data;
    wb->callback = callback;
    wb->edge = TRUE;
    wb->arrow_direction = NONE;
    wb->arrow_padding = 2;
    wb->label_padding = 3;
    if (txt != NULL)
    	wb->txt = strdup(txt);
    wb->repeating = FALSE;
    wb->repeat_timer = sys_timer_new(scs.clock);
    sys_timer_set_interval_ms(wb->repeat_timer, 150);

    /* set up colors */
    wb->colors = g_hash_table_new_full(g_str_hash, g_str_equal,
	    NULL, (void (*)(gpointer))vec3_destroy);
    t = wb->colors;

    g_hash_table_insert(t, "background",           vec3_new(0.0,  0.03, 0.25));
    g_hash_table_insert(t, "background_highlight", vec3_new(0.0,  0.0,  1.0));
    g_hash_table_insert(t, "background_depressed", vec3_new(0.3,  0.3,  0.3));
    g_hash_table_insert(t, "border",               vec3_new(0.6,  0.6,  0.6));
    g_hash_table_insert(t, "border_highlight",     vec3_new(0.6,  0.6,  0.6));
    g_hash_table_insert(t, "border_depressed",     vec3_new(0.0,  0.0,  1.0));
    g_hash_table_insert(t, "label",                vec3_new(0.6,  0.6,  1.0));
    g_hash_table_insert(t, "label_highlight",      vec3_new(1.0,  1.0,  1.0));
    g_hash_table_insert(t, "label_depressed",      vec3_new(0.0,  0.0,  1.0));

    /* create label */
    widget_posv_t lpos, lsize;

    v = g_hash_table_lookup(t, "label");

    // this needs to be moved into the label widget itself
    // it overly complicates the widgets that try to use
    // this feature
    lpos[X] = wb->label_padding;
    lpos[Y] = wb->label_padding;
    vec2_cp(size, lsize);
    lsize[WIDTH] -= wb->label_padding;
    lsize[HEIGHT] -= wb->label_padding;
    wb->label = ui_widget_label_new(w, txt, fnt, *v, lpos, size);

    w->set_size(w, size, NULL);

    ui_widget_label_set_font(wb->label, fnt);

    // UNTESTED: use hratio to correctly adjust the font size to the correct height
    ui_widget_label_set_font_size(wb->label, fnt->hratio * (size[HEIGHT] - 6) );

    return w;
}
