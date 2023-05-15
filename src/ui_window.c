#include <SDL_opengl.h>

#include "scs.h"
#include "ui.h"
#include "sys_gl.h"

/*
 * WARNING: The widget actually returned to the user is of a SUBWINDOW type
 *
 * This allows other widgets to correctly reference parent->size options to
 * work correctly without having to take into account the title bar & other
 * window features.
 *
 * Most of the event handlers take the WINDOW widget as an argument
 * The get/set methods take the SUBWINDOW widget as an argument
 */

// subwindow
static gboolean
ui_widget_subwindow_set_size(widget_t *subwidget, widget_sizev_t size, GError **err)
{
    assert(subwidget != NULL);
    widget_t *widget = subwidget->parent;
    assert(widget != NULL);
    widget_window_t *window = widget->data;
    assert(window != NULL);
    widget_posv_t subwindow_pos;
    widget_sizev_t subwindow_size;

    vec2_cp(size, widget->size);

    // resize label
    if (window->title_label != NULL) {
        widget_sizev_t fsize;
        widget_posv_t fpos;

        fsize[WIDTH]  = size[WIDTH];
        fsize[HEIGHT] = window->title_height;
	fpos[X] = 0;
	fpos[Y] = size[HEIGHT] - window->title_height;

        CALL(window->title_label->set_pos)(window->title_label, fpos, NULL);
        CALL(window->title_label->set_size)(window->title_label, fsize, NULL);
    }

    vec2_set(subwindow_pos, 0, 0);
    vec2_set(subwindow_size, widget->size[WIDTH], widget->size[HEIGHT] - window->title_height);

    vec2_cp(subwindow_pos, subwidget->upos);
    vec2_cp(subwindow_size, subwidget->size);

//    printf("subwindow resized to: [%d,%d]\n", subwidget->size[WIDTH], subwidget->size[HEIGHT]);

/*    CALL(window->subwindow->set_pos)(window->subwindow, pos);
    CALL(window->subwindow->set_size)(window->subwindow, size); */

    return OK;
}

// window
static gboolean
ui_widget_window_screen_resize(widget_t *widget, int32_t width, int32_t height, GError **err)
{
    GError *tmp = NULL;
    widget_window_t *window = widget->data;
    widget_sizev_t size;
    gboolean ret;
    widget_posv_t lpos;
    widget_sizev_t lsize;

    assert(widget != NULL);
    assert(window != NULL);
    assert(window->subwindow != NULL);

    if (window->maximized != TRUE)
	return OK;

    assert(widget->parent != NULL);
    assert(widget->parent->type == UI_WIDGET_ROOT);

    vec2_cp(widget->parent->size, size);

    if ((ret = window->subwindow->set_size(window->subwindow, size, err)) != OK) {
	g_propagate_error(err, tmp);
	return ret;
    }

    return OK;
}

// window
static gboolean
ui_widget_window_draw(widget_t *widget, GError **err)
{
    widget_window_t *window = widget->data;
    real top, bot, left, right;
    char *key;
    vec3_t *v;

    assert(widget != NULL);
    assert(window != NULL);

    glBegin(GL_QUADS);
	key = "background";
	v = g_hash_table_lookup(widget->colors, key);
        glColor3v(*v);

        /* draw the window quad */
    	left  = widget->rpos[X];
	right = widget->size[WIDTH];
        bot   = widget->rpos[Y];
	top   = widget->size[HEIGHT];
    	glVertex3f(left,  bot, 0.0f);
	glVertex3f(right, bot, 0.0f);
	glVertex3f(right, top, 0.0f);
	glVertex3f(left,  top, 0.0f);


	/* draw the window title quad */
	key = "title";
	v = g_hash_table_lookup(widget->colors, key);
        glColor3v(*v);
    	left  = widget->rpos[X];
	right = widget->size[WIDTH];
        bot   = widget->size[HEIGHT] - window->title_height;
	top   = widget->size[HEIGHT];
    	glVertex3f(left,  bot, 0.0f);
	glVertex3f(right, bot, 0.0f);
	glVertex3f(right, top, 0.0f);
	glVertex3f(left,  top, 0.0f);
    glEnd();

    glLineWidth(1.0);
    glBegin(GL_LINES);
        /* outline the window */
	key = "border";
	v = g_hash_table_lookup(widget->colors, key);
        glColor3v(*v);
    	left  = widget->rpos[X];
	right = widget->size[WIDTH];
        bot   = widget->rpos[Y];
	top   = widget->size[HEIGHT];

    	glVertex3f(left,  bot, 0.0f);
	glVertex3f(right, bot, 0.0f);

	glVertex3f(right, bot, 0.0f);
	glVertex3f(right, top, 0.0f);

	glVertex3f(right, top, 0.0f);
	glVertex3f(left,  top, 0.0f);

	glVertex3f(left,  top, 0.0f);
    	glVertex3f(left,  bot, 0.0f);

	/* outline the window title */
	key = "title_border";
	v = g_hash_table_lookup(widget->colors, key);
        glColor3v(*v);
    	left  = widget->rpos[X];
	right = widget->size[WIDTH] - 1;
        bot   = widget->size[HEIGHT] - window->title_height;
	top   = widget->size[HEIGHT] - 1;
    	glVertex3f(left,  bot, 0.0f);
	glVertex3f(right, bot, 0.0f);

	glVertex3f(right, bot, 0.0f);
	glVertex3f(right, top, 0.0f);

	glVertex3f(right, top, 0.0f);
	glVertex3f(left,  top, 0.0f);

	glVertex3f(left,  top, 0.0f);
    	glVertex3f(left,  bot, 0.0f);
    glEnd();

    return OK;
}

// subwindow
void
ui_widget_window_set_color(widget_t *subwidget, char *key, vec3_t color)
{
    assert(subwidget != NULL);
    widget_t *widget = subwidget->parent;
    vec3_t *v;

    assert(widget != NULL);

    v = g_hash_table_lookup(widget->colors, key);
    if (v == NULL) {
	printerr("no color: %s\n", key);
	return;
    }
    vec3_cp(color, *v);
}

// subwindow
void
ui_widget_window_set_font(widget_t *subwidget, font_t *font, uint16_t font_size)
{
    assert(subwidget != NULL);
    widget_t *widget = subwidget->parent;
    assert(widget != NULL);
    assert(widget->type == UI_WIDGET_WINDOW);
    widget_window_t *window = widget->data;
    assert(window != NULL);

    ui_widget_label_set_font(window->title_label, font);
    ui_widget_label_set_font_size(window->title_label, font_size);

    window->title_font = font;
    window->title_font_size = font_size;
    window->title_height = ui_font_get_height(window->title_font, window->title_font_size) + (window->title_label_padding * 2) - 2;
//printf("wtf: %d %d %f %d %d\n", window->title_height, window->title_font_size, window->title_font->hratio, window->title_font->chrsize[HEIGHT], window->title_font->chrsize[WIDTH]);

    // recalculate subwindow size because the titlebar size has changed
    CALL(subwidget->set_size)(subwidget, widget->size, NULL);
}

// subwindow
void
ui_widget_window_set_text(widget_t *subwidget, char *txt)
{
    assert(subwidget != NULL);
    widget_t *widget = subwidget->parent;
    assert(widget != NULL);
    assert(widget->type == UI_WIDGET_WINDOW);
    widget_window_t *window = widget->data;
    assert(window != NULL);

    if (window->title_text != NULL) {
	g_free(window->title_text);
	window->title_text = strdup(txt);
    }

    ui_widget_label_set_text(window->title_label, txt);
}

// subwindow
void
ui_widget_window_set_fullscreen(widget_t *subwidget, gboolean state)
{
    assert(subwidget != NULL);
    widget_t *widget = subwidget->parent;
    assert(widget != NULL);
    widget_window_t *window = widget->data;
    assert(window != NULL);

    window->maximized = state;
}

widget_t *
ui_widget_window_new(
	widget_t *parent,
	widget_posv_t pos, widget_sizev_t size)
{
    GHashTable *t;
    widget_t *widget;
    widget_window_t *window;
    widget_posv_t label_pos;
    widget_sizev_t label_size;
    widget_posv_t subwindow_pos;
    widget_sizev_t subwindow_size;
    char *key;
    vec3_t *v;

    // create new widget
    widget = ui_widget_new(parent, UI_WIDGET_WINDOW, NULL);

    // set event handlers
    widget->draw = &ui_widget_window_draw;
    widget->screen_resize = &ui_widget_window_screen_resize;

    // set initial position & size
    widget->set_pos(widget, pos, NULL);
    widget->set_size(widget, size, NULL);

    // set up colors
    widget->colors = g_hash_table_new_full(g_str_hash, g_str_equal,
	    NULL, (void (*)(gpointer))vec3_destroy);
    t = widget->colors;

    g_hash_table_insert(t, "background",   vec3_new(0.2,  0.2,  0.3));
    g_hash_table_insert(t, "border",       vec3_new(0.6,  0.6,  0.6));
    g_hash_table_insert(t, "title",        vec3_new(0.0,  0.03, 0.25));
    g_hash_table_insert(t, "title_border", vec3_new(1.0,  1.0,  1.0));
    g_hash_table_insert(t, "label",        vec3_new(0.5,  0.0,  0.9));

    window = widget->data;
    window->maximized = TRUE;	// default to maximized state

    // create the subwindow
    window->subwindow = ui_widget_new(widget, UI_WIDGET_SUBWINDOW, NULL);
    window->subwindow->set_size = &ui_widget_subwindow_set_size;

    // default font settings
    window->title_font = ui_font_get("medium");
    window->title_font_size = 26;
    window->title_label_padding = 5;
    window->title_height = ui_font_get_height(window->title_font, window->title_font_size) + (window->title_label_padding * 2) - 2;
    vec2_set(label_pos, 0, 0);
    vec2_set(label_size, size[WIDTH], window->title_height);

    // create label widget
    key = "label";
    v = g_hash_table_lookup(widget->colors, key);
    glColor3v(*v);
    window->title_label = ui_widget_label_new(widget, "Untitled", window->title_font, *v, label_pos, label_size);
    ui_widget_label_set_alignment(window->title_label, UI_ALIGN_CENTER | UI_ALIGN_MIDDLE);

    // configure sub window
    vec2_set(subwindow_pos, 0, 0);
    vec2_set(subwindow_size, size[WIDTH], size[HEIGHT] - window->title_height);
    CALL(window->subwindow->set_pos)(window->subwindow, pos, NULL);
    CALL(window->subwindow->set_size)(window->subwindow, size, NULL);

    return window->subwindow;
}

