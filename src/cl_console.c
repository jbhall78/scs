#include <string.h>

#include <SDL_opengl.h>
#include <glib.h>

#include "scs.h"
#include "shared.h"
#include "ui.h"
#include "client.h"
#include "sys_gl.h"

#define CONSOLE_SCROLLBACK_LINES 1000

void
con_resize(void)
{
    cl_console_t *con = &client.console;
    con->height = client.res[HEIGHT];
    con->width  = client.res[WIDTH];
    print("console resized\n");
}

void
con_add_line(char *txt)
{
    cl_console_t *con = &client.console;
    GList *l;

    if (g_list_length(con->txtbuf) == CONSOLE_SCROLLBACK_LINES) {
	l = g_list_last(con->txtbuf);

	g_free(l->data);

	con->txtbuf = g_list_remove_link(con->txtbuf, l);
    }

    con->txtbuf = g_list_prepend(con->txtbuf, txt);
}

void
con_add_text(char *txt)
{
    cl_console_t *con = &client.console;
    int32_t w, len;
    char *buf, *p;

    w = (con->width - con->pad * 2 - con->fntsize) / con->fntsize;
    len = strlen(txt);
    p = txt;
    buf = g_malloc(w+1);
 
    /* wrap text if needed */
    while (len > 0) {
	memset(buf, 0, w+1);

	if (strlen(p) > w)
	    memcpy(buf, p, w);
	else
	    memcpy(buf, p, strlen(p));

	con_add_line(strdup(buf));

	len -= w;
	p   += w;
    }

    g_free(buf);
}

void
con_exec(char *str)
{
    GError *tmp = NULL;
    char buf[BUFSIZ];

    if (! sh_exec(client.shell, str, &tmp)) {
	snprintf(buf, BUFSIZ, "ERROR: %s\n", tmp->message);
	con_add_text(buf);
	g_error_free(tmp);
    }

    return; 
}

void
con_proc_key(int keysym, int mod)
{
    cl_console_t *con = &client.console;
    uint32_t key;
    char *buf;
    dlist_t *item;

    switch (keysym) {
	case SDLK_BACKSPACE:
	    if (con->pos) {
		g_array_remove_index(con->inbuf, --con->pos);
//		resetCursor();
	    }
	    break;
	case SDLK_RETURN:
	    if (! con->inbuf->len)
		break;

	    buf = g_malloc0(strlen(con->prompt) + con->inbuf->len + 1);

	    // add buffer to scrollback
	    sprintf(buf, "%s%s", con->prompt, con->inbuf->data);
	    con_add_text(buf);
	    con_exec(con->inbuf->data);

	    // add history item
	    item = dlist_new();
	    item->data = strdup(con->inbuf->data);

	    con->history = dlist_append(con->history, item);
	    printf("added: 0x%08lx (%s)\n", (uint64_t)item, (char *)item->data);
	    con->history_item = item;
	    printf("links: 0x%08lx 0x%08lx\n", (uint64_t)item->prev, (uint64_t)item->next);

	    // clear buffer
	    g_array_remove_range(con->inbuf, 0, con->inbuf->len);
	    con->pos = 0;

	    g_free(buf);

	    break;
	case SDLK_PAGEUP:
	    if (con->scroll_start <= CONSOLE_SCROLLBACK_LINES)
		con->scroll_start += con->scroll_by;
	    break;
	case SDLK_PAGEDOWN:
	    con->scroll_start -= con->scroll_by;
	    if (con->scroll_start < 0)
		con->scroll_start = 0;
	    break;
	case SDLK_LEFT:
	    if (con->pos > 0)
		con->pos--;
	    break;
	case SDLK_RIGHT:
	    if (con->pos < con->inbuf->len)
		con->pos++;
	    break;
	case SDLK_DOWN:
	    if (con->history_item->next) {
		char *str;
		con->history_item = con->history_item->next;
		str = con->history_item->data;
		printf("current: 0x%08lx\n", (uint64_t)con->history_item);

		if (con->inbuf->len > 0)
		    g_array_remove_range(con->inbuf, 0, con->inbuf->len);
		g_array_insert_vals(con->inbuf, 0, str, strlen(str));
	    } else {
		printf("no history item next\n");
	    }
	    break;
	case SDLK_UP:

	    if (con->history_item) {
		char *str;
		str = con->history_item->data;
		if (con->inbuf->len > 0)
		    g_array_remove_range(con->inbuf, 0, con->inbuf->len);
		g_array_insert_vals(con->inbuf, 0, str, strlen(str));
		con->pos = strlen(str);

		if (con->history_item->prev)
		    con->history_item = con->history_item->prev;
		printf("current: 0x%08lx\n", (uint64_t)con->history_item);
	    } else {
		printf("no history item\n");
	    }
	    break;
	    /*
	case SDLK_SPACE:
	default:
	    // map unicode to ascii
	    if ((unicode & 0xFF80) == 0) {
		key = unicode & 0x7F;
		// make sure it is valid for input
		if (key >= con->fnt->range[LO] && key <= con->fnt->range[HI]) {
		    if (con->pos == con->inbuf->len)
			g_array_append_val(con->inbuf, key);
		    else
			g_array_insert_val(con->inbuf, con->pos, key);
		    con->pos++;
//		    resetCursor();
		}
	    }
	    break;
	    */
    }
}

void
con_key(SDL_KeyboardEvent *k)
{
    if (k->type == SDL_KEYUP)
	return;

    con_proc_key(k->keysym.sym, k->keysym.mod);
}

void
con_text(char c)
{

    cl_console_t *con = &client.console;
    if (c >= con->fnt->range[LO] && c <= con->fnt->range[HI]) {
	if (con->pos == con->inbuf->len)
	    g_array_append_val(con->inbuf, c);
	else
	    g_array_insert_val(con->inbuf, con->pos, c);
	con->pos++;
    }
}

void
con_draw_input(void)
{
    static gboolean cursor_visible = FALSE;
    cl_console_t *con = &client.console;
    real y, x;
    char buf[strlen(con->prompt)+con->inbuf->len+1];

    y = (client.res[HEIGHT] - con->height + con->pad);

    sprintf(buf, "%s%s", con->prompt, con->inbuf->data);

    ui_font_draw_text(con->fnt, con->pad, y, con->fntsize, con->color, buf);

    x = con->pad + ui_font_get_strwidth(con->fnt, con->fntsize, strlen(con->prompt) + con->pos);

    // draw text cursor
    if (sys_timer_is_ready(con->cursor_timer)) {
	cursor_visible ^= 1;
    }
    if (cursor_visible) {
        glBegin(GL_QUADS);
            glVertex2f(x, y);
	    glVertex2f(x + con->fntsize, y);
	    glVertex2f(x + con->fntsize, y + con->cursor_height);
	    glVertex2f(x, y + con->cursor_height);
        glEnd();
    }
}

void
con_draw_text(void)
{
    cl_console_t *con = &client.console;
    uint16_t i, n;
    GList *line;
    real y, off;

    n = (con->height - con->fntheight * 2 - con->pad * 2) / (con->fntheight + con->pad);

    off = (con->fntheight + con->pad) * 2;

    for (line = con->txtbuf, i = 0; line; line = line->next, i++) {
	if (i < con->scroll_start)
	    continue;

	if (i - con->scroll_start >= n)
	    break;

	y = (client.res[HEIGHT] - con->height) + off + ((con->fntheight + con->pad) * (i - con->scroll_start)); 
    	ui_font_draw_text(con->fnt, con->pad, y, con->fntsize, con->color, line->data);
    }

}

void
con_draw(void)
{
    cl_console_t *con = &client.console;
    real left, right, top, bot;

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
	glLoadIdentity();

	glOrtho(0.0, client.res[WIDTH], 0.0, client.res[HEIGHT], -1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	    glLoadIdentity();

	    glEnable (GL_TEXTURE_2D);
	    glBindTexture (GL_TEXTURE_2D, con->tex->id);
    	    glColor4f(1.0, 1.0, 1.0, 1.0);

	    glBegin (GL_QUADS);
    		top = client.res[HEIGHT];
		bot = client.res[HEIGHT] - con->height;
		left = 0.0;
		right = client.res[WIDTH];

		glTexCoord2f (0.0f,0.0f);  glVertex3f (left, bot, 0.0f);
		glTexCoord2f (2.0f, 0.0f); glVertex3f (right, bot, 0.0f);
		glTexCoord2f (2.0f, 2.0f); glVertex3f (right, top, 0.0f);
		glTexCoord2f (0.0f, 2.0f); glVertex3f (left, top, 0.0f);
	    glEnd ();

	    glShadeModel(GL_FLAT);
	    con_draw_text();
	    con_draw_input();
	    glShadeModel(GL_SMOOTH);

	    glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
}

gboolean
con_load(GError **err)
{
    cl_console_t *con = &client.console;

    con->enabled = TRUE;

    return OK;
}

gboolean
con_unload(GError **err)
{
    cl_console_t *con = &client.console;

    con->enabled = FALSE;

    return OK;
}

void
con_toggle(void)
{
    cl_console_t *con = &client.console;

    if (con->enabled)
	con_unload(NULL);
    else
	con_load(NULL);
}

gboolean
con_init(GError **err)
{
    cl_console_t *con = &client.console;

    con->fnt = ui_font_get("console");
    con->fntsize = 8;
    con->fntheight = ui_font_get_height(con->fnt, con->fntsize);
    con->tex = tex_load("console_bg.png");
    con->inbuf = g_array_new(TRUE, TRUE, sizeof(char));
    con->prompt = strdup("--> ");
    con->height = client.res[HEIGHT];
    con->width  = client.res[WIDTH];
    con->scroll_by = 8;
    con->scroll_start = 0;
    con->pad = 2;
    con->cursor_height = 5;
    con->cursor_blink_interval = 750;
    con->cursor_timer = sys_timer_new(scs.clock);
    sys_timer_set_interval_ms(con->cursor_timer, con->cursor_blink_interval);
    sys_timer_set_repeating(con->cursor_timer, TRUE);
    sys_timer_reset(con->cursor_timer);
    vec3_set(con->color, 1, 1, 1);

    con->initialized = TRUE;

    con_add_text("console initialized");
    con_add_text("===================");

    return OK;
}

gboolean
con_shutdown(GError **err)
{
    cl_console_t *con = &client.console;
    GList *list;

    /* clear text buffer */
    for (list = con->txtbuf; list; list = list->next) 
	g_free(list->data);

    g_list_free(con->txtbuf);
    con->txtbuf = NULL;

    g_free(con->prompt);
    con->prompt = NULL;

    return OK;
}
