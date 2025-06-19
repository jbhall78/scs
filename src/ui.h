#ifndef _UI_H
#define _UI_H

#include <glib.h>
#include <SDL.h>

#include "scs.h"
#include "tex.h"

#include "shared.h"

#include "timer.h"

// use these types to avoid compiler warnings
// when all references to uint16_vec2_t are removed
// we will change these to typedefs
#if 0
#define widget_size_t	int32_t
#define widget_pos_t	int32_t
#define widget_sizev_t	int32_vec2_t
#define widget_posv_t	int32_vec2_t
#else
typedef int32_t		widget_size_t;
typedef int32_t		widget_pos_t;
typedef int32_vec2_t	widget_sizev_t;
typedef int32_vec2_t	widget_posv_t;
#endif


#define UI_MIN	0
#define UI_MAX	1

#define UI_ZINDEX_DEFAULT	0

/* a window is locked if the user cannot move the window */
#define UI_WINDOW_LOCKED	(1 << 0)	
#define UI_WINDOW_TITLED	(1 << 1)
#define UI_WINDOW_BORDER	(1 << 2)

//#define UI_BUTTON_RADIO		(1 << 0)
//#define UI_BUTTON_CHECK		(1 << 1)
//#define UI_BUTTON_LABEL		(1 << 2)

#define UI_TEXT_PASSWORD	(1 << 0)
#define UI_TEXT_MULTI		(1 << 1)
#define UI_TEXT_SCROLLBAR	(1 << 2)

typedef enum {
    UI_WIDGET_NONE	= 0x00,
    UI_WIDGET_ROOT	= 0x01,
    UI_WIDGET_WINDOW	= 0x02,
    UI_WIDGET_LABEL	= 0x03,
    UI_WIDGET_BUTTON	= 0x04,
    UI_WIDGET_TEXT	= 0x05,
    UI_WIDGET_SLIDER	= 0x06,
    UI_WIDGET_PROGRESS	= 0x07,
    UI_WIDGET_SCROLLBAR	= 0x08,
    UI_WIDGET_CHECKBOX  = 0x09,
    UI_WIDGET_CLIST     = 0x10,
    UI_WIDGET_SELECT    = 0x11,
    UI_WIDGET_TOOLTIP   = 0x12,
    UI_WIDGET_SUBWINDOW = 0x13,		// actual window returned from window_new, used only for positioning
} widget_type_t;


#define UI_ALIGN_NONE		0		// no preference
#define UI_ALIGN_CENTER		(1 << 0)	// horizontal center
#define UI_ALIGN_MIDDLE		(1 << 1)	// vertical center
#define UI_ALIGN_LEFT		(1 << 2)	// left
#define UI_ALIGN_RIGHT		(1 << 3)	// right
#define UI_ALIGN_TOP		(1 << 4)	// top
#define UI_ALIGN_BOTTOM		(1 << 5)	// bottom

typedef enum {
    NONE  = 0,
    NORTH = 1,
    SOUTH = 2,
    WEST  = 3,
    EAST  = 4
} direction_t;

typedef struct font_s {
    char		*name;

    int16_vec2_t	texsize;
    int16_vec2_t	chrsize;
    int16_vec2_t	range;
    real		hratio;
    uint16_t		filter_method;
    vec4_t		glyphs[CHAR_MAX];
    uint32_t		texids[CHAR_MAX];

    uint16_t		ntextures;
    GPtrArray		*textures;
} font_t;

//#define ui_widget_set_visible(w,v) { w->visible = v; }
//#define ui_widget_toggle_visible(w) { w->visible ^= 1; }
typedef struct widget_s {
    /* widget info */
    widget_type_t	type;
    uint32_t		flags;
    gpointer		data;

    /* shared attributes */
//    deprecated widget_posv_t	pos;	// user specified position
    widget_posv_t	upos;		// user specified position
    widget_posv_t	rpos;		// relative position (calculated automatically)
    widget_sizev_t	size;
    int16_t		z;	// z index
    gboolean		visible;
    GHashTable		*colors;	// color table

    /* relationships */
    uint16_t num_childs;
    struct widget_s **children;
    struct widget_s *parent;

    /* event handlers */
    gboolean	(*key)(struct widget_s *w, SDL_KeyboardEvent *ev, gboolean *handled, GError **err);
    gboolean	(*text)(struct widget_s *w, SDL_TextInputEvent *ev, gboolean *handled, GError **err);
    gboolean	(*mbutton)(struct widget_s *w, SDL_MouseButtonEvent *ev, struct widget_s **handled_by, GError **err);
    gboolean	(*mmotion)(struct widget_s *w, SDL_MouseMotionEvent *ev, widget_posv_t rmpos, GError **err);
    gboolean	(*destroy)(struct widget_s *w, GError **err);
    gboolean    (*update)(struct widget_s *w, GError **err);
    gboolean	(*draw)(struct widget_s *w, GError **err);
    // set position & size overrides
    gboolean	(*set_pos)(struct widget_s *w, widget_posv_t pos, GError **err);
    gboolean	(*set_size)(struct widget_s *w, widget_sizev_t size, GError **err);
    // notification of focus
    gboolean	(*focus)(struct widget_s *w, GError **err);
    // notification of visibility updates
    // this feature is really broken now, maybe bring it back later with a thought-out design
//    gboolean	(*set_visible)(struct widget_s *w, gboolean visible, GError **err);
    // notification of "ancestor" position & size changes
    gboolean	(*parent_set_pos)(struct widget_s *w, GError **err);
    gboolean	(*parent_set_size)(struct widget_s *w, GError **err);
    gboolean	(*screen_resize)(struct widget_s *w, int32_t width, int32_t height, GError **err);
} widget_t;

#define ui_widget_window_maximize(w) ui_widget_window_set_maximized(w, TRUE)
#define ui_widget_window_minimize(w) ui_widget_window_set_maximized(w, FALSE)
typedef struct widget_window_s {
    widget_t	*title_label;
    uint16_t	title_height;
    font_t	*title_font;
    char	*title_text;
    uint16_t	title_label_padding;
    uint16_t	title_font_size;
    gboolean	maximized;
    widget_t	*subwindow;
} widget_window_t;

typedef struct widget_label_s {
    char		*txt;
    char		*txt_truncated;
    int			len;
    int			len_truncated;
    font_t		*font;
    uint16_t		font_size;
    int			align;
    vec3_t		color;
    widget_t		*tooltip;
} widget_label_t;

typedef struct widget_root_s {
    tex_t		*cursor;		// texture for mouse cursor (needs to be moved to mouse widget)
    widget_posv_t	mpos;			// current mouse position
    //deprecated widget_t	*active;		// (deprecated) widget with mouse focus
    widget_t	*active;		// (deprecated) widget with mouse focus
    widget_t		*focus_keyboard;	// widget with keyboard focus
    widget_t		*focus_mouse;		// widget with mouse focus
    int16_vec2_t	z;			// minimum/maximum z values
} widget_root_t;

#define ui_widget_button_set_data(w,d)	{ ((widget_button_t *)(w->data))->data = d; }
#define ui_widget_button_set_repeat_interval(w,n)	{ ((widget_button_t *)(w->data))->repeat_interval = n; }
//#define ui_widget_button_set_visible(w,v) { w->visible = v; }
//#define ui_widget_button_toggle_visible(w) { w->visible ^= 1; }
typedef struct widget_button_s {
    //deprecated GHashTable  *colors;
    GHashTable  *colors;
    widget_t	*label;
    uint16_t	label_padding;
    font_t	*font;
    uint16_t	font_size;
    char	*txt;
    gboolean	depressed;
    gboolean    edge;
    gboolean	repeating;
    sys_timer_t	*repeat_timer;
    direction_t arrow_direction;
    uint16_t    arrow_padding;
    void        *data;
    void	(*callback)(widget_t *w, void *data);
} widget_button_t;

typedef struct widget_tooltip_s {
    //deprecated GHashTable  *colors;
    GHashTable  *colors;
    widget_t	*label;
    uint16_t	label_padding;
    font_t	*font;
    uint16_t	font_size;
    uint32_t	idle;
    char	*txt;
    sys_timer_t	*timer;
    gboolean	ready;		// if timer is ready (since repeating is disabled, we have to keep track of this)
    gboolean	enabled;	// if tooltip is enabled (too complicated to use widget->visible)
} widget_tooltip_t;

typedef struct widget_text_s {
    //deprecated GHashTable *colors;
    GHashTable *colors;
    font_t *font;
    GArray *buf;
    uint16_t pos;
} widget_text_t;

typedef struct widget_checkbox_s {
    //deprecated GHashTable *colors;
    GHashTable *colors;
    gboolean checked;
} widget_checkbox_t;

typedef struct widget_clist_s {
    font_t *font;
    //deprecated GHashTable *colors;
    GHashTable *colors;
    int16_t selected;
    uint16_t cols;
    GArray *widths;

    uint16_t rows;
    GList *list;
    GPtrArray *data;
} widget_clist_t;

//#define ui_select_set_selected(w,d)	{ ((widget_select_t *)(w->data))->selected = d; }
typedef struct widget_select_s {
    font_t *font;
    uint16_t font_size;
    //deprecated GHashTable *colors;
    GHashTable *colors;
    int32_t selected;
    uint16_t items;
    uint16_t dropdown_size;
    GList *list;
    GPtrArray *data;
    GPtrArray *buttons;
    widget_t *button;		// dropdown/down arrow button
    widget_t *button_text;	// text (works as button)
    gboolean dropdown_extended;
    uint16_t scrollbar_width;
    widget_t *scrollbar;
} widget_select_t;

//#define ui_widget_scrollbar_set_items(w,n) { ((widget_scrollbar_t *)(w->data))->items = n; }
#define ui_widget_scrollbar_set_visible_items(w,n) { ((widget_scrollbar_t *)(w->data))->visible_items = n; }
#define ui_widget_scrollbar_set_item_size(w,n) { ((widget_scrollbar_t *)(w->data))->item_size = n; }
typedef struct widget_scrollbar_s {
    //deprecated GHashTable *colors;
    GHashTable *colors;
    void (*callback)(void);
    gboolean verticle;
    widget_t *button1;	// top or left
    widget_t *button2;	// bottom or right
    widget_posv_t slider_pos;
    widget_sizev_t slider_size;
    uint16_t button_size;
    uint16_t items;
    uint16_t item_size;
    uint16_t visible_items;
    uint16_t pos;
} widget_scrollbar_t;

typedef struct ui_s {
    widget_t *root;
} ui_t;

/* digital text */
typedef enum {
    DIGITAL_CHAR_NUM,
    DIGITAL_CHAR_COLON
} digital_char_type_t;

typedef struct {
    int mid_top   : 1;
    int left_top  : 1;
    int right_top : 1;
    int mid_mid   : 1;
    int left_bot  : 1;
    int right_bot : 1;
    int mid_bot   : 1;
} digital_num_parts_t;


gboolean ui_font_init(GError **err);
font_t *ui_font_get(char *);

/* event handlers */
gboolean ui_mbutton(widget_t *r, SDL_MouseButtonEvent *b, GError **err);
gboolean ui_mmotion(widget_t *r, SDL_MouseMotionEvent *m, GError **err);
gboolean ui_update(widget_t *r, GError **err);
gboolean ui_draw(widget_t *r, GError **err);

/* misc stuff */
widget_t *ui_init(widget_posv_t, widget_sizev_t, GError **);
gboolean ui_screen_resize(widget_t *root, int32_t width, int32_t height, GError **);
widget_t *ui_widget_new(widget_t *parent, widget_type_t type, GError **err);
void ui_widget_set_color(widget_t *w, vec3_t color);
void ui_widget_set_pos(widget_t *w, widget_posv_t pos);
void ui_widget_set_size(widget_t *w, widget_sizev_t size);
//deprecated void ui_widget_set_rect(widget_t *w, widget_posv_t pos, widget_sizev_t size);
void ui_widget_set_rect(widget_t *w, widget_posv_t pos, widget_sizev_t size);
void ui_widget_root_set_cursor(widget_t *w, tex_t *tex);
widget_t *ui_widget_get_root(widget_t *w);
gboolean ui_key(widget_t *, SDL_KeyboardEvent *, gboolean *, GError **);
widget_t *ui_widget_get_active(widget_posv_t mpos, widget_t *w);
void ui_widget_calculate_relativity(widget_t *widget);
gboolean _ui_widget_set_pos(widget_t *w, widget_posv_t pos, GError **err);
void ui_widget_set_z(widget_t *w, int16_t z);
void ui_widget_inc_z(widget_t *w);
void ui_widget_dec_z(widget_t *w);
gboolean ui_widget_toggle_visible(widget_t *w, GError **err);
gboolean ui_widget_set_visible(widget_t *w, gboolean visible, GError **err);
gboolean ui_widget_is_mouse_over(widget_t *w, widget_posv_t rmpos);




/* generic font stuff */
void ui_font_draw_text(font_t *, int16_t, int16_t, int16_t, vec3_t, char *, ...);
uint32_t ui_font_get_strwidth(font_t *fnt, uint32_t size, int strlen);
uint32_t ui_font_get_height(font_t *fnt, uint32_t size);

/* root widget */
widget_t *ui_widget_root_new(widget_posv_t pos, widget_sizev_t size);

/* label widget */
widget_t *ui_widget_label_new(widget_t *parent, char *txt, font_t *fnt, vec3_t color, widget_posv_t pos, widget_sizev_t size);
void ui_widget_label_set_text(widget_t *w, char *txt);
void ui_widget_label_set_font(widget_t *w, font_t *fnt);
void ui_widget_label_set_font_size(widget_t *w, uint16_t size);
void ui_widget_label_set_color(widget_t *w, vec3_t color);
void ui_widget_label_set_alignment(widget_t *w, int mask);

/* window widget */
widget_t *ui_widget_window_new(widget_t *parent, widget_posv_t, widget_sizev_t);
void ui_widget_window_set_color(widget_t *widget, char *key, vec3_t color);
void ui_widget_window_set_text(widget_t *widget, char *txt);
void ui_widget_window_set_font(widget_t *widget, font_t *fnt, uint16_t font_height);
void ui_widget_window_set_fullscreen(widget_t *widget, gboolean state);

#if 0 /* old api */
widget_t *ui_widget_window_new(widget_t *parent,
	char *, font_t *, uint16_t, widget_posv_t, widget_sizev_t);
void ui_widget_window_set_border(widget_t *w, uint8_t border, vec3_t color);
void ui_widget_window_set_title_border(widget_t *w, uint8_t border, vec3_t color);
void ui_widget_window_set_title(widget_t *w, uint16_t height, vec3_t color);
void ui_widget_window_set_color(widget_t *w, vec3_t color);
#endif

/* button widget */
widget_t *ui_widget_button_new(widget_t *, char *,
		widget_posv_t, widget_sizev_t, void (*)(widget_t *, void *));
void ui_widget_button_set_color(widget_t *w, char *key, vec3_t color);
gboolean ui_widget_destroy(widget_t *w, GError **err);
void ui_widget_button_set_edge(widget_t *w, gboolean edge);
// resize button & children correctly
//deprecated void ui_widget_button_set_rect(widget_t *w, widget_posv_t pos, widget_sizev_t size);
void ui_widget_button_set_rect(widget_t *w, widget_posv_t pos, widget_sizev_t size);
// set edge visual attribute
void ui_widget_button_set_edge(widget_t *w, gboolean edge);
// set font attribute for attached label
void ui_widget_button_set_font(widget_t *button, font_t *fnt, uint16_t size);
void ui_widget_button_set_arrow(widget_t *button, direction_t direction, uint16_t padding);
gboolean ui_widget_button_set_text(widget_t *w, char *txt, GError **err);
void ui_widget_button_set_repeating(widget_t *w, gboolean repeating);


/* text widget */
widget_t * ui_widget_text_new(widget_t *parent,
		widget_posv_t pos, widget_sizev_t size);
void ui_widget_text_set_color(widget_t *w, char *key, vec3_t color);
void ui_widget_text_set_text(widget_t *w, char *txt);
gboolean ui_key_text(widget_t *, SDL_KeyboardEvent *, GError **);
char *ui_widget_text_get_text(widget_t *);

/* checkbox widget */
widget_t *ui_widget_checkbox_new(widget_t *parent, gboolean checked,
			widget_posv_t pos, widget_sizev_t size);
void ui_widget_checkbox_set_color(widget_t *w, char *key, vec3_t color);
gboolean ui_widget_checkbox_get_checked(widget_t *w, GError **err);

/* clist widget */
widget_t *ui_widget_clist_new(widget_t *parent, char **hdrs, widget_size_t *sizes,
	int n, widget_posv_t pos, widget_sizev_t size);
void ui_widget_clist_set_color(widget_t *w, char *key, vec3_t color);
void ui_widget_clist_append(widget_t *w, char **arr, char *data);
char *ui_widget_clist_get_data(widget_t *w);
void ui_widget_clist_clear(widget_t *w);

/* select widget */
widget_t *ui_widget_select_new(widget_t *parent, widget_posv_t pos, widget_sizev_t size);
void ui_widget_select_set_color(widget_t *w, char *key, vec3_t color);
void ui_widget_select_append(widget_t *w, char *text, char *data);
char *ui_widget_select_get_data(widget_t *w);
void ui_widget_select_clear(widget_t *w);
void ui_widget_select_set_font(widget_t *sel, font_t *fnt, uint16_t size);
void ui_widget_select_set_dropdown(widget_t *sel, gboolean val);
void ui_select_set_selected(widget_t *w, int32_t idx);



/* scrollbar widget */
widget_t *ui_widget_scrollbar_new(widget_t *, gboolean,
	widget_posv_t, widget_sizev_t, void (*)(void));
void ui_widget_scrollbar_set_color(widget_t *w, char *key, vec3_t color);
void ui_widget_scrollbar_set_rect(widget_t *w, widget_posv_t pos, widget_sizev_t size);
#define ui_widget_scrollbar_scroll_left ui_widget_scrollbar_scroll_up
void ui_widget_scrollbar_scroll_up(widget_t *w);
#define ui_widget_scrollbar_scroll_right ui_widget_scrollbar_scroll_down
void ui_widget_scrollbar_scroll_down(widget_t *w);
void ui_widget_scrollbar_set_items(widget_t *w, uint16_t n);

/* tooltip widget */
widget_t *ui_widget_tooltip_new(widget_t *, char *txt);
gboolean ui_widget_tooltip_set_enable(widget_t *w, gboolean enable, GError **err);
gboolean ui_widget_tooltip_set_text(widget_t *w, char *txt, GError **err);

/* drawing functions */
void ui_draw_axis(vec3_t v, double len);
void ui_draw_cursor(widget_t *w);
void ui_draw_arrow(direction_t direction, uint16_t padding, widget_posv_t pos, widget_sizev_t size);
void ui_draw_3d_grid(real GRID_SCALE, real GRID_XSTEPS, real GRID_ZSTEPS);
void ui_draw_strange_2d_object(void);
void ui_draw_spiral_wireframe(real radius, real length);
void ui_draw_uv_sphere_wireframe(real radius, real long_ang, real lat_ang, vec3_t color_long, vec3_t color_lat);
void ui_draw_digital_text(char *str, int x, int y, int width, int height);
void ui_draw_earth(real x, real y, real z, real radius, int mode, real long_ang, real lat_ang, vec3_t color_long, vec3_t color_lat);


#endif
