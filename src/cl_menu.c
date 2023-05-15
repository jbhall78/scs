#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <glib.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include "scs.h"
#include "shared.h"
#include "client.h"
#include "ui.h"
#include "sys_gl.h"

#include "config.h"

static widget_t *main_menu = NULL;
static widget_t *configuration_menu = NULL;
static widget_t *multiplayer_menu = NULL;
static widget_t *internet_menu = NULL;
static widget_t *film_menu = NULL;
static widget_t *engine_demos_menu = NULL;

static widget_t *root = NULL;

/* variables */
static widget_t *internet_hostname = NULL;
static widget_t *internet_port = NULL;
static widget_t *internet_demo = NULL;
static widget_t *film_demo = NULL;

static void menu_insert_demos(widget_t *w);
/*
 * Game Creation callbacks
 */
static void
menu_single_player(widget_t *w, void *data)
{
    client.game_type = GAME_SINGLE;
    cl_mode(&game_callbacks, NULL);
}

static void
menu_newton_demo(widget_t *w, void *data)
{
    GError *tmp = NULL;

#if 0 /* DISABLED DUE TO 32BIT ASSEMBLY */
    if (cl_mode(&newton_demo_callbacks, &tmp) != OK) {
        printerr("Client failed to load: %s\n", tmp->message);
        g_error_free(tmp);

	if (client.console.initialized) {
            client.console.enabled = TRUE;
	    cl_mode(&menu_callbacks, NULL);
	} else
            cl_shutdown(1);
    }
#endif
}

static void
menu_globe_demo(widget_t *w, void *data)
{
    GError *tmp = NULL;

    if (cl_mode(&globe_demo_callbacks, &tmp) != OK) {
        printerr("Client failed to load: %s\n", tmp->message);
        g_error_free(tmp);

	if (client.console.initialized) {
            client.console.enabled = TRUE;
	    cl_mode(&menu_callbacks, NULL);
	} else
            cl_shutdown(1);
    }
}


static void
menu_multiplayer_internet_connect(widget_t *w, void *data)
{
    char *val;
    real port = 0;
    real checked;

    client.game_type = GAME_INTERNET;

    /* register the hostname var */
    val = ui_widget_text_get_text(internet_hostname);
    if (val) {
	if (! sh_var_exists(client.shell, "server"))
	    sh_var_add(client.shell, "server", NULL);
	sh_var_set_str(client.shell, "server", val, NULL);
	g_free(val);
    }

    /* register the port var */
    val = ui_widget_text_get_text(internet_port);
    if (val) {
	if (! sh_var_exists(client.shell, "port"))
	    sh_var_add(client.shell, "port", NULL);
	port = g_ascii_strtod(val, NULL);
	g_free(val);
    }
    if (! (port > 0))
	port = SCS_NET_PORT;
    sh_var_set_num(client.shell, "port", port, NULL);

    /* register the record demo var */
    checked = ui_widget_checkbox_get_checked(internet_demo, NULL);
    if (checked) {
	if (! sh_var_exists(client.shell, "record"))
	    sh_var_add(client.shell, "record", NULL);
	sh_var_set_num(client.shell, "record", checked, NULL);
    }

    cl_mode(&game_callbacks, NULL);
}

static void
menu_multiplayer_create(widget_t *w, void *data)
{
    client.game_type = GAME_SERVER;
    cl_mode(&game_callbacks, NULL);
}

static void
menu_multiplayer_lan(widget_t *w, void *data)
{
    /* look for hosts broadcasting */
    client.game_type = GAME_LAN;
    print("lan game selected.\n");
}

static void
menu_film_play(widget_t *w, void *data) 
{
    char *val; 

    /* register the demo var */
    val = ui_widget_clist_get_data(film_demo);
    if (val) {
	if (! sh_var_exists(client.shell, "demo"))
	    sh_var_add(client.shell, "demo", NULL);
	sh_var_set_str(client.shell, "demo", val, NULL);
	g_free(val);
    }

    client.game_type = GAME_PLAYBACK;
    cl_mode(&game_callbacks, NULL);
}

/*
 * Navigation Callbacks
 */
static void menu_multiplayer(widget_t *w, void *data) { root = multiplayer_menu; }
static void menu_multiplayer_internet(widget_t *w, void *data) { root = internet_menu; }
static void menu_configuration(widget_t *w, void *data) { root = configuration_menu; }
static void menu_engine_demos(widget_t *w, void *data) { root = engine_demos_menu; }
static void menu_back_main(widget_t *w, void *data) { root = main_menu; }

static void
menu_film(widget_t *w, void *data) 
{ 
    /* populate the clist widget .. */
    ui_widget_clist_clear(film_demo);
    menu_insert_demos(film_demo);

    root = film_menu; 
}

void
menu_exit(widget_t *w, void *data)
{
    print("exit button clicked.\n");
    cl_shutdown(0);
}

static void
menu_library(widget_t *w, void *data)
{
    print("library button clicked.\n");
}

static gboolean
menu_key(SDL_KeyboardEvent *k, GError **err)
{
    GError *tmp = NULL;
    gboolean handled = FALSE;
    int ret;

//    print("running ui_key\n");
    if ((ret = ui_key(root, k, &handled, &tmp)) != OK) {
	g_propagate_error(err, tmp);
	return ret;
    }

    if (handled == TRUE)
	return OK;

    if (k->type == SDL_KEYUP)
	return OK;

//    print("running menu_key\n");
    switch (k->keysym.sym) {
	case SDLK_ESCAPE:
	    if (root == main_menu)
		cl_shutdown(0);
	    else if (root == configuration_menu)
		root = main_menu;
	    else if (root == internet_menu)
		root = multiplayer_menu;
	    else if (root == multiplayer_menu)
		root = main_menu;
	    else if (root == film_menu)
		root = main_menu;
	    else if (root == engine_demos_menu)
		root = main_menu;
	    break;
	default:
	    break;
    }

    return OK;
}

/* pass these right along to the UI handlers */
static gboolean
menu_mbutton(SDL_MouseButtonEvent *b, GError **err)
{
    return ui_mbutton(root, b, err);
}

static gboolean
menu_mmotion(SDL_MouseMotionEvent *m, GError **err)
{
    return ui_mmotion(root, m, err);
}

static gboolean
menu_screen_resize(int32_t w, int32_t h, GError **err)
{
    return ui_screen_resize(root, w, h, err);
}

static gboolean
menu_update(GError **err)
{
    return ui_update(root, err);
}

static gboolean
menu_draw(GError **err)
{
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
	glLoadIdentity();

	/* we want everything to appear the same size in all resolutions 
	   so we dont use our screen size here. */
	glOrtho(0.0, client.ortho[WIDTH],
		0.0, client.ortho[HEIGHT],
		-1.0, 100.0);

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	    glLoadIdentity();

	    ui_draw(root, NULL);

	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    return OK;
}

static widget_t *
menu_load_main(void)
{
    widget_t *w, *l, *b, *menu;
    widget_posv_t pos;
    widget_sizev_t size;
//    vec3_t color1 = { 0.5, 0.0, 0.9 };
    vec3_t color2 = { 0.0, 0.03, 0.25 };
    vec3_t color3 = { 1.0, 1.0, 1.0 };
    font_t *titlefnt, *itemfnt, *verfnt;
    char *txt = "v" PACKAGE_VERSION;
//    uint16_t start = client.ortho[HEIGHT] - 75, step = 32;
    int title_font_size;
    tex_t *tex;
    int button_padding = 5;
    int button_height  = 26;

    pos[X] = 0;
    pos[Y] = 0;
    size[WIDTH]  = client.ortho[WIDTH];
    size[HEIGHT] = client.ortho[HEIGHT];

    /* initialize the root window */
    tex = tex_load("cursor.png");

    menu = ui_init(pos, size, NULL);
    ui_widget_root_set_cursor(menu, tex);

    titlefnt  = ui_font_get("menu_title");
    itemfnt   = ui_font_get("menu_item");
    verfnt    = ui_font_get("menu_version");

    if (size[WIDTH] <= 640) 
	title_font_size = 26;
    else
	title_font_size = 34;

    /* create the main window */
    w = ui_widget_window_new(menu, pos, size);
    ui_widget_window_set_text(w, "Space Combat Simulator");
    ui_widget_window_set_font(w, titlefnt, title_font_size);

    /* display the version */
    size[HEIGHT] = 9;
    size[WIDTH]  = ui_font_get_strwidth(verfnt, size[HEIGHT], strlen(txt));
    pos[X] = client.ortho[WIDTH] - size[WIDTH] - 4;
    pos[Y] = 4;
    l = ui_widget_label_new(w, txt, verfnt, color3, pos, size);

    /* create buttons */
    size[HEIGHT] = button_height;
    size[WIDTH]  = 280;
    pos[X] = 10;
    pos[Y] = -1 * ((size[HEIGHT] + button_padding) * 1) - 20;
    b = ui_widget_button_new(w, "Single Player", pos, size, menu_single_player);
    ui_widget_button_set_color(b, "background", color2);

    pos[Y] = -1 * ((size[HEIGHT] + button_padding) * 2) - 20;
    b = ui_widget_button_new(w, "Multiplayer", pos, size, menu_multiplayer);
    ui_widget_button_set_color(b, "background", color2);

    pos[Y] = -1 * ((size[HEIGHT] + button_padding) * 3) - 20;
    b = ui_widget_button_new(w, "3D Mesh Viewer", pos, size, menu_library);
    ui_widget_button_set_color(b, "background", color2);

    pos[Y] = -1 * ((size[HEIGHT] + button_padding) * 4) - 20;
    b = ui_widget_button_new(w, "Recorded Games", pos, size, menu_film);
    ui_widget_button_set_color(b, "background", color2);

    pos[Y] = -1 * ((size[HEIGHT] + button_padding) * 5) - 20;
    b = ui_widget_button_new(w, "Configuration", pos, size, menu_configuration);
    ui_widget_button_set_color(b, "background", color2);

    pos[Y] = -1 * ((size[HEIGHT] + button_padding) * 6) - 20;
    b = ui_widget_button_new(w, "Engine Demos", pos, size, menu_engine_demos);
    ui_widget_button_set_color(b, "background", color2);

    pos[Y] = -1 * ((size[HEIGHT] + button_padding) * 7) - 20;
    b = ui_widget_button_new(w, "Exit", pos, size, menu_exit);
    ui_widget_button_set_color(b, "background", color2);

    return menu;
}

static void
menu_insert_demo(widget_t *w, FILE *f, char *filename)
{
    net_vcr_hdr_t hdr;
    char **arr;
    char buf[BUFSIZ];
    struct tm *t;

    if (fread(&hdr, sizeof(hdr), 1, f) <= 0)
	return;

    t = localtime((const time_t *)&hdr.creation);
    strftime(buf, sizeof(buf), "%I:%M%P %m/%d/%Y", t);

    arr = g_new0(char *, 5);
    arr[0] = strdup(filename);
    arr[1] = strdup(hdr.desc);
    arr[2] = strdup(hdr.player);
    arr[3] = strdup(buf);
    arr[4] = NULL;

    ui_widget_clist_append(w, arr, filename);

    g_strfreev(arr);
}

static void
menu_insert_demos(widget_t *w)
{
    char name[BUFSIZ];
    char smallname[BUFSIZ];
    uint16_t len, i;
    FILE *f;

    snprintf(name, sizeof(name), "%s/demos/%s", sys_datadir(), "demo00.dem");
    strcpy(smallname, "demo00.dem");
    len = strlen(name);
    for (i = 0; i <= 99; i++) {
	smallname[4] = name[len-6] = i / 10 + '0';
	smallname[5] = name[len-5] = i % 10 + '0';
	
	/* remember: always open in binary, we run on win32 too! */
	f = fopen(name, "rb");
	if (f) {
	    menu_insert_demo(w, f, smallname);
	    fclose(f);
	}
    }
}

static widget_t *
menu_load_film(void)
{
    widget_t *w, *b, *c, *menu;
    widget_posv_t pos;
    widget_sizev_t size;
    vec3_t color2 = { 0.0, 0.03, 0.25 };
    font_t *titlefnt, *itemfnt, *inputfnt;
    uint16_t start = 20, step = 32;
    tex_t *tex;
    char *hdrs[] = {
	"Name", "Description", "Player", "Time", NULL
    };
    widget_size_t sizes[] = {
	90, 246, 90, 150
    };

    pos[X] = 0;
    pos[Y] = 0;
    size[WIDTH]  = client.ortho[WIDTH];
    size[HEIGHT] = client.ortho[HEIGHT];

    /* initialize the root window */
    tex = tex_load("cursor.png");

    menu = ui_init(pos, size, NULL);
    ui_widget_root_set_cursor(menu, tex);

    titlefnt  = ui_font_get("menu_title");
    itemfnt   = ui_font_get("menu_item");
    inputfnt = ui_font_get("input");

    /* create the main window */
//    w = ui_widget_window_new(menu, "Film Room", titlefnt, 26,
//	    pos, size);
    w = ui_widget_window_new(menu, pos, size);
    ui_widget_window_set_text(w, "Film Room");
    ui_widget_window_set_font(w, titlefnt, 26);

    /* create the columned list widget */
    size[HEIGHT] = client.ortho[HEIGHT] - 100;
    size[WIDTH]  = client.ortho[WIDTH] - 20;
    pos[X] = 10;
    pos[Y] = 60;
    c = ui_widget_clist_new(menu, hdrs, sizes, G_N_ELEMENTS(hdrs)-1, pos, size);
    film_demo = c;

    /* create buttons */
    size[HEIGHT] = step - 6;
    size[WIDTH]  = 280;
    pos[X] = 10;
    pos[Y] = start - step*0;
    b = ui_widget_button_new(w, "Back", pos, size, menu_back_main);
    ui_widget_button_set_color(b, "background", color2);

    pos[X] = client.ortho[WIDTH] - 300;
    pos[Y] = start - step*0;
    b = ui_widget_button_new(w, "Play", pos, size, menu_film_play);
    ui_widget_button_set_color(b, "background", color2);

    return menu;

}

static widget_t *
menu_load_internet(void)
{
    widget_t *w, *b, *l, *t, *c, *menu;
    widget_posv_t pos;
    widget_sizev_t size;
    vec3_t color1 = { 1, 1, 1 };
    vec3_t color2 = { 0.0, 0.03, 0.25 };
    font_t *titlefnt, *itemfnt, *inputfnt;
    uint16_t start = 20, step = 32;
    tex_t *tex;
    char *txt;
    char *hdrs[] = {
	"Hostname", "Port", "Players", "Ping", NULL
    };
    widget_size_t sizes[] = {
	246, 90, 110, 130
    };

    pos[X] = 0;
    pos[Y] = 0;
    size[WIDTH]  = client.ortho[WIDTH];
    size[HEIGHT] = client.ortho[HEIGHT];

    /* initialize the root window */
    tex = tex_load("cursor.png");

    menu = ui_init(pos, size, NULL);
    ui_widget_root_set_cursor(menu, tex);

    titlefnt  = ui_font_get("menu_title");
    itemfnt   = ui_font_get("menu_item");
    inputfnt = ui_font_get("input");

    /* create the main window */
//    w = ui_widget_window_new(menu, "Internet Game", titlefnt, 26,
//	    pos, size);
    w = ui_widget_window_new(menu, pos, size);
    ui_widget_window_set_text(w, "Internet Game");
    ui_widget_window_set_font(w, titlefnt, 26);

    /* create the columned list widget */
    size[HEIGHT] = client.ortho[HEIGHT] - 170;
    size[WIDTH]  = client.ortho[WIDTH] - 20;
    pos[X] = 10;
    pos[Y] = client.ortho[HEIGHT] - 40 - (client.ortho[HEIGHT] - 170);
    c = ui_widget_clist_new(menu, hdrs, sizes, G_N_ELEMENTS(hdrs)-1, pos, size);

    /* create hostname input stuff */
    txt = "Server Name:";
    size[HEIGHT] = 8;
    size[WIDTH]  = ui_font_get_strwidth(inputfnt, size[HEIGHT], strlen(txt));
    pos[X] = 30;
    pos[Y] = 95;
    l = ui_widget_label_new(menu, txt, inputfnt, color1, pos, size);

    pos[X] = pos[X] + size[WIDTH] + 15;
    pos[Y] -= 4;	/* XXX - the labels coords don't align right */
    size[WIDTH]  = 280;
    size[HEIGHT] = 20;
    t = ui_widget_text_new(menu, pos, size);
    internet_hostname = t;

    /* create port input stuff */
    txt = "Port:";
    size[HEIGHT] = 8;
    pos[X] = pos[X] + size[WIDTH] + 30;
    size[WIDTH]  = ui_font_get_strwidth(inputfnt, size[HEIGHT], strlen(txt));
    pos[Y] = 95;
    l = ui_widget_label_new(menu, txt, inputfnt, color1, pos, size);

    pos[X] = pos[X] + size[WIDTH] + 15;
    pos[Y] -= 4;	/* XXX - the labels coords don't align right */
    size[WIDTH]  = 100;
    size[HEIGHT] = 20;
    t = ui_widget_text_new(menu, pos, size);
    ui_widget_text_set_text(t, "31337");
    internet_port = t;


    /* create hostname input stuff */
    txt = "Record Demo:";
    size[HEIGHT] = 8;
    size[WIDTH]  = ui_font_get_strwidth(inputfnt, size[HEIGHT], strlen(txt));
    pos[X] = 30;
    pos[Y] = 70;
    l = ui_widget_label_new(menu, txt, inputfnt, color1, pos, size);

    pos[X] = pos[X] + size[WIDTH] + 15;
    pos[Y] -= 4;	/* XXX - the labels coords don't align right */
    size[WIDTH]  = 20;
    size[HEIGHT] = 20;
    t = ui_widget_checkbox_new(menu, FALSE, pos, size);
    internet_demo = t;

    /* create buttons */
    size[HEIGHT] = step - 6;
    size[WIDTH]  = 280;
    pos[X] = 10;
    pos[Y] = start - step*0;
    b = ui_widget_button_new(w, "Back", pos, size, menu_multiplayer);
    ui_widget_button_set_color(b, "background", color2);

    pos[X] = client.ortho[WIDTH] - 300;
    pos[Y] = start - step*0;
    b = ui_widget_button_new(w, "Connect", pos, size, menu_multiplayer_internet_connect);
    ui_widget_button_set_color(b, "background", color2);

    return menu;

}

static widget_t *
menu_load_multiplayer(void)
{
    widget_t *w, *b, *menu;
    widget_posv_t pos;
    widget_sizev_t size;
    vec3_t color2 = { 0.0, 0.03, 0.25 };
    font_t *titlefnt, *itemfnt, *verfnt;
    uint16_t start, step = 32;
    tex_t *tex;

    pos[X] = 0;
    pos[Y] = 0;
    size[WIDTH]  = client.ortho[WIDTH];
    size[HEIGHT] = client.ortho[HEIGHT];
    start = size[HEIGHT] - (step * 2);

    /* initialize the root window */
    tex = tex_load("cursor.png");

    menu = ui_init(pos, size, NULL);
    ui_widget_root_set_cursor(menu, tex);

    titlefnt  = ui_font_get("menu_title");
    itemfnt   = ui_font_get("menu_item");
    verfnt    = ui_font_get("menu_version");

    /* create the main window */
//    w = ui_widget_window_new(menu, "Multiplayer", titlefnt, 26,
//	    pos, size);
    w = ui_widget_window_new(menu, pos, size);
    ui_widget_window_set_text(w, "Multiplayer");
    ui_widget_window_set_font(w, titlefnt, 26);

    /* create buttons */
    size[HEIGHT] = step - 6;
    size[WIDTH]  = 280;
    pos[X] = 10;
    pos[Y] = start - step*0;
    b = ui_widget_button_new(w, "Internet Game", pos, size, menu_multiplayer_internet);
    ui_widget_button_set_color(b, "background", color2);

    pos[Y] = start - step*1;
    b = ui_widget_button_new(w, "LAN Game", pos, size, menu_multiplayer_lan);
    ui_widget_button_set_color(b, "background", color2);

    pos[Y] = start - step*2;
    b = ui_widget_button_new(w, "Create Game", pos, size, menu_multiplayer_create);
    ui_widget_button_set_color(b, "background", color2);

    pos[Y] = start - step*3;
    b = ui_widget_button_new(w, "Back", pos, size, menu_back_main);
    ui_widget_button_set_color(b, "background", color2);

    /* TODO: set background image ... */
    return menu;
}

static widget_t *
menu_load_engine_demos(void)
{
    widget_t *w, *b, *menu;
    widget_posv_t pos;
    widget_sizev_t size;
    vec3_t color2 = { 0.0, 0.03, 0.25 };
    font_t *titlefnt, *itemfnt, *verfnt;
    uint16_t start, step = 32, save_width;
    tex_t *tex;

    pos[X] = 0;
    pos[Y] = 0;
    size[WIDTH]  = client.ortho[WIDTH];
    size[HEIGHT] = client.ortho[HEIGHT];
    start = size[HEIGHT] - (step * 2);

    /* initialize the root window */
    tex = tex_load("cursor.png");

    menu = ui_init(pos, size, NULL);
    ui_widget_root_set_cursor(menu, tex);

    titlefnt  = ui_font_get("menu_title");
    itemfnt   = ui_font_get("menu_item");
    verfnt    = ui_font_get("menu_version");

    /* create the main window */
    w = ui_widget_window_new(menu, pos, size);
    ui_widget_window_set_text(w, "Engine Demos");
    ui_widget_window_set_font(w, titlefnt, 26);

    /* create buttons */
    size[HEIGHT] = step - 6;
    size[WIDTH]  = 280;
    pos[X] = 10;

    pos[Y] = start - step*0;
    save_width = size[WIDTH];
    size[WIDTH] = 600;
    b = ui_widget_button_new(w, "Newtonian Gravity Simulation", pos, size, menu_newton_demo);
    ui_widget_button_set_color(b, "background", color2);

    pos[Y] = start - step*1;
    size[WIDTH] = 600;
    b = ui_widget_button_new(w, "Globe Texture Mapping", pos, size, menu_globe_demo);
    ui_widget_button_set_color(b, "background", color2);

    pos[Y] = start - step*2;
    size[WIDTH] = save_width;
    b = ui_widget_button_new(w, "Back", pos, size, menu_back_main);
    ui_widget_button_set_color(b, "background", color2);

    return menu;
}

static widget_t *
menu_load_configuration(void)
{
    widget_t *w, *b, *menu;
    widget_posv_t pos;
    widget_sizev_t size;
    vec3_t color2 = { 0.0, 0.03, 0.25 };
    font_t *titlefnt, *itemfnt, *verfnt;
    uint16_t start, step = 32;
    tex_t *tex;

    pos[X] = 0;
    pos[Y] = 0;
    size[WIDTH]  = client.ortho[WIDTH];
    size[HEIGHT] = client.ortho[HEIGHT];
    start = size[HEIGHT] - (step * 2);

    /* initialize the root window */
    tex = tex_load("cursor.png");

    menu = ui_init(pos, size, NULL);
    ui_widget_root_set_cursor(menu, tex);

    titlefnt  = ui_font_get("menu_title");
    itemfnt   = ui_font_get("menu_item");
    verfnt    = ui_font_get("menu_version");

    /* create the main window */
//    w = ui_widget_window_new(menu, "Configuration", titlefnt, 26,
//	    pos, size);
    w = ui_widget_window_new(menu, pos, size);
    ui_widget_window_set_text(w, "Configuration");
    ui_widget_window_set_font(w, titlefnt, 26);

    /* create buttons */
    size[HEIGHT] = step - 6;
    size[WIDTH]  = 280;
    pos[X] = 10;

    pos[Y] = start - step*0;
    b = ui_widget_button_new(w, "Player", pos, size, NULL);
    ui_widget_button_set_color(b, "background", color2);

    pos[Y] = start - step*1;
    b = ui_widget_button_new(w, "Controls", pos, size, NULL);
    ui_widget_button_set_color(b, "background", color2);

    pos[Y] = start - step*2;
    b = ui_widget_button_new(w, "Video", pos, size, NULL);
    ui_widget_button_set_color(b, "background", color2);

    pos[Y] = start - step*3;
    b = ui_widget_button_new(w, "Audio", pos, size, NULL);
    ui_widget_button_set_color(b, "background", color2);

    pos[Y] = start - step*4;
    b = ui_widget_button_new(w, "Network", pos, size, NULL);
    ui_widget_button_set_color(b, "background", color2);

    pos[Y] = start - step*5;
    b = ui_widget_button_new(w, "Back", pos, size, menu_back_main);
    ui_widget_button_set_color(b, "background", color2);

    return menu;
}

static gboolean
menu_load(GError **err)
{
    if (! main_menu)
	main_menu = menu_load_main();
    if (! configuration_menu)
	configuration_menu = menu_load_configuration();
    if (! multiplayer_menu)
	multiplayer_menu = menu_load_multiplayer();
    if (! internet_menu)
	internet_menu = menu_load_internet();
    if (! film_menu)
	film_menu = menu_load_film();
    if (! engine_demos_menu)
	engine_demos_menu = menu_load_engine_demos();


    if (! root)
	root = main_menu;

    // make SDL happy
    SDL_EnableKeyRepeat(250, 30);
    SDL_WarpMouse(0, 0);

    // make OpenGL happy
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH);

    client.initialized = TRUE;

    return OK;
}

static gboolean
menu_unload(GError **err)
{
#if 0
    ui_widget_destroy(root, NULL);

    root = NULL;
#endif

    SDL_EnableKeyRepeat(0,0);

    client.initialized = FALSE;

    return OK;
}

cl_callbacks_t menu_callbacks = { 
    menu_load,
    menu_unload,
    menu_update,
    menu_draw,
    menu_key,
    menu_mbutton,
    menu_mmotion,
    NULL, /* .jbutton */
    NULL, /* .jmotion */
    NULL, /* .jhat */
    menu_screen_resize,
};
