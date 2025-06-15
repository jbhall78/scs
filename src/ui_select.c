#include <SDL_opengl.h>
#include <glib.h>

#include "scs.h"
#include "shared.h"
#include "ui.h"
#include "sys_gl.h"

static char *DefaultFont = "input";
static uint16_t DefaultFontSize = 8;

char *
ui_widget_select_get_selected_text(widget_t *w)
{
    assert(w != NULL);

    widget_select_t *ws = w->data;
    assert(ws != NULL);

    GList *item;
    int i;

    for (item = ws->list, i = 0; ws->selected != -1 && item; item = item->next, i++) {
	char *text = item->data;

	if (i != ws->selected)
	    continue;

	return text;
    }

    return NULL;
}

void
ui_widget_select_set_selected(widget_t *w, int32_t idx)
{
    assert(w != NULL);
    assert(w->type == UI_WIDGET_SELECT);

    widget_select_t *ws = w->data;
    assert(ws != NULL);

    char *txt;

    if (idx < 0)
	return;
    if (idx > ws->items)
	return;

    ws->selected = idx;

    if (ws->button_text != NULL) {
	txt = ui_widget_select_get_selected_text(w);
	if (txt != NULL) {
	    ui_widget_button_set_text(ws->button_text, txt, NULL);
	}
    }
}

#if 1
void
ui_widget_select_recalculate_dropdown_buttons(widget_t *sel)
{
    widget_select_t *ws = sel->data;
    assert(ws != NULL);
    widget_scrollbar_t *wsb = ws->scrollbar->data;
    assert(wsb != NULL);
    widget_t *button;
    int i, j = 0;
    char *key;
    vec3_t *v;
    vec3_t color_fg, color_bg;
    vec3_t color_selected_fg, color_selected_bg;

    key = "background";
    v = g_hash_table_lookup(ws->colors, key);
    vec3_cp(*v, color_bg);

    key = "text";
    v = g_hash_table_lookup(ws->colors, key);
    vec3_cp(*v, color_fg);

    key = "selected";
    v = g_hash_table_lookup(ws->colors, key);
    vec3_cp(*v, color_selected_bg);

    key = "selected_text";
    v = g_hash_table_lookup(ws->colors, key);
    vec3_cp(*v, color_selected_fg);

    if (ws->dropdown_extended != TRUE) {
	for (i = 0; i < ws->buttons->len; i++) {
	    button = g_ptr_array_index(ws->buttons, i);
	    ui_widget_set_visible(button, FALSE, NULL);
	}
	return;
    }

    for (i = 0; i < ws->buttons->len; i++) {
	button = g_ptr_array_index(ws->buttons, i);

	// not visible on top
	if (i < wsb->pos) {
    	    ui_widget_set_visible(button, FALSE, NULL);
	    continue;
	}

	// not visible on bottom
	if (i >= (ws->dropdown_size + wsb->pos)) {
    	    ui_widget_set_visible(button, FALSE, NULL);
	    continue;
	}

	// visible, update coordinates
	widget_posv_t pos;
	widget_sizev_t size;

	ui_widget_set_visible(button, TRUE, NULL);

	vec2_zero(pos);
	vec2_cp(sel->size, size);
	size[WIDTH] -= ws->scrollbar_width;

	pos[Y] -= sel->size[HEIGHT] * (j + 1);
	j++;

	button->set_pos(button, pos, NULL);
	button->set_size(button, size, NULL);

	if (i == ws->selected) {
	    ui_widget_button_set_color(button, "label", color_selected_fg);
	    ui_widget_button_set_color(button, "background", color_selected_bg);
	} else {
	    ui_widget_button_set_color(button, "label", color_fg);
	    ui_widget_button_set_color(button, "background", color_bg);
	}
    }
}
#endif

void
ui_widget_select_inc_selected(widget_t *w)
{
    assert(w != NULL);
    assert(w->type == UI_WIDGET_SELECT);
    widget_select_t *ws = w->data;
    assert(ws != NULL);

    int n = ws->selected + 1;
    if (n > ws->items)
        return;

    ui_widget_select_set_selected(w, n);

    ui_widget_select_recalculate_dropdown_buttons(w);
}

void
ui_widget_select_dec_selected(widget_t *w)
{
    assert(w != NULL);
    assert(w->type == UI_WIDGET_SELECT);
    widget_select_t *ws = w->data;
    assert(ws != NULL);

    int n = ws->selected - 1;
    if (n < 0)
        return;

    ui_widget_select_set_selected(w, n);
}

static gboolean
ui_widget_select_key(widget_t *w, SDL_KeyboardEvent *k, gboolean *handled, GError **err)
{
    assert(w != NULL);
    widget_select_t *ws = w->data;
    assert(ws != NULL);

//print("called\n");

    if (k->type == SDL_KEYUP)
        return OK;

    switch (k->keysym.sym) {
        case SDLK_ESCAPE:
	    if (ws->dropdown_extended == TRUE) {
    		ui_widget_select_set_dropdown(w, FALSE);
    		*handled = TRUE;
	    }
	    break;
	case SDLK_UP:
	    ui_widget_select_dec_selected(w);
	    ui_widget_scrollbar_scroll_up(ws->scrollbar);
	    *handled = TRUE;
	    break;
	case SDLK_DOWN:
	    ui_widget_select_inc_selected(w);
	    ui_widget_scrollbar_scroll_down(ws->scrollbar);
	    *handled = TRUE;
	    break;
	default:
	    break;
    }

    return OK;
}

static gboolean
ui_widget_select_mbutton(widget_t *w, SDL_MouseButtonEvent *ev, widget_t **handled_by, GError **err)
{
    widget_select_t *ws = w->data;
    assert(ws != NULL);

    /*
    if (ev->type != SDL_MOUSEBUTTONUP)
        return OK;

    if (ev->button == SDL_BUTTON_WHEELUP || ev->button == SDL_BUTTON_WHEELDOWN) {
	if (ws->scrollbar == NULL)
	    return OK;

	if (ws->dropdown_extended) {
	    if (ev->button == SDL_BUTTON_WHEELUP)
		ui_widget_scrollbar_scroll_up(ws->scrollbar);
	    else if (ev->button == SDL_BUTTON_WHEELDOWN)
		ui_widget_scrollbar_scroll_down(ws->scrollbar);
	} else {
	    if (ev->button == SDL_BUTTON_WHEELUP)
		ui_widget_select_dec_selected(w);
	    else if (ev->button == SDL_BUTTON_WHEELDOWN)
		ui_widget_select_inc_selected(w);
	}

	*handled_by = w;
    }   
    */

    return OK;
}

static gboolean
ui_widget_select_update(widget_t *w, GError **err)
{
    ui_widget_select_recalculate_dropdown_buttons(w);
    return OK;
}

static gboolean
ui_widget_select_draw(widget_t *w, GError **err)
{
    widget_select_t *ws = w->data;
    real top, bot, left, right;
    char *key;
    vec3_t *v;

    assert(w != NULL);
    assert(ws != NULL);

    left  = w->rpos[X];
    right = w->rpos[X] + w->size[WIDTH];
    bot   = w->rpos[Y];
    top   = w->rpos[Y] + w->size[HEIGHT];

    /* draw the button background */
    key = "background";

    v = g_hash_table_lookup(ws->colors, key);
    glColor3v(*v);

    glBegin(GL_TRIANGLES);
        glVertex2f(left,  bot);
        glVertex2f(right, top);
        glVertex2f(left,  top);

        glVertex2f(left,  bot);
        glVertex2f(right, bot);
        glVertex2f(right, top);
    glEnd();

    /* outline the button */
    key = "border";

    v = g_hash_table_lookup(ws->colors, key);
    glColor3v(*v);
    glLineWidth(1.0);
    glBegin(GL_LINES);
        // bottom
        glVertex2f(left,  bot);
        glVertex2f(right, bot);

	// right
        glVertex2f(right, bot);
        glVertex2f(right, top);

	// top
        glVertex2f(right, top);
        glVertex2f(left,  top);

	// left
        glVertex2f(left,  top);
        glVertex2f(left,  bot);
    glEnd();

    /* draw selected text */
#if 0
    widget_posv_t pos;
    int i;
    GList *l;

    v = g_hash_table_lookup(ws->colors, "text");
    glColor3v(*v);

    pos[X] = left + 8;
    pos[Y] = bot + 4;

    for (l = ws->list, i = 0; ws->selected != -1 && l; l = l->next, i++) {
	char *text = l->data;
	if (i != ws->selected)
	    continue;

	ui_font_draw_text(ws->font, pos[X], pos[Y], 8, *v, "%s", text);
	break;
    }
#endif


    return OK;
}

gboolean
ui_widget_select_destroy(widget_t *w, GError **err)
{
    widget_select_t *ws = w->data;
    assert(ws != NULL);
    g_hash_table_destroy(ws->colors);
    return OK;
}

void
ui_widget_select_set_dropdown_size(widget_t *sel, uint16_t n)
{
    widget_select_t *ws = sel->data;
    assert(ws != NULL);

    ws->dropdown_size = n;

    /* create scrollbar object */
    widget_posv_t sbpos;
    widget_sizev_t sbsize;

    sbpos[X] = sel->size[WIDTH] - ws->scrollbar_width;
    sbpos[Y] = 0 - (sel->size[HEIGHT] * ws->dropdown_size);
    sbsize[WIDTH] = ws->scrollbar_width;
    sbsize[HEIGHT] = sel->size[HEIGHT] * ws->dropdown_size;
    ws->scrollbar->set_pos(ws->scrollbar, sbpos, NULL);
    ws->scrollbar->set_size(ws->scrollbar, sbsize, NULL);

    ui_widget_scrollbar_set_visible_items(ws->scrollbar, ws->dropdown_size);
}

void
ui_widget_select_set_font(widget_t *sel, font_t *fnt, uint16_t size)
{
    int i;
    widget_select_t *ws = sel->data;
    assert(ws != NULL);

    ws->font = fnt;
    ws->font_size = size;

    for (i = 0; i < ws->buttons->len; i++) {
	widget_t *button = g_ptr_array_index(ws->buttons, i);
	assert(button != NULL);
	ui_widget_button_set_font(button, fnt, size);
    }
}

void
ui_widget_select_set_dropdown(widget_t *sel, gboolean val)
{
    widget_select_t *ws = sel->data;

    ws->dropdown_extended = val;
    ui_widget_select_recalculate_dropdown_buttons(sel);
    ui_widget_set_visible(ws->scrollbar, val, NULL);
}

void
ui_widget_select_toggle_dropdown(widget_t *sel)
{
    widget_select_t *ws = sel->data;
    ui_widget_select_set_dropdown(sel, ws->dropdown_extended ^ 1);
}

void
ui_widget_select_list_button_cb(widget_t *button, void *data)
{
    widget_t *sel = button->parent;
    uint32_t *index = data;

    ui_widget_select_set_selected(sel, *index);
    ui_widget_select_toggle_dropdown(sel);
}

void
ui_widget_select_button_cb(widget_t *button, void *data)
{
    widget_t *sel = button->parent;
    assert(sel != NULL);

    ui_widget_select_toggle_dropdown(sel);
}

void
ui_widget_select_append(widget_t *w, char *text, char *data)
{
    widget_select_t *ws = w->data;
    widget_t *button;
    widget_posv_t pos;
    widget_sizev_t size;

    vec2_zero(pos);
    vec2_cp(w->size, size);
    size[WIDTH] -= ws->scrollbar_width;

    // create button
    uint32_t *index = g_new(uint32_t, 1);
    *index = ws->items;
    button = ui_widget_button_new(w, text, pos, size, ui_widget_select_list_button_cb); 
    ui_widget_set_visible(button, FALSE, NULL);
    ui_widget_button_set_data(button, index);
    ui_widget_button_set_font(button, ws->font, ws->font_size);
    ui_widget_button_set_edge(button, FALSE);

    ws->list = g_list_append(ws->list, strdup(text));
    g_ptr_array_add(ws->data, data);
    g_ptr_array_add(ws->buttons, button);
    ws->items++;

    ui_widget_scrollbar_set_items(ws->scrollbar, ws->items);

    if (ws->selected == -1)
	ui_widget_select_set_selected(w, 0);
}

void
ui_widget_select_clear(widget_t *w)
{
    GList *l;
    widget_select_t *ws = w->data;
    uint16_t i;

    ws->selected = -1;

    for (l = ws->list, i = ws->items-1; l; l = l->next, i--) {
	g_free(l->data);
	// XXX memory leak if data is not freed (data free callback?)
	//g_free(g_ptr_array_index(ws->data, i));
	g_ptr_array_remove_index(ws->data, i);
	g_ptr_array_remove_index(ws->buttons, i);
    }
    g_list_free(ws->list);
    ws->list = NULL;

    ws->items = 0;
}
char *
ui_widget_select_get_data(widget_t *w)
{
    widget_select_t *ws = w->data;

    if (ws->selected == -1)
	return NULL;

    return strdup(g_ptr_array_index(ws->data, ws->selected));
}

void
ui_widget_select_set_color(widget_t *w, char *key, vec3_t color)
{
    widget_select_t *ws = w->data;
    vec3_t *v;

    assert(ws != NULL);

    v = g_hash_table_lookup(ws->colors, key);
    if (v == NULL) {
	printerr("no color: %s\n", key);
	return;
    }
    vec3_cp(color, *v);
}

widget_t *
ui_widget_select_new(widget_t *parent, widget_posv_t pos, widget_sizev_t size)
{
    widget_t *w;
    widget_select_t *ws;
    GHashTable *t;
    int scrollbar_width = 20;

    /* create the columned list */
    w = ui_widget_new(parent, UI_WIDGET_SELECT, NULL);

    /* default event handlers */
    w->destroy = &ui_widget_select_destroy;
    w->update  = &ui_widget_select_update;
    w->draw    = &ui_widget_select_draw;
    w->mbutton = &ui_widget_select_mbutton;
    w->key     = &ui_widget_select_key;

    size[WIDTH] -= scrollbar_width;
    w->set_pos(w, pos, NULL);
    w->set_size(w, size, NULL);
    ws = w->data;

    ws->list = NULL;
    ws->selected = -1;
    ws->data = g_ptr_array_new();
    ws->buttons = g_ptr_array_new();
    ws->scrollbar_width = scrollbar_width;


    /* set up colors */
    ws->colors = g_hash_table_new_full(g_str_hash, g_str_equal,
	    NULL, (void (*)(gpointer))vec3_destroy);
    t = ws->colors;

    g_hash_table_insert(t, "background",	vec3_new(0.0, 0.03, 0.25));
    g_hash_table_insert(t, "border",		vec3_new(0.6, 0.6,  0.6));
    g_hash_table_insert(t, "text",		vec3_new(1.0, 1.0,  1.0));

    // active/hilight (mouse)
    g_hash_table_insert(t, "selection",		vec3_new(0.0, 0.5,  0.5));

    // selected item  (keyboard)
    g_hash_table_insert(t, "selected",		vec3_new(1.0, 0.0, 0.0));
    g_hash_table_insert(t, "selected_text",	vec3_new(1.0, 1.0, 1.0));

    /* add button to the selectbox object */
    widget_posv_t bpos;
    widget_sizev_t bsize;

    bpos[X] = size[WIDTH] - scrollbar_width;
    bpos[Y] = 0;
    bsize[WIDTH] = scrollbar_width;
    bsize[HEIGHT] = size[HEIGHT];

    // dropdown button
    ws->button = ui_widget_button_new(w, NULL, bpos, bsize, ui_widget_select_button_cb);
    ui_widget_button_set_arrow(ws->button, SOUTH, 2);
    ui_widget_button_set_edge(ws->button, FALSE);
    font_t *fnt = ui_font_get(DefaultFont);
    ui_widget_select_set_font(w, fnt, DefaultFontSize);

    // text button
    bpos[X] = 0;
    bpos[Y] = 0;
    bsize[WIDTH] = size[WIDTH] - scrollbar_width;
    bsize[HEIGHT] = size[HEIGHT];
    ws->button_text = ui_widget_button_new(w, NULL, bpos, bsize, ui_widget_select_button_cb);
    ui_widget_button_set_edge(ws->button_text, FALSE);
    ui_widget_button_set_font(ws->button_text, ws->font, ws->font_size);


    /* create scrollbar object */
    ws->scrollbar = ui_widget_scrollbar_new(w, TRUE, pos, size, NULL);
    ui_widget_scrollbar_set_items(ws->scrollbar, 0);
    ui_widget_scrollbar_set_item_size(ws->scrollbar, size[HEIGHT]);
    ui_widget_select_set_dropdown_size(w, 10);
    ui_widget_set_visible(ws->scrollbar, FALSE, NULL);

    return w;
}
