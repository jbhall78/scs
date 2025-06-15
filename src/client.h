#ifndef _CLIENT_H
#define _CLIENT_H

#ifndef DEDICATED

#include <SDL2/SDL.h>
#include <gio/gio.h>

#include "ui.h"
#include "shell.h"
#include "net.h"
#include "snd.h"
#include "cl_light.h"
#include "timer.h"
#include "game.h"

#define	CL_HZ		30
#define	FPS		30

/* extend SDL keycodes with mouse/joystick buttons */
#define SDLK_LAST 100000
#define SDLK_EX_MOUSE_FIRST (SDLK_LAST+100)
#define SDLK_EX_MOUSE1	(SDLK_LAST+101)
#define SDLK_EX_MOUSE2	(SDLK_LAST+102)
#define SDLK_EX_MOUSE3	(SDLK_LAST+103)
#define SDLK_EX_MOUSE4	(SDLK_LAST+104)
#define SDLK_EX_MOUSE5	(SDLK_LAST+105)
#define SDLK_EX_MOUSE6	(SDLK_LAST+106)
#define SDLK_EX_MOUSE7	(SDLK_LAST+107)
#define SDLK_EX_MOUSE8	(SDLK_LAST+108)
#define SDLK_EX_MOUSE9	(SDLK_LAST+109)
#define SDLK_EX_MOUSE10	(SDLK_LAST+110)
#define SDLK_EX_MOUSE11	(SDLK_LAST+111)
#define SDLK_EX_MOUSE12	(SDLK_LAST+112)
#define SDLK_EX_MOUSE13	(SDLK_LAST+113)
#define SDLK_EX_MOUSE14	(SDLK_LAST+114)
#define SDLK_EX_MOUSE15	(SDLK_LAST+115)

#define SDLK_EX_JOY1	(SDLK_LAST+201)
#define SDLK_EX_JOY2	(SDLK_LAST+202)
#define SDLK_EX_JOY3	(SDLK_LAST+203)
#define SDLK_EX_JOY4	(SDLK_LAST+204)
#define SDLK_EX_JOY5	(SDLK_LAST+205)
#define SDLK_EX_JOY6	(SDLK_LAST+206)
#define SDLK_EX_JOY7	(SDLK_LAST+207)
#define SDLK_EX_JOY8	(SDLK_LAST+208)
#define SDLK_EX_JOY9	(SDLK_LAST+209)
#define SDLK_EX_JOY10	(SDLK_LAST+210)
#define SDLK_EX_JOY11	(SDLK_LAST+211)
#define SDLK_EX_JOY12	(SDLK_LAST+212)
#define SDLK_EX_JOY13	(SDLK_LAST+213)
#define SDLK_EX_JOY14	(SDLK_LAST+214)
#define SDLK_EX_JOY15	(SDLK_LAST+215)
#define SDLK_EX_JOY16	(SDLK_LAST+216)
#define SDLK_EX_JOY17	(SDLK_LAST+217)
#define SDLK_EX_JOY18	(SDLK_LAST+218)
#define SDLK_EX_JOY19	(SDLK_LAST+219)
#define SDLK_EX_JOY20	(SDLK_LAST+220)
#define SDLK_EX_JOY21	(SDLK_LAST+221)
#define SDLK_EX_JOY22	(SDLK_LAST+222)
#define SDLK_EX_JOY23	(SDLK_LAST+223)
#define SDLK_EX_JOY24	(SDLK_LAST+224)
#define SDLK_EX_JOY25	(SDLK_LAST+225)
/* if anyone needs more mouse or joystick buttons they can use shift|ctrl|alt modifiers */

#define BIND_MOD_CTRL	(1 << 0)
#define BIND_MOD_ALT	(1 << 1)
#define BIND_MOD_SHIFT  (1 << 2)

typedef struct cl_callbacks_s {
    gboolean (*load)(GError **);
    gboolean (*unload)(GError **);
    gboolean (*update)(GError **);
    gboolean (*draw)(GError **);

    /* event handlers */
    gboolean (*key)(SDL_KeyboardEvent *, GError **);
    gboolean (*mbutton)(SDL_MouseButtonEvent *, GError **);
    gboolean (*mmotion)(SDL_MouseMotionEvent *, GError **);
    gboolean (*jbutton)(GError **);
    gboolean (*jmotion)(GError **);
    gboolean (*jhat)(GError **);
    gboolean (*screen_resize)(int32_t w, int32_t h, GError **);
} cl_callbacks_t;

typedef struct cl_console_s {
    gboolean initialized;
    gboolean enabled;

    char *prompt;
    uint16_t height;
    uint16_t width;
    uint8_t pad;

    GList *txtbuf;
    font_t *fnt;
    vec3_t color;
    uint8_t fntsize;
    uint8_t fntheight;

    tex_t *tex;

    GArray *inbuf;

    uint16_t scroll_by;
    int16_t scroll_start;

    int16_t cursor_height;
    uint32_t cursor_blink_interval;
    sys_timer_t *cursor_timer;

    dlist_t *history;
    dlist_t *history_item;

    uint16_t pos;	// cursor position
} cl_console_t;

/* _ALL_ CLIENT SIDE GLOBALS GO IN THIS STRUCTURE */
typedef struct client_s {
    GThread *thread;
    cl_callbacks_t *callbacks;

    gboolean initialized;

    /* video stuff */
    //SDL_Surface *surface;
    SDL_Window *window; // This replaces the concept of the main surface for display
    SDL_GLContext gl_context; // Your OpenGL rendering context
    gboolean fullscreen;
    gboolean video;
    uint16_vec2_t res;
    uint16_vec2_t center;
    uint16_vec2_t ortho;
    uint16_vec2_t fullscreen_res;
    uint16_vec2_t prev_res;
    uint8_t bpp;
    real fov, znear, zfar;
    mat4x4_t proj_mat;
    light_group_t *light_group;

    /* input stuff */
    gboolean mouse_drag_enabled;
    gboolean mouse_glide;
    gboolean mouse_invert;
    real mouse_sensitivity;
    real mouse_glide_sensitivity;

    /* sound stuff */
    gboolean snd_initialized;
    snd_listener_t snd_listener;
    GHashTable *sounds;

    /* console / shell related */
    cl_console_t console;
    sh_state_t *shell;
    GHashTable *bindings;

    /* user interface / widget toolkit stuff */
    ui_t ui;

    /* game stuff */
    game_type_t game_type;
    GHashTable *objects;
    GArray *targets;
    int hud_enabled;

    /* data */
    GCache *meshes;

    /* thread stuff */
    GAsyncQueue *queue;

    /* net stuff */
    net_state_t *net;
    conn_t *conn; 

    /* player info */
    int8_t player_id;
    uint32_t obj_id;
    vec3_t posv, rotv;	/* velocities */
} client_t;

typedef struct {
    uint32_t	id;
    char	*str;
} cl_bind_map_t;

typedef enum {
    BIND_NONE = 0x00,
    BIND_UP,
    BIND_DOWN,
    BIND_UPDOWN,
    BIND_UPDATE,
} bind_state_t;

typedef struct {
    uint8_t modifiers;
    bind_state_t state;    
    char *action;
} binding_t;

extern client_t client;
extern cl_callbacks_t menu_callbacks;
extern cl_callbacks_t game_callbacks;
extern cl_callbacks_t newton_demo_callbacks;
extern cl_callbacks_t globe_demo_callbacks;

gboolean vid_init(GError **);
void vid_set(gboolean, uint16_t, uint16_t);
void vid_toggle_fullscreen(void);
void vid_resize(gboolean fullscreen, int32_t x, int32_t y);
void vid_screenshot(void);
void vid_perspective(float fov, float aspect, float zNear, float zFar);
gboolean vid_set_mode(void);


gboolean cl_mode(cl_callbacks_t *cb, GError **err);
gboolean cl_io_launch(GError **err);

void cl_shutdown(int);

gboolean tex_init(GError **err);
tex_t *tex_load(char *name);
void tex_refresh_unload(void);
void tex_refresh_load(void);

void cl_send_conn_req(conn_t *conn);
void cl_send_conn_disco(conn_t *conn);

void con_toggle(void);
void con_draw(void);
void con_key(SDL_KeyboardEvent *);
void con_add_text(char *);
gboolean con_init(GError **err);
void con_resize(void);

gboolean bind_init(GError **err);
void bind_update(void);
void bind_key(SDL_KeyboardEvent *k);
void bind_mbutton(SDL_MouseButtonEvent *m);

void cl_proc_pkt(conn_t *conn, pkt_t *pkt);

void cl_radar_draw(void);
gboolean cl_draw(gpointer data);
void cl_hud_init(void);
void cl_crosshair_draw(void);
void cl_target_computer_draw(void);
void cl_target_draw(void);
void cl_target_computer3d_draw(void);

#endif

#endif
