#include "scs.h"
#include "ui.h"

#include "shared.h"

#if 0
static gboolean
ui_widget_label_focus(widget_t *w, GError **err)
{
    widget_label_t *wl = w->data;

    CALL(w->parent->focus)(w, err);
}
#endif

static gboolean
ui_widget_label_update(widget_t *w, GError **err)
{
    assert(w != NULL);

    widget_label_t *wl = w->data;
    assert(wl != NULL);

    widget_t *tooltip  = wl->tooltip;
    assert(tooltip != NULL);

    widget_tooltip_t *wt = tooltip->data;
    gboolean is_over;

    if (w->visible == FALSE)
	return OK;

    is_over = ui_widget_is_mouse_over(w, NULL);

    // enable if mouse is hovering, disable if it is not
    if (is_over) {
	if (wt->enabled == FALSE) {
	    if (wl->len_truncated != wl->len) {
//		printf("[%s] %d %d\n", wl->txt, wl->len_truncated, wl->len);
		ui_widget_tooltip_set_enable(wl->tooltip, TRUE, NULL);
	    }
	}
    } else {
	if (wt->enabled == TRUE)
	    ui_widget_tooltip_set_enable(wl->tooltip, FALSE, NULL);
    }

    return OK;
}

static gboolean
ui_widget_label_draw(widget_t *w, GError **err)
{
    widget_label_t *label = w->data;
    widget_size_t width;
    widget_posv_t pos;

    assert(label != NULL);

    if (label->len_truncated <= 0)
	return OK;

    width = ui_font_get_strwidth(label->font, label->font_size, strlen(label->txt_truncated));
    
    // calculate horizontal alignment
    if (label->align & UI_ALIGN_CENTER) {
	pos[X] = w->size[WIDTH] / 2 - (width / 2);
    } else if (label->align & UI_ALIGN_RIGHT) {
	pos[X] = w->size[WIDTH] - width;
    } else {
	pos[X] = 0;
    }
    // calculate vertical alignment
    if (label->align & UI_ALIGN_MIDDLE) {
	pos[Y] = w->size[HEIGHT] / 2 - (label->font_size / 2);
//	printf("size: %d  font_size: %d  pos: %d  middle: %d\n", w->size[HEIGHT], label->font_size, w->rpos[Y], pos[Y]);
    } else if (label->align & UI_ALIGN_TOP) {
	pos[Y] = w->size[HEIGHT] - label->font_size;
    } else {
	pos[Y] = 0;
    }

    ui_font_draw_text(label->font, w->rpos[X] + pos[X], w->rpos[Y] + pos[Y], label->font_size,
	    label->color, label->txt_truncated);

    return OK;
}

static void
ui_widget_label_recalculate_truncated_text(widget_t *w)
{
    widget_label_t *wl = w->data;
    uint32_t len;
    widget_size_t width;

    if (wl->txt_truncated != NULL)
	g_free(wl->txt_truncated);

    wl->txt_truncated = strdup(wl->txt);

    len = wl->len;

    width = ui_font_get_strwidth(wl->font, wl->font_size, strlen(wl->txt_truncated));
    while (width > w->size[WIDTH] && len > 0) {
	len--;
	wl->txt_truncated[len] = 0;
	width = ui_font_get_strwidth(wl->font, wl->font_size, strlen(wl->txt_truncated));
    }
    wl->len_truncated = len;

//    print("god damn: [%d,%d] [%d,%d] << [%d,%d] [%d,%d]\n", w->upos[X], w->upos[Y], w->size[WIDTH], w->size[HEIGHT],
//	    w->parent->upos[X], w->parent->upos[Y], w->parent->size[WIDTH], w->parent->size[HEIGHT]);

//    print("truncated text: %s\n", wl->txt_truncated);
}

void
ui_widget_label_set_font_size(widget_t *w, uint16_t size)
{
    widget_label_t *lbl = w->data;

    assert(lbl != NULL);

    lbl->font_size = size;

    ui_widget_label_recalculate_truncated_text(w);
}

void
ui_widget_label_set_font(widget_t *w, font_t *fnt)
{
    widget_label_t *lbl = w->data;

    assert(lbl != NULL);

    lbl->font = fnt;

    ui_widget_label_recalculate_truncated_text(w);
}

void
ui_widget_label_set_alignment(widget_t *w, int mask)
{
    assert(w != NULL);
    assert(w->type == UI_WIDGET_LABEL);
    widget_label_t *label = w->data;
    assert(label != NULL);

    label->align = mask;
}

void
ui_widget_label_set_text(widget_t *w, char *txt)
{
    char *failsafe = "";
    widget_label_t *wl = w->data;

    assert(wl != NULL);

    if (txt == NULL)
	txt = failsafe;

    if (wl->txt != NULL)
	g_free(wl->txt);

    wl->txt = strdup(txt);
    wl->len = strlen(txt); 

    ui_widget_label_recalculate_truncated_text(w);

    ui_widget_tooltip_set_text(wl->tooltip, wl->txt, NULL);
}

void
ui_widget_label_set_color(widget_t *w, vec3_t color)
{
    widget_label_t *wl = w->data;

    assert(wl != NULL);

    vec3_cp(color, wl->color);
}

static gboolean
ui_widget_label_destroy(widget_t *w, GError **err)
{
    widget_label_t *wl = w->data;

    if (wl->txt != NULL)
	g_free(wl->txt);

    if (wl->txt_truncated != NULL)
	g_free(wl->txt_truncated);

    return OK;
}

#if 1
gboolean
ui_widget_label_mmotion(widget_t *w, SDL_MouseMotionEvent *ev, widget_posv_t mpos, GError **err)
{
    widget_label_t *wl = w->data;
//    print("[0x%08d] label mouse motion: %d/%d\n", w, mpos[0], mpos[1]);

    // disable tooltip on mouse movement
    ui_widget_tooltip_set_enable(wl->tooltip, FALSE, NULL);

    return OK;
}
#endif

widget_t *
ui_widget_label_new(widget_t *parent,
	char *txt, font_t *fnt, vec3_t color,
	widget_posv_t pos, widget_sizev_t size)
{
    widget_t *w;
    widget_label_t *wl;

    assert(parent != NULL);
    assert(fnt != NULL);

    w = ui_widget_new(parent, UI_WIDGET_LABEL, NULL);

    vec2_cp(pos, w->upos);
    vec2_cp(size, w->size);

    /* default events */
    w->draw    = &ui_widget_label_draw;
    w->destroy = &ui_widget_label_destroy;
    w->update  = &ui_widget_label_update;
    w->mmotion = &ui_widget_label_mmotion;
    wl = w->data;

    wl->tooltip = ui_widget_tooltip_new(w, txt);
    ui_widget_tooltip_set_enable(wl->tooltip, FALSE, NULL);

    ui_widget_label_set_text(w, txt);
    ui_widget_label_set_font(w, fnt);
    ui_widget_label_set_font_size(w, size[HEIGHT]);
    ui_widget_label_set_color(w, color);

    w->set_pos(w, pos, NULL);
    w->set_size(w, size, NULL);

    return w;
}
