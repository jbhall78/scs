#include <SDL_opengl.h>
#include <glib.h>

#include "scs.h"
#include "shared.h"
#include "ui.h"
#include "sys_gl.h"

gboolean
ui_widget_clist_draw(widget_t *w, GError **err)
{
    widget_clist_t *wcl = w->data;
    widget_t *r;
    widget_root_t *wr;
    real top, bot, left, right;
    vec3_t *v;
    GList *l;
    widget_pos_t x, y;

    assert(w != NULL);
    assert(wcl != NULL);

    left  = w->rpos[X];
    right = w->rpos[X] + w->size[WIDTH];
    bot   = w->rpos[Y];
    top   = w->rpos[Y] + w->size[HEIGHT];

    r = ui_widget_get_root(w);
    wr = r->data;

    v = g_hash_table_lookup(wcl->colors, "background");
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

    /* outline the clist */
    v = g_hash_table_lookup(wcl->colors, "border");
    glColor3v(*v);
    glLineWidth(2.0);
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

    /* draw list items */
#if 1
    for (l = wcl->list, x = 0; l; l = l->next, x++) {
	char **vals = l->data;
	widget_posv_t pos;

	if (x == wcl->selected) {
	    widget_posv_t start, end;

	    start[X] = end[X] = w->rpos[X] + 4;
	    start[Y] = w->size[HEIGHT] + w->rpos[Y] - (20 * (x+1)) - 6; 
	    end[Y] = start[Y] - 16;
	    for (y = 0; y < wcl->cols; y++) 
		end[X] += g_array_index(wcl->widths, widget_pos_t, y) + 4;
	    end[X] -= 2;

	    v = g_hash_table_lookup(wcl->colors, "selection");
	    glColor3v(*v);
	    glBegin(GL_TRIANGLES);
	        glVertex2f(start[X], end[Y]);
   	        glVertex2f(end[X],   end[Y]);
	        glVertex2f(start[X], start[Y]);
	    
	        glVertex2f(start[X], start[Y]);
	        glVertex2f(end[X], end[Y]);
	        glVertex2f(end[X], start[Y]);
	    glEnd();
	}

	v = g_hash_table_lookup(wcl->colors, "text");
	glColor3v(*v);

	pos[X] = w->rpos[X] + 8;
	pos[Y] = w->size[HEIGHT] + w->rpos[Y] - (20 * (x+2)); 
	for (y = 0; y < wcl->cols; y++) {
	    ui_font_draw_text(wcl->font, pos[X], pos[Y], 8, *v, "%s", vals[y]);
	    pos[X] += g_array_index(wcl->widths, widget_pos_t, y) + 4;
	}	    
    }
#endif

    return OK;
}

static gboolean
ui_widget_clist_update(widget_t *w, GError **err)
{
    return OK;
}

static gboolean
ui_widget_clist_mbutton(widget_t *w, SDL_MouseButtonEvent *ev, widget_t **handled_by, GError **err)
{
    widget_t *root = ui_widget_get_root(w);
    assert(root != NULL);

    widget_posv_t pos;
    GList *l;
    widget_clist_t *wcl = w->data;
    widget_posv_t start, end;
    widget_pos_t x, y;

    if (ev->type == SDL_MOUSEBUTTONUP)
	return OK;

    pos[X] = ev->x;
//    pos[Y] = w->parent->size[HEIGHT] - ev->y;
    pos[Y] = root->size[HEIGHT] - ev->y;

    /* draw list items */
    for (l = wcl->list, x = 0; l; l = l->next, x++) {
	start[X] = end[X] = w->rpos[X] + 8;
	start[Y] = w->size[HEIGHT] + w->rpos[Y] - (20 * (x+1)); 
	end[Y] = start[Y] - 20;
	for (y = 0; y < wcl->cols; y++) 
	    end[X] += g_array_index(wcl->widths, widget_pos_t, y) + 4;

	if (pos[X] > start[X] && pos[X] < end[X] &&
		pos[Y] > end[Y] && pos[Y] < start[Y]) {
	    wcl->selected = x;
	    break;
	}
    }

    *handled_by = w;

    return OK;
}

static gboolean
ui_widget_clist_destroy(widget_t *w, GError **err)
{
    widget_clist_t *wcl = w->data;
    assert(wcl != NULL);
    g_hash_table_destroy(wcl->colors);
    return OK;
}

void
ui_widget_clist_append(widget_t *w, char **arr, char *data)
{
    widget_clist_t *wcl = w->data;

    wcl->list = g_list_append(wcl->list, g_strdupv(arr));
    g_ptr_array_add(wcl->data, strdup(data));
    wcl->rows++;
}

void
ui_widget_clist_clear(widget_t *w)
{
    GList *l;
    widget_clist_t *wcl = w->data;
    widget_pos_t i;

    wcl->selected = -1;

    for (l = wcl->list, i = wcl->rows-1; l; l = l->next, i--) {
	g_free(l->data);
	g_free(g_ptr_array_index(wcl->data, i));
	g_ptr_array_remove_index(wcl->data, i);
    }
    g_list_free(wcl->list);
    wcl->list = NULL;

    wcl->rows = 0;
}

char *
ui_widget_clist_get_data(widget_t *w)
{
    widget_clist_t *wcl = w->data;

    if (wcl->selected == -1)
	return NULL;

    return strdup(g_ptr_array_index(wcl->data, wcl->selected));
}

void
ui_widget_clist_set_color(widget_t *w, char *key, vec3_t color)
{
    widget_clist_t *wcl = w->data;
    vec3_t *v;

    assert(wcl != NULL);

    v = g_hash_table_lookup(wcl->colors, key);
    if (v == NULL) {
	printerr("no color: %s\n", key);
	return;
    }
    vec3_cp(color, *v);
}

widget_t *
ui_widget_clist_new(widget_t *parent, char **hdrs, widget_size_t *sizes, int n,
			widget_posv_t pos, widget_sizev_t size)
{
    widget_t *w, *b, *sb;
    widget_clist_t *wcl;
    GHashTable *t;
    widget_pos_t i;
    widget_posv_t bpos;
    widget_sizev_t bsize;

    /* create the columned list */
    w = ui_widget_new(parent, UI_WIDGET_CLIST, NULL);

    /* default event handlers */
    w->mbutton = &ui_widget_clist_mbutton;
    w->destroy = &ui_widget_clist_destroy;
    w->update  = &ui_widget_clist_update;
    w->draw    = &ui_widget_clist_draw;

    /* default data */
    w->set_pos(w, pos, NULL);
    w->set_size(w, size, NULL);
    wcl = w->data;

    wcl->font = ui_font_get("input");
    wcl->list = NULL;
    wcl->selected = -1;
    wcl->data = g_ptr_array_new();

    /* store the list widths */
    wcl->cols = n;
    wcl->widths = g_array_new(FALSE, TRUE, sizeof(widget_pos_t));
    for (i = 0; i < n; i++) 
	g_array_append_val(wcl->widths, sizes[i]);

    /* create buttons */
    bsize[HEIGHT] = 20;
    bpos[X] = 4;
    bpos[Y] = size[HEIGHT] - bsize[HEIGHT] - 4;
    for (i = 0; i < n; i++) {
	bsize[WIDTH] = sizes[i];
	b = ui_widget_button_new(w, hdrs[i], bpos, bsize, NULL);
	ui_widget_button_set_edge(b, FALSE);
	bpos[X] += bsize[WIDTH] + 4;
    }

    /* create scroll bar */
    bsize[HEIGHT] = size[HEIGHT] - bsize[HEIGHT] - 8;
    bsize[WIDTH] = 20;
    bpos[X] = size[WIDTH] - bsize[WIDTH] - 4;
    bpos[Y] = 4;
    sb = ui_widget_scrollbar_new(w, TRUE, bpos, bsize, NULL);

    /* set up colors */
    wcl->colors = g_hash_table_new_full(g_str_hash, g_str_equal,
	    NULL, (void (*)(gpointer))vec3_destroy);
    t = wcl->colors;

    g_hash_table_insert(t, "background",           vec3_new(0.0, 0.0,  0.0));
    g_hash_table_insert(t, "border",               vec3_new(0.6, 0.6,  0.6));
    g_hash_table_insert(t, "text",                 vec3_new(1.0, 1.0,  1.0));
    g_hash_table_insert(t, "selection",            vec3_new(0.0, 0.5,  0.5));

    return w;
}
