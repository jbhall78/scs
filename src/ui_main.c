#include <glib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "scs.h"
#include "shared.h"
#include "ui.h"
#include "client.h"
#include "sys_gl.h"

void
ui_widget_calculate_relativity(widget_t *widget)
{
    slist_t *lineage = NULL;
    slist_t *item = NULL;
    widget_t *w, *prev;
    int i;

    assert(widget != NULL);

//    print("start\n");

    // work our way torwards root,
    // building a list of pointers to all of the nodes along the way
    for (w = widget; w; w = w->parent) {
	item = slist_new();
	item->data = w;
	lineage = slist_add_start(lineage, item);
    }

    w = (widget_t *)lineage->data;
    assert(w != NULL);

    // compute the offset (root widget is first in list)
    vec2_zero(widget->rpos);
    prev = NULL;
    for (item = lineage; item; item = item->next) {
	w = (widget_t *)item->data;
	// user positions with a negative value calculates
	// a position relative to the parent's size[WIDTH,HEIGHT]
	// values
	//
	// this feature could end up being kludgy, but it is easy
	// to implement and I don't feel like adding complicated
	// alignment code right now
	if (w->upos[X] < 0.0) {
	    if (prev == NULL) {
		// this condition should not happen,
		// under normal circumstances
		// doing nothing will just pile the widgets in the
		// lower,left corner of the screen
		;
	    } else {
		widget->rpos[X] += prev->size[WIDTH] + w->upos[X];	// actually subtraction
	    }
	} else {
    	    widget->rpos[X] += w->upos[X];
	}

	// do the same for the height
	if (w->upos[Y] < 0.0) {
	    if (prev == NULL) {
		// this condition should not happen,
		// under normal circumstances
		// doing nothing will just pile the widgets in the
		// lower,left corner of the screen
		;
	    } else {
		widget->rpos[Y] += prev->size[HEIGHT] + w->upos[Y];	// actually subtraction
	    }
	} else {
	    widget->rpos[Y] += w->upos[Y];
	}

//	print("widget[0x%02x] offset: [%d,%d]\n", w->type, widget->rpos[X], widget->rpos[Y]);
	prev = w;
    }

//    print("end\n");
    slist_destroy(lineage, NULL);


    // recalculate rpos on child widgets
    for (i = 0; i < w->num_childs; i++) {
        ui_widget_calculate_relativity(widget->children[i]);
    }   
}

#if 0	// this feature is really broken now, maybe bring it back later with a thought-out design
gboolean
_ui_widget_set_visible(widget_t *w, gboolean visible, GError **err)
{
    assert(w != NULL);
    GError *tmp = NULL;
    int ret;


//print("asdasd uhm: %x %d\n", w->type, w->visible);

	// call this handler
	if ((ret = CALL(w->set_visible)(w, visible, &tmp)) != OK) {
	    g_propagate_error(err, tmp);
	    return ret;
	}

#if 0
    int i;
    // call child handlers
    for (i = 0; i < w->num_childs; i++) {
        if ((ret = ui_widget_set_visible(w->children[i], visible, &tmp)) != OK) {
	    g_propagate_error(err, tmp);
	    return ret;
	}
    }
#endif

    return OK;
}
#endif

gboolean
ui_widget_set_visible(widget_t *w, gboolean visible, GError **err)
{
    assert(w != NULL);
//    GError *tmp = NULL;
//    int ret;

    if (w->visible != visible) {
	w->visible = visible;
// this feature is really broken now, maybe bring it back later with a thought-out design
//	return _ui_widget_set_visible(w, visible, err);
    }

    return OK;
}

gboolean
ui_widget_toggle_visible(widget_t *w, GError **err)
{
    assert(w != NULL);
    gboolean visible = w->visible;

    ui_widget_set_visible(w, visible ^ 1, NULL);

    return OK;
}

static gboolean
ui_widget_parent_set_pos(widget_t *w, GError **err)
{
    assert(w != NULL);
    GError *tmp = NULL;
    int ret;
    int i;

    // call this handler
    if ((ret = CALL(w->parent_set_pos)(w, &tmp)) != OK) {
	g_propagate_error(err, tmp);
	return ret;
    }

    // call child handlers
    for (i = 0; i < w->num_childs; i++) {
        if ((ret = ui_widget_parent_set_pos(w->children[i], &tmp)) != OK) {
	    g_propagate_error(err, tmp);
	    return ret;
	}
    }

    return OK;
}


/* generic handlers to be used by widgets */
gboolean
_ui_widget_set_pos(widget_t *w, widget_posv_t pos, GError **err)
{
    GError *tmp = NULL;
    int ret;
    int i;

    vec2_cp(pos, w->upos);	// future

    ui_widget_calculate_relativity(w);

    for (i = 0; i < w->num_childs; i++) {
        if ((ret = ui_widget_parent_set_pos(w->children[i], &tmp)) != OK) {
            g_propagate_error(err, tmp);
            return ret;
        }
    }

    return OK;
}

gboolean
_ui_widget_set_size(widget_t *w, widget_sizev_t size, GError **err)
{
    vec2_cp(size, w->size);
    return OK;
}

/* generic widget functions */
widget_t *
ui_widget_new(widget_t *parent, widget_type_t type, GError **err)
{
    widget_t *p = parent;
    widget_t *w = g_malloc0(sizeof(widget_t));

    assert(w != NULL);

    w->type = type;
    w->parent = p;
    w->visible = TRUE;
    w->set_size = &_ui_widget_set_size;
    w->set_pos  = &_ui_widget_set_pos;


    if (p != NULL) {
	p->num_childs++;
	p->children = g_realloc(p->children,
				p->num_childs * sizeof(widget_t *));
 
	p->children[p->num_childs-1] = w;
	w->z = p->z;
    }

    /* allocate per widget data */
    switch (w->type) {
	case UI_WIDGET_ROOT:
	   w->data = g_malloc0(sizeof(widget_root_t));
	   break;
	case UI_WIDGET_WINDOW:
	   w->data = g_malloc0(sizeof(widget_window_t));
	   break;
	case UI_WIDGET_LABEL:
	   w->data = g_malloc0(sizeof(widget_label_t));
	   break;
	case UI_WIDGET_BUTTON:
	   w->data = g_malloc0(sizeof(widget_button_t));
	   break;
	case UI_WIDGET_TEXT:
	   w->data = g_malloc0(sizeof(widget_text_t));
	   break;
	case UI_WIDGET_CHECKBOX:
	   w->data = g_malloc0(sizeof(widget_checkbox_t));
	   break;
	case UI_WIDGET_CLIST:
	   w->data = g_malloc0(sizeof(widget_clist_t));
	   break;
	case UI_WIDGET_SELECT:
	   w->data = g_malloc0(sizeof(widget_select_t));
	   break;
	case UI_WIDGET_SCROLLBAR:
	   w->data = g_malloc0(sizeof(widget_scrollbar_t));
	   break;
	case UI_WIDGET_TOOLTIP:
	   w->data = g_malloc0(sizeof(widget_tooltip_t));
	default:
	   break;
    }

    return w;
}

gboolean
ui_widget_destroy(widget_t *w, GError **err)
{
#if 0
    struct {
	widget_type_t type;
	gboolean (*func)(widget_t *, GError **);
    } destroyfuncs[] = {
	{ UI_WIDGET_BUTTON,    ui_destroy_button    },
	{ UI_WIDGET_TEXT,      ui_destroy_text      },
	{ UI_WIDGET_CHECKBOX,  ui_destroy_checkbox  },
	{ UI_WIDGET_CLIST,     ui_destroy_clist     },
	{ UI_WIDGET_SELECT,    ui_destroy_select    },
	{ UI_WIDGET_SCROLLBAR, ui_destroy_scrollbar },
    };
#endif
    GError *tmp = NULL;
    int32_t i;
    int ret;

    /* loop over children and destroy them */
    for (i = 0; i < w->num_childs; i++) {
	ret = ui_widget_destroy(w->children[i], &tmp);
	if (ret != OK) {
	    g_propagate_error(err, tmp);
	    return ret;
	}
    }

    /* destroy the current widget */
#if 0
    for (i = 0; i < G_N_ELEMENTS(destroyfuncs); i++) {
	if (destroyfuncs[i].type == w->type) {
	    destroyfuncs[i].func(w, NULL);
	    break;
	}
    }
#endif

    ret = CALL(w->destroy)(w, &tmp);
    if (ret != OK) {
	g_propagate_error(err, tmp);
	return ret;
    }
    g_free(w->data);
    g_free(w);

    return OK;
}

void
ui_widget_root_set_cursor(widget_t *w, tex_t *tex)
{
    widget_root_t *wr = w->data;

    assert(wr != NULL);

    wr->cursor = tex;
}

widget_t *
ui_widget_get_root(widget_t *w)
{
    widget_t *p;

    assert(w != NULL);

    /* find the widget with no parents */
    for (p = w; p->parent; p = p->parent)
	;

    /* this should be a root widget */
    if (p->type == UI_WIDGET_ROOT)
	return p;
    else
	return NULL;
}

gboolean
ui_widget_is_mouse_over(widget_t *w, widget_posv_t rmpos)
{
    assert(w != NULL);

    widget_t *root = ui_widget_get_root(w);
    assert(root != NULL);

    widget_root_t *wr = root->data;
    assert(wr != NULL);

    widget_posv_t mpos;
    vec2_cp(wr->mpos, mpos);

    if (mpos[X] > w->rpos[X] && mpos[X] < w->rpos[X] + w->size[WIDTH] &&
	mpos[Y] > w->rpos[Y] && mpos[Y] < w->rpos[Y] + w->size[HEIGHT]) {
	if (rmpos != NULL) {
	    rmpos[X] = mpos[X] - w->rpos[X];
	    rmpos[Y] = mpos[Y] - w->rpos[Y];
	}
	return TRUE;
    }

    return FALSE;
}


#define ui_widget_get_focused ui_widget_get_active
widget_t *
ui_widget_get_active(widget_posv_t mpos, widget_t *w)
{
    widget_type_t allowed[] = {
	UI_WIDGET_BUTTON,
	UI_WIDGET_TEXT,
	UI_WIDGET_SLIDER,
	UI_WIDGET_SCROLLBAR,
	UI_WIDGET_CHECKBOX,
	UI_WIDGET_CLIST,
	UI_WIDGET_SELECT,
    };
    int i;
    widget_t *ret;

    /* test children first */
    for (i = 0; i < w->num_childs; i++) {
	if ((ret = ui_widget_get_active(mpos, w->children[i])) != NULL)
	    return ret;
    }

    /* now test ourselves */
    for (i = 0; i < G_N_ELEMENTS(allowed); i++) {
	if (w->type == allowed[i])
	    break;
    }
    if (i >= G_N_ELEMENTS(allowed))
	return NULL;

    if (w->visible == FALSE)
	return NULL;

#if 0
    if (w->type == UI_WIDGET_BUTTON && w->parent->type == UI_WIDGET_SELECT && w->visible == TRUE) {
	widget_button_t *button = w->data;
	widget_t *l = button->label;
	widget_label_t *label = l->data;
//	if (label->txt != NULL)
    	    print("[0x%08x] mouse:[%d,%d] widget:[%d,%d,%d,%d] %s\n", w, mpos[X], mpos[Y], w->pos[X], w->pos[Y], w->size[WIDTH], w->size[HEIGHT], label->txt);
    }
#endif
#if 0
    if (w->type == UI_WIDGET_SELECT) {
	print("[0x%08x] mouse:[%d,%d] widget:[%d,%d,%d,%d]\n", w, mpos[X], mpos[Y], w->upos[X], w->upos[Y], w->size[WIDTH], w->size[HEIGHT]);
    }
#endif

    if (mpos[X] > w->rpos[X] && mpos[X] < w->rpos[X] + w->size[WIDTH] &&
	mpos[Y] > w->rpos[Y] && mpos[Y] < w->rpos[Y] + w->size[HEIGHT]) {
//	if (w->type == UI_WIDGET_SELECT)
//	    print("select actived\n");
	return w;
    }

    return NULL;
}

gboolean
ui_mbutton(widget_t *root, SDL_MouseButtonEvent *ev, GError **err)
{
    GError *tmp = NULL;
    widget_posv_t pos;
    widget_t *active;
    widget_t *handled_by = NULL;
    static widget_t *mbutton_down_handler = NULL;
    int ret;

    // always send mouse button up events to the widget that handled
    // the mouse button down event
    if (ev->type == SDL_MOUSEBUTTONUP && mbutton_down_handler != NULL) {
	ret = CALL(mbutton_down_handler->mbutton)(mbutton_down_handler, ev, &handled_by, &tmp); 
	if (ret != OK) {
	    g_propagate_error(err, tmp);
	    return ret;
	}
	mbutton_down_handler = NULL;
	handled_by = NULL;
    }

    pos[X] = ev->x;
    pos[Y] = root->size[HEIGHT] - ev->y;
    active = ui_widget_get_active(pos, root);

    if (! active)
	return OK;

    if (active->mbutton != NULL) {
	ret = CALL(active->mbutton)(active, ev, &handled_by, &tmp); 
	if (ret != OK) {
	    g_propagate_error(err, tmp);
	    return ret;
	}
	mbutton_down_handler = handled_by;
    }

    return OK;
}

static gboolean
_ui_mmotion(widget_t *w, SDL_MouseMotionEvent *ev, widget_posv_t mpos, GError **err)
{
    GError *tmp = NULL;
    int i;
    gboolean ret;
    widget_posv_t rmpos;	// relative mouse position

    /* make sure widget is visibile */
    if (w->visible == FALSE)
	return OK;

    /* test children first */
    for (i = 0; i < w->num_childs; i++) {
	if ((ret = _ui_mmotion(w->children[i], ev, mpos, &tmp)) != OK) {
	    g_propagate_error(err, tmp);
	    return ret;
	}
    }

    if (! (mpos[X] > w->rpos[X] && mpos[X] < w->rpos[X] + w->size[WIDTH] &&
	mpos[Y] > w->rpos[Y] && mpos[Y] < w->rpos[Y] + w->size[HEIGHT])) {
	return OK;
    }

    rmpos[X] = mpos[X] - w->rpos[X];
    rmpos[Y] = mpos[Y] - w->rpos[Y];

    if ((ret = CALL(w->mmotion)(w, ev, rmpos, &tmp)) != OK) {
	g_propagate_error(err, tmp);
	return ret;
    }

    return OK;
}

gboolean
ui_mmotion(widget_t *w, SDL_MouseMotionEvent *ev, GError **err)
{
    assert(w != NULL);
    widget_t *root = ui_widget_get_root(w);
    GError *tmp = NULL;
    widget_posv_t mpos;
    gboolean ret;

    mpos[X] = ev->x;
    mpos[Y] = root->size[HEIGHT] - ev->y;

    if ((ret = _ui_mmotion(w, ev, mpos, &tmp)) != OK) {
	g_propagate_error(err, tmp);
	return ret;
    }

    return OK;
}

gboolean
ui_screen_resize(widget_t *widget, int32_t width, int32_t height, GError **err)
{
    GError *tmp = NULL;
    gboolean ret;
    int32_t i;

    if ((ret = CALL(widget->screen_resize)(widget, width, height, &tmp)) != OK) {
	g_propagate_error(err, tmp);
	return ret;
    }

    for (i = 0; i < widget->num_childs; i++) {
	ret = ui_screen_resize(widget->children[i], width, height, &tmp);
	if (ret != OK) {
	    g_propagate_error(err, tmp);
	    return ret;
	}
    }

    // force relative positions to be recalculated
    ui_widget_calculate_relativity(widget);

    return OK;
}

gboolean
ui_update(widget_t *w, GError **err)
{
    GError *tmp = NULL;
    int ret;
    int i;

    /* update widget */
    ret = CALL(w->update)(w, &tmp);
    if (ret != OK) {
	g_propagate_error(err, tmp);
	return ret;
    }

    /* loop over children and update them */
    for (i = 0; i < w->num_childs; i++) {
	ret = ui_update(w->children[i], NULL);
	if (ret != OK) {
	    g_propagate_error(err, tmp);
	    return ret;
	}
    }
    return OK;
}

gboolean
ui_key(widget_t *w, SDL_KeyboardEvent *k, gboolean *handled, GError **err)
{
    GError *tmp = NULL;
    widget_t *active, *r;
    widget_root_t *wr;
    int ret;
    widget_t *p;
#if 0
    uint16_t i;
    struct {
	widget_type_t type;
	gboolean (*func)(widget_t *, SDL_KeyboardEvent *, GError **);
    } keyfuncs[] = {
	{ UI_WIDGET_TEXT, ui_key_text },
    };
#endif

    r = ui_widget_get_root(w);
    r->update(r, NULL);

    wr = r->data;
    active = wr->active;

    /* no active widget, do nothing */
    if (! active) {
//	print("no active widget\n");
	return OK;
    }


    /* we have an active widget, pass the keystroke onto it */
#if 0
    print("start\n");
    print("grr: %d\n", active->type);
    if (active->type == UI_WIDGET_BUTTON) {
	widget_button_t *b = active->data;
	print("asd: %s\n", b->txt);
    }
#endif
    if ((ret = CALL(active->key)(active, k, handled, &tmp)) != OK) {
//	print("error\n");
	g_propagate_error(err, tmp);
        return ret;
    }
    if (*handled == FALSE) {
//	print("unhandled\n");
	// next, try to get the parents to handle the event
	for (p = active->parent; p; p = p->parent) {
//	    print("grr: %d\n", p->type);
	    if ((ret = CALL(p->key)(p, k, handled, &tmp)) != OK) {
		g_propagate_error(err, tmp);
		return ret;
	    }
	    if (*handled == TRUE)
		break;
	}
    }
//    print("end\n");

#if 0
    for (i = 0; i < G_N_ELEMENTS(keyfuncs); i++) {
	if (keyfuncs[i].type == active->type) {
	    keyfuncs[i].func(active, k, NULL);
	    break;
	}
    }
#endif
    return OK;
}

gboolean
ui_text(widget_t *w, SDL_TextInputEvent *e, gboolean *handled, GError **err)
{
    GError *tmp = NULL;
    widget_t *active, *r;
    widget_root_t *wr;
    int ret;
    widget_t *p;

    r = ui_widget_get_root(w);
    r->update(r, NULL);

    wr = r->data;
    active = wr->active;

    /* no active widget, do nothing */
    if (! active) {
    	return OK;
    }

    /* we have an active widget, pass the keystroke onto it */
    if ((ret = CALL(active->text)(active, e, handled, &tmp)) != OK) {
    	g_propagate_error(err, tmp);
        return ret;
    }
    if (*handled == FALSE) {
    	// next, try to get the parents to handle the event
	    for (p = active->parent; p; p = p->parent) {
	        if ((ret = CALL(p->text)(p, e, handled, &tmp)) != OK) {
		    g_propagate_error(err, tmp);
		    return ret;
	    }
	    if (*handled == TRUE)
		    break;
	}
}

#if 0
    for (i = 0; i < G_N_ELEMENTS(keyfuncs); i++) {
	if (keyfuncs[i].type == active->type) {
	    keyfuncs[i].func(active, k, NULL);
	    break;
	}
    }
#endif
    return OK;
}

/*
 * z-index handling
 */
void
ui_widget_set_z(widget_t *w, int16_t z)
{
    assert(w != NULL);

    widget_t *root = ui_widget_get_root(w);
    assert(root != NULL);

    widget_root_t *wr = root->data;
    assert(wr != NULL);

    if (z < wr->z[UI_MIN])
	wr->z[UI_MIN] = z;

    if (z > wr->z[UI_MAX])
	wr->z[UI_MAX] = z;

    w->z = z;
}

void
ui_widget_inc_z(widget_t *w)
{
    assert(w != NULL);

    if (w->z < INT16_MAX)
    	ui_widget_set_z(w, w->z + 1);
}

void
ui_widget_dec_z(widget_t *w)
{
    assert(w != NULL);

    if (w->z > INT16_MIN)
	ui_widget_set_z(w, w->z - 1);
}

/*
 * drawing functions
 */
#if 0
void
ui_draw_cursor(widget_t *w)
{
    widget_root_t *wr = w->data;
    real left, right, top, bot;
    vec3_t color = { 1.0, 1.0, 1.0 };

    assert(wr != NULL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, wr->cursor->id);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glColor3v(color);

    left  = wr->mpos[X];
    right = wr->mpos[X] + wr->cursor->size[WIDTH];
    top   = wr->mpos[Y];
    bot   = wr->mpos[Y] - wr->cursor->size[HEIGHT];

    glBegin(GL_QUADS);
	glTexCoord2f(0,  0);
	glVertex2d(left, bot);
	    
	glTexCoord2f(1, 0);
	glVertex2d(right, bot);
    
	glTexCoord2f(1, 1);
	glVertex2d(right, top);
    
	glTexCoord2f(0,  1);
	glVertex2d(left, top);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
}
#endif

#if 0
void
ui_draw_axis(vec3_t pos, double len)
{
    vec3_t color;
    vec3_t dir;
    vec3_t a;

    glDisable(GL_LIGHTING);

    glLineWidth(3.0);

    color[R] = 1.0;
    color[G] = 0.0;
    color[B] = 0.0;
    glColor3v(color);
    glBegin(GL_LINES);
        glVertex3f(pos[X], pos[Y], pos[Z]);
        glVertex3f(pos[X] + len, pos[Y], pos[Z]);
    glEnd();
    color[R] = 0.0;
    color[G] = 1.0;
    color[B] = 0.0;
    glColor3v(color);
    glBegin(GL_LINES);
        glVertex3f(pos[X], pos[Y], pos[Z]);
        glVertex3f(pos[X], pos[Y] + len, pos[Z]);
    glEnd();
    color[R] = 0.0;
    color[G] = 0.0;
    color[B] = 1.0;
    glColor3v(color);
    glBegin(GL_LINES);
        glVertex3f(pos[X], pos[Y], pos[Z]);
        glVertex3f(pos[X], pos[Y], pos[Z] + len);
    glEnd();

    glEnable(GL_LIGHTING);
}
#endif

#if 0
void
ui_draw_arrow(direction_t direction, uint16_t padding, widget_posv_t pos, widget_sizev_t size)
{
    float top, bot, left, right, cx, cy;

    top = pos[Y] + size[HEIGHT] - padding*2;
    bot = pos[Y] + padding*2;
    left = pos[X] + padding*2;
    right = pos[X] + size[WIDTH] - padding*2;
    cx = left + ((right - left) / 2);
    cy = bot + ((top - bot) / 2);

    // i could get more complicated with this
    // by applying rotation to a matrix &
    // using an angle as an argument
    // this gets what i want more precisely
    // though
    switch(direction) {
	case NORTH:
	    glBegin(GL_TRIANGLES);
	    glVertex2f(left, bot);
	    glVertex2f(right, bot);
	    glVertex2f(cx, top);
	    glEnd();
	    break;
	case SOUTH:
	    glBegin(GL_TRIANGLES);
	    glVertex2f(cx, bot);
	    glVertex2f(right, top);
	    glVertex2f(left, top);
	    glEnd();
	    break;
	case WEST:
	    glBegin(GL_TRIANGLES);
	    glVertex2f(right, bot);
	    glVertex2f(right, top);
	    glVertex2f(left, cy);
	    glEnd();
	    break;
	case EAST:
	    glBegin(GL_TRIANGLES);
	    glVertex2f(left, bot);
	    glVertex2f(right, cy);
	    glVertex2f(left, top);
	    glEnd();
	    break;
	default:
	    break;
    }
}
#endif

gboolean
_ui_draw(widget_t *widget, int16_t z, slist_t **tooltips, GError **err)
{
    GError *tmp = NULL;
    int i, ret;

    // don't draw children this widget is not visible
    if (widget->visible == FALSE)
	return OK;

    // handle special case for tooltips
    if (widget->type == UI_WIDGET_TOOLTIP) {
	if (tooltips != NULL) {
	    slist_t *item = slist_new();
	    item->data = widget;
	    *tooltips = slist_append(*tooltips, item);
	    return OK;
	} else {
	    widget->z = z;
	}
    }

    // draw this widget
    if (widget->z == z) {
	ret = CALL(widget->draw)(widget, &tmp);
	if (ret != OK) {
	    g_propagate_error(err, tmp);
	    return ret;
	}
    }

    /* loop over children and draw them */
    for (i = 0; i < widget->num_childs; i++) {
	ret = _ui_draw(widget->children[i], z, tooltips, NULL);
	if (ret != OK) {
	    g_propagate_error(err, tmp);
	    return ret;
	}
    }

    return OK;
}

gboolean
ui_draw(widget_t *widget, GError **err)
{
    widget_root_t *wr;
    GError *tmp = NULL;
    int j;
    int ret;
    slist_t *list = NULL;
    slist_t *iter;

    if (widget->type == UI_WIDGET_ROOT) {
	wr = widget->data;

	for (j = wr->z[UI_MIN]; j <= wr->z[UI_MAX]; j++) {
	    if ((ret = _ui_draw(widget, j, &list, &tmp)) != OK) {
		g_propagate_error(err, tmp);
		return ret;
	    }
	}

	/* draw the mouse cursor (not an actual widget (yet)) */
	ui_draw_cursor(widget);

	/* draw tooltips last */
	for (iter = list; iter; iter = iter->next) {
	    if ((ret = _ui_draw((widget_t *)iter->data, j, NULL, &tmp)) != OK) {
		g_propagate_error(err, tmp);
		return ret;
	    }
	}	    
	slist_destroy(list, NULL);
    }

    return OK;
}

widget_t *
ui_init(widget_posv_t pos, widget_sizev_t size, GError **err)
{
    widget_t *w = ui_widget_root_new(pos, size);

    return w;
}
