#include <glib.h>
#include <SDL_opengl.h>

#include "scs.h"
#include "shared.h"
#include "ui.h"

#include "client.h"
#include "sys_gl.h"

void
ui_widget_scrollbar_recalculate_slider(widget_t *w)
{
    assert(w != NULL);
    assert(w->type == UI_WIDGET_SCROLLBAR);
    widget_scrollbar_t *wsb = w->data;
    assert(wsb != NULL);

    vec2_zero(wsb->slider_pos);
    vec2_zero(wsb->slider_size);
    if (wsb->items > 0) {
	widget_pos_t display_items = wsb->items - wsb->visible_items;
	if (display_items > 0) {
	    // NOTE: we lose some precision if we are using integers here
	    //       it could be rewritten a different way
	    double sbheight = w->size[HEIGHT] - (wsb->button_size * 2); // scrollbar height after removal of buttons
	    double height = sbheight / display_items; // height of scroller
	    double freeheight = sbheight - height;	// free scrollbar space after taking out buttons & scroller size
	    double offset = wsb->pos * (freeheight / (display_items)); // position of scroller

	    wsb->slider_pos[X] = 0;
	    wsb->slider_pos[Y] = w->size[HEIGHT] - wsb->button_size - offset - height;
	    wsb->slider_size[WIDTH] = w->size[WIDTH];
	    wsb->slider_size[HEIGHT] = height;
	}
    }
}

void
ui_widget_scrollbar_set_items(widget_t *w, uint16_t n)
{
    assert(w != NULL);
    assert(w->type == UI_WIDGET_SCROLLBAR);
    widget_scrollbar_t *wsb = w->data;
    assert(wsb != NULL);

    wsb->items = n;

    ui_widget_scrollbar_recalculate_slider(w);
}

static gboolean
ui_widget_scrollbar_set_size(widget_t *w, widget_sizev_t size, GError **err)
{
    assert(w != NULL);
    assert(w->type == UI_WIDGET_SCROLLBAR);
    widget_scrollbar_t *wsb = w->data;
    assert(wsb != NULL);
    widget_sizev_t bsize;

    vec2_cp(size, w->size);

    bsize[HEIGHT] = wsb->button_size;
    bsize[WIDTH] = w->size[WIDTH];

    // set button 1 size
    wsb->button1->set_size(wsb->button1, bsize, NULL);

    // set button 2 size
    wsb->button2->set_size(wsb->button2, bsize, NULL);

    // recalculate child button positions
    // so we don't have to call set_pos/set_size in
    // a particular order
    w->set_pos(w, w->upos, NULL);

    return OK;
}

static gboolean
ui_widget_scrollbar_set_pos(widget_t *w, widget_posv_t pos, GError **err)
{
    widget_scrollbar_t *wsb = w->data;
    assert(wsb != NULL);
    widget_pos_t left, bot;
    widget_sizev_t bpos;

    //vec2_cp(pos, w->pos);
    vec2_cp(pos, w->upos);
    ui_widget_calculate_relativity(w);

    // set button 1
    left  = 0;
    bot   = w->size[HEIGHT] - wsb->button_size;
    bpos[X] = left;
    bpos[Y] = bot;
    wsb->button1->set_pos(wsb->button1, bpos, NULL);

    // set button 2
    left  = 0;
    bot   = 0;
    bpos[X] = left;
    bpos[Y] = bot;
    wsb->button2->set_pos(wsb->button2, bpos, NULL);

    ui_widget_scrollbar_recalculate_slider(w);

    return OK;
}

static gboolean
ui_widget_scrollbar_draw(widget_t *w, GError **err)
{
    widget_scrollbar_t *wsb;
    widget_t *r;
    widget_root_t *wr;
    real top, bot, left, right;
    char *key;
    vec3_t *v;

    assert(w != NULL);
    wsb = w->data;
    assert(wsb != NULL);

    r = ui_widget_get_root(w);
    wr = r->data;

    if (wr->active == w)
	key = "border_highlight";
    else
	key = "border";

    /* draw scrollbar bg */
    left  = w->rpos[X];
    right = w->rpos[X] + w->size[WIDTH];
    bot   = w->rpos[Y];
    top   = w->rpos[Y] + w->size[HEIGHT];

    v = g_hash_table_lookup(wsb->colors, "background");
    glColor3v(*v);
    glBegin(GL_TRIANGLES);
    	glVertex2f(left,  bot);
	glVertex2f(right, top);
	glVertex2f(left,  top);

    	glVertex2f(left,  bot);
	glVertex2f(right, bot);
	glVertex2f(right, top);
    glEnd();
    v = g_hash_table_lookup(wsb->colors, key);
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

    /* draw scrollbar */
    if (wsb->items > 0) {
	/*
	double sbheight = w->size[HEIGHT] - (wsb->button_size * 2); // scrollbar height after removal of buttons
	//double virtual_size = wsb->item_size * wsb->items; // virtual size of scrollbar
	double height = sbheight / (wsb->items - wsb->visible_items); // height of scroller
	double freeheight = sbheight - height;	// free scrollbar space after taking out buttons & scroller size
	double offset = wsb->pos * (freeheight / (wsb->items - wsb->visible_items)); // position of scroller
//	print("grr: %f %f\n", height, offset);
	left  = w->rpos[X];
	right = w->rpos[X] + w->size[WIDTH];
	bot   = w->rpos[Y] + w->size[HEIGHT] - wsb->button_size - offset - height;
	top   = w->rpos[Y] + w->size[HEIGHT] - wsb->button_size - offset;
	*/
	left = w->rpos[X] + wsb->slider_pos[X];
	right = w->rpos[X] + wsb->slider_pos[X] + wsb->slider_size[WIDTH];
	bot = w->rpos[Y] + wsb->slider_pos[Y];
	top = w->rpos[Y] + wsb->slider_pos[Y] + wsb->slider_size[WIDTH];

	v = g_hash_table_lookup(wsb->colors, "bar");
	glColor3v(*v);
	glBegin(GL_TRIANGLES);
	    glVertex2f(left,  bot);
	    glVertex2f(right, top);
	    glVertex2f(left,  top);

	    glVertex2f(left,  bot);
	    glVertex2f(right, bot);
	    glVertex2f(right, top);
	glEnd();
    }

    return OK;
}

static gboolean
ui_widget_scrollbar_update(widget_t *w, GError **err)
{
    return OK;
}

static gboolean
ui_widget_scrollbar_destroy(widget_t *w, GError **err)
{
    widget_scrollbar_t *wsb = w->data;
    assert(wsb != NULL);
    g_hash_table_destroy(wsb->colors);
    return OK;
}

#define ui_widget_scrollbar_scroll_left ui_widget_scrollbar_scroll_up
void
ui_widget_scrollbar_scroll_up(widget_t *w)
{
    widget_scrollbar_t *wsb = w->data;
    assert(wsb != NULL);

    if (wsb->pos != 0)
	wsb->pos--;

    ui_widget_scrollbar_recalculate_slider(w);
}

#define ui_widget_scrollbar_scroll_right ui_widget_scrollbar_scroll_down
void
ui_widget_scrollbar_scroll_down(widget_t *w)
{
    widget_scrollbar_t *wsb = w->data;
    assert(wsb != NULL);

    if (wsb->pos < (wsb->items - wsb->visible_items))
	wsb->pos++;

    ui_widget_scrollbar_recalculate_slider(w);
}

void
ui_widget_scrollbar_button_cb(widget_t *w, void *data)
{

    widget_t *scrollbar = w->parent;
    assert(scrollbar != NULL);
    assert(scrollbar->type == UI_WIDGET_SCROLLBAR);

    widget_scrollbar_t *wsb = scrollbar->data;
    assert(wsb != NULL);

    direction_t *d = data;
    assert(d != NULL);

    switch (*d) {
	case NORTH:
	    ui_widget_scrollbar_scroll_up(scrollbar);
	    break;
	case SOUTH:
	    ui_widget_scrollbar_scroll_down(scrollbar);
	    break;
	case WEST:
	    ui_widget_scrollbar_scroll_left(scrollbar);
	    break;
	case EAST:
	    ui_widget_scrollbar_scroll_right(scrollbar);
	    break;
	default:
	    break;
    }
//    print("clicked: %d %d %d\n", *d, wsb->items, wsb->pos);
}

static gboolean
ui_widget_scrollbar_mbutton(widget_t *w, SDL_MouseButtonEvent *ev, widget_t **handled_by, GError **err)
{
    assert(w != NULL);
    assert(w->type == UI_WIDGET_SCROLLBAR);
    widget_scrollbar_t *wsb = w->data;
    assert(wsb != NULL);
    widget_t *root = ui_widget_get_root(w);
    assert(root != NULL);
    widget_pos_t left, right, bot, top;
    widget_posv_t mpos;

    if (ev->type == SDL_MOUSEBUTTONDOWN) {
	mpos[X] = ev->x;
	mpos[Y] = root->size[HEIGHT] - ev->y;

	left  = w->rpos[X] + wsb->slider_pos[X];
	right = w->rpos[X] + wsb->slider_pos[X] + wsb->slider_size[WIDTH];
	bot   = w->rpos[Y] + wsb->slider_pos[Y];
	top   = w->rpos[Y] + wsb->slider_pos[Y] + wsb->slider_size[WIDTH];

	if (mpos[X] > left && mpos[X] < right) {
	    if (mpos[Y] < bot)
		ui_widget_scrollbar_scroll_down(w);

	    if (mpos[Y] > top)
		ui_widget_scrollbar_scroll_up(w);
	}
    }

    *handled_by = w;

    return OK;
}

static gboolean
ui_widget_scrollbar_mwheel(widget_t *w, SDL_MouseWheelEvent *ev, gboolean *handled, GError **err)
{
    assert(w != NULL);
    assert(w->type == UI_WIDGET_SCROLLBAR);
    widget_scrollbar_t *wsb = w->data;
    assert(wsb != NULL);
    widget_t *root = ui_widget_get_root(w);
    assert(root != NULL);
    widget_pos_t left, right, bot, top;
    widget_posv_t mpos;

    if (ev->y > 0) {
	ui_widget_scrollbar_scroll_up(w);
    } else if (ev->y < 0) {
	ui_widget_scrollbar_scroll_down(w);
    } 

    *handled = TRUE;

    return OK;
}

void
ui_widget_scrollbar_set_color(widget_t *w, char *key, vec3_t color)
{
    widget_scrollbar_t *wsb = w->data;
    vec3_t *v;

    assert(wsb != NULL);

    v = g_hash_table_lookup(wsb->colors, key);
    if (v == NULL) {
	printerr("no color: %s\n", key);
	return;
    }
    vec3_cp(color, *v);
}

widget_t *
ui_widget_scrollbar_new(widget_t *parent, gboolean verticle,
			widget_posv_t pos, widget_sizev_t size,
			void (*callback)(void))
{
    widget_t *w;
    widget_scrollbar_t *wsb;
    GHashTable *t;

    /* create scrollbar */
    w = ui_widget_new(parent, UI_WIDGET_SCROLLBAR, NULL);

    /* default event handlers */
    w->destroy = &ui_widget_scrollbar_destroy;
    w->update  = &ui_widget_scrollbar_update;
    w->draw    = &ui_widget_scrollbar_draw;
    w->mbutton = &ui_widget_scrollbar_mbutton;
    w->mwheel  = &ui_widget_scrollbar_mwheel;
    w->set_size = &ui_widget_scrollbar_set_size;
    w->set_pos  = &ui_widget_scrollbar_set_pos;

    wsb = w->data;
    wsb->callback = callback;
    wsb->verticle = verticle;
    wsb->items = size[HEIGHT];
    wsb->pos = 0;
    wsb->button_size = 20;
    wsb->visible_items = size[HEIGHT];

    /* set up colors */
    wsb->colors = g_hash_table_new_full(g_str_hash, g_str_equal,
	    NULL, (void (*)(gpointer))vec3_destroy);
    t = wsb->colors;

    g_hash_table_insert(t, "background",           vec3_new(0.0, 0.03,  0.25));
    g_hash_table_insert(t, "background_highlight", vec3_new(0.0, 0.0,  1.0));
    g_hash_table_insert(t, "background_depressed", vec3_new(0.3, 0.3,  0.3));
    g_hash_table_insert(t, "buttonbg",             vec3_new(0.0, 0.03, 0.25));
    g_hash_table_insert(t, "buttonbg_highlight",   vec3_new(0.0, 0.0,  1.0));
    g_hash_table_insert(t, "buttonbg_depressed",   vec3_new(0.3, 0.3,  0.3));
    g_hash_table_insert(t, "border",               vec3_new(0.6, 0.6,  0.6));
    g_hash_table_insert(t, "border_highlight",     vec3_new(0.6, 0.6,  0.6));
    g_hash_table_insert(t, "border_depressed",     vec3_new(0.0, 0.0,  1.0));
    g_hash_table_insert(t, "arrow",                vec3_new(1.0, 1.0,  1.0));
    g_hash_table_insert(t, "arrow_highlight",      vec3_new(1.0, 1.0,  1.0));
    g_hash_table_insert(t, "arrow_depressed",      vec3_new(0.0, 0.0,  1.0));
    g_hash_table_insert(t, "bar",                  vec3_new(1.0, 0.0,  0.0));
    g_hash_table_insert(t, "bar_highlight",        vec3_new(1.0, 1.0,  1.0));
    g_hash_table_insert(t, "bar_depressed",        vec3_new(0.0, 0.0,  1.0));

    /* create buttons */
    direction_t *d = g_new(direction_t, 1);
    wsb->button1 = ui_widget_button_new(w, NULL, pos, size, ui_widget_scrollbar_button_cb);
    ui_widget_button_set_edge(wsb->button1, FALSE);
    if (wsb->verticle) {
	*d = NORTH;
    } else {
	*d = WEST;
    }
    ui_widget_button_set_arrow(wsb->button1, *d, 2);
    ui_widget_button_set_data(wsb->button1, d);
    ui_widget_button_set_repeating(wsb->button1, TRUE);

    wsb->button2 = ui_widget_button_new(w, NULL, pos, size, ui_widget_scrollbar_button_cb);
    ui_widget_button_set_edge(wsb->button2, FALSE);
    d = g_new(direction_t, 1);
    if (wsb->verticle) {
	*d = SOUTH;
    } else {
	*d = EAST;
    }
    ui_widget_button_set_arrow(wsb->button2, *d, 2);
    ui_widget_button_set_data(wsb->button2, d);
    ui_widget_button_set_repeating(wsb->button2, TRUE);

    // make sure the size is propagated to the buttons
    w->set_pos(w, pos, NULL);
    w->set_size(w, size, NULL);

    return w;
}

