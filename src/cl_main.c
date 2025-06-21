#include <stdlib.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <glib.h>
#include <gio/gio.h>

#include "scs.h"
#include "shared.h"
#include "client.h"
#include "snd.h"
#include "sys_gl.h"

/* global client data */
client_t client;

static gboolean
cl_resize(gboolean fullscreen, int32_t w, int32_t h, GError **err)
{
    GError *tmp = NULL;
    gboolean ret;

    if (client.console.height == client.res[HEIGHT]) {
	    client.console.height = client.res[HEIGHT];
    } else {
	    // if you change this change in cl_pkt.c too
	    client.console.height = client.res[HEIGHT] / 2;
    }

    vid_resize(fullscreen, w, h);
    ret = CALL(client.callbacks->screen_resize)(w, h, &tmp);
    if (ret != OK) {
	    g_propagate_error(err, tmp);
	    return ret;
    }
    con_resize();

    return OK;
}

gboolean
CMD_exit(sh_state_t *st, int argc, char **argv, GError **err)
{
    cl_shutdown(0);
    return OK;
}

gboolean
CMD_vid_mode(sh_state_t *st, int argc, char **argv, GError **err)
{
    char *usage = "<width>x<height>(@<bpp>) (fullscreen)";
    char buf[BUFSIZ];
    char *p;
    int i;
    int width, height, bpp, len;
    gboolean fullscreen = 0;
    GError *tmp = NULL;
    gboolean ret;

    if (argc < 2) {
    	g_set_error(err, SCS_ERROR, SCS_ERROR_SH, "usage: %s %s", argv[0], usage);
	    return FAIL;
    }

    len = strlen(argv[1]);

    // parse width
    memset(buf, 0, BUFSIZ);
    p = &buf[0];
    for (i = 0; i < len; i++) {
	    if (argv[1][i] == 'x' || argv[1][i] == '\0')
	        break;
	    *p++ = argv[1][i];
    }
    if (strlen(buf) <= 0) {
	    g_set_error(err, SCS_ERROR, SCS_ERROR_SH, "usage: %s %s", argv[0], usage);
	    return FAIL;
    }
    width = atoi(buf);
    if (! (width > 0)) {
	    g_set_error(err, SCS_ERROR, SCS_ERROR_SH, "usage: %s %s", argv[0], usage);
	    return FAIL;
    }

    // parse height
    memset(buf, 0, BUFSIZ);
    p = &buf[0];
    for (i++; i < len; i++) {
	    if (argv[1][i] == '@' || argv[1][i] == '\0')
	        break;
	    *p++ = argv[1][i];
    }
    if (strlen(buf) <= 0) {
	    g_set_error(err, SCS_ERROR, SCS_ERROR_SH, "usage: %s %s", argv[0], usage);
	    return FAIL;
    }
    height = atoi(buf);
    if (! (height > 0)) {
	    g_set_error(err, SCS_ERROR, SCS_ERROR_SH, "usage: %s %s", argv[0], usage);
	    return FAIL;
    }

    // parse bits per pixel (optional)
    memset(buf, 0, BUFSIZ);
    p = &buf[0];
    for (i++; i < len; i++) {
	    if (argv[1][i] == ' ' || argv[1][i] == '\0')
	        break;
	    *p++ = argv[1][i];
    }
    if (strlen(buf) > 0) {
	    bpp = atoi(buf);
	    if (! (bpp == 8 || bpp == 15 || bpp == 16 || bpp == 24 || bpp == 32)) {
	        g_set_error(err, SCS_ERROR, SCS_ERROR_SH, "usage: %s %s", argv[0], usage);
	        return FAIL;
	    }
    }

    if (argc == 3 && argv[2][0] == 'f') {
	    fullscreen = TRUE;
    }

//    print(">>> attempting to set video mode: %dx%d@%d %s\n", width, height, bpp, (fullscreen) ? "fullscreen" : " ");

    vid_set(fullscreen, width, height);
    if (fullscreen == TRUE)
	    vec2_set(client.fullscreen_res, width, height);

    if (client.initialized) {
	    if ((ret = cl_resize(fullscreen, width, height, &tmp)) != OK) {
	        g_propagate_error(err, tmp);
	        return ret;
	    }
    }

    return OK;
}

static gboolean
cl_events(GError **err)
{
    GError *tmp = NULL;
    SDL_Event event;
    int ret;

    // Parse global events, then send them off to their proper handlers.
    while (SDL_PollEvent(&event)) {
	    switch (event.type) {
/*	    case SDL_VIDEORESIZE:
		if ((ret = cl_resize(client.fullscreen, event.resize.w, event.resize.h, &tmp)) != OK) {
		    g_propagate_error(err, tmp);
		    return ret;
		}
		break;*/

	    case SDL_QUIT:
		    cl_shutdown(0);
		    break;

	    // Keyboard Events
	    case SDL_KEYDOWN:
		    switch (event.key.keysym.sym) {
		        case SDLK_F12:
			        vid_screenshot();
			        break;
		        case SDLK_BACKQUOTE:
			        con_toggle();
			        break;
		        case SDLK_RETURN:
                    if (event.key.keysym.mod & KMOD_ALT) {
			            vid_toggle_fullscreen();
			            break;
			        }
			        /* fallthrough */
		            default:
			        if (client.console.enabled)
			            con_key(&event.key);
			        else {
			            ret = CALL(client.callbacks->key)(&event.key, &tmp);
			            if (ret != OK) {
				            g_propagate_error(err, tmp);
				            return ret;
			            }
			        }
			        break;
		    }
		    break;
	    case SDL_KEYUP:
		    if (client.console.enabled)
		        con_key(&event.key);
		    else {
		        ret = CALL(client.callbacks->key)(&event.key, &tmp);
		        if (ret != OK) {
			        g_propagate_error(err, tmp);
			        return ret;
		        }
		    }
		    break;
	    case SDL_MOUSEMOTION:
		    if (! client.console.enabled) {
		        ret = CALL(client.callbacks->mmotion)(&event.motion, &tmp);
		        if (ret != OK) {
			        g_propagate_error(err, tmp);
			        return ret;
		        }
		    }
		    break;
	    case SDL_MOUSEBUTTONDOWN:
	    case SDL_MOUSEBUTTONUP:
		    if (! client.console.enabled) {
		        ret = CALL(client.callbacks->mbutton)(&event.button, &tmp);
		        if (ret != OK) {
			        g_propagate_error(err, tmp);
			        return ret;
		        }
		    }
		    break;
	    case SDL_MOUSEWHEEL:
		    if (client.console.enabled) {
		        ; // con_mwheel
	        } else {
		        ret = CALL(client.callbacks->mwheel)(&event.wheel, &tmp);
		        if (ret != OK) {
			        g_propagate_error(err, tmp);
			        return ret;
		        }
		    }
		    break;     
	    case SDL_TEXTINPUT:
	    	// event->text.text is a char array (UTF-8 encoded string)
		    if (client.console.enabled) {
	    	    con_text(event.text.text[0]); // UTF-8 -> ASCII
		    } else {
		        ret = CALL(client.callbacks->text)(&event.text, &tmp);
		        if (ret != OK) {
			        g_propagate_error(err, tmp);
			        return ret;
		        }
		    }
		    break;
	    }
    }

    return OK;
}

static gboolean
cl_update(gpointer data)
{
    GError *tmp = NULL;
    int ret;

    /* handle incoming net messages */
    if (client.conn) {
	    net_recv_pkts(client.net);
    }

    /* handle input events */
    cl_events(NULL);

    /* run game frame (NOTE: cl_events() can change the callbacks pointer */
    ret = CALL(client.callbacks->update)(&tmp);
    if (ret != OK) {
	    printerr("error during update: %s\n", tmp->message);
	    g_error_free(tmp);

	    // prevent further updates & repeated messages
	    client.callbacks->update = NULL;

	    return FALSE;
    }

    /* send outgoing net messages */
    if (client.conn)
	    net_send_pkts(client.net);

    return TRUE;
}

gboolean
cl_draw(gpointer data)
{
    GError *tmp = NULL;
    cl_callbacks_t *cb = client.callbacks;
    int ret;

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    if (client.initialized) {
	ret = CALL(cb->draw)(&tmp);
	if (ret != OK) {
    	printerr("error during draw: %s\n", tmp->message);
	    g_error_free(tmp);
	}
    }

    // hopefully this works correctly even after an error
    if (client.console.enabled)
	    con_draw();

    SDL_GL_SwapWindow(client.window);

    return TRUE;
}

gboolean
cl_mode(cl_callbacks_t *cb, GError **err)
{
    GError *tmp = NULL;
    gboolean ret;

    assert(cb != NULL);

    if (client.callbacks != NULL) {
        /* shut down previous mode if we can */
        if (client.callbacks->unload != NULL) {
            if (! client.callbacks->unload(&tmp))
		        goto fail;
	    }
    }

    /* initialize new mode if we have to */
    if (cb->load != NULL) {
        if (! cb->load(&tmp))
	        goto fail;
    }

    client.callbacks = cb;

    // send a screen resize event (in case one was received during previous mode)
    ret = CALL(client.callbacks->screen_resize)(client.res[WIDTH], client.res[HEIGHT], &tmp);
    if (ret != OK)
	    goto fail;

    return OK;

fail:
    g_propagate_error(err, tmp);
    return FAIL;
}

static gboolean
cl_parse_cfg(GError **err)
{
    GError *tmp = NULL;
    char *files[] = {
	    "cl_binds.cfg",
	    "cl_vars.cfg",
    };
    uint8_t i;
    FILE *f = NULL;

    for (i = 0; i < G_N_ELEMENTS(files); i++) {
	    if (! (f = opencfgfile(files[i], &tmp)))
	        goto fail;

	    if (! sh_exec_file(client.shell, f, &tmp))
	        goto fail;

	    fclose(f);
    }

    return OK;

fail:
    if (f)
	    fclose(f);
    g_propagate_error(err, tmp);

    return FAIL;
}

//static void cl_obj_destroy_val(gpointer key) { g_free(key); }

static gboolean
cl_init(GError **err)
{
    GError *tmp = NULL;

    /* set our thread id */
    client.thread = g_thread_self();

    /* initialize our command queue */
    client.queue = g_async_queue_new();

    /* initialize objects */
    client.objects = g_hash_table_new_full(g_int_hash, g_int_equal,
	    /* cl_obj_destroy_val */ NULL, NULL);

    /* create the shell & parse configs before the video is initialized */
    client.shell = sh_new();

    sh_cmd_add(client.shell, "vid_mode", CMD_vid_mode, NULL);
    sh_cmd_add(client.shell, "exit", CMD_exit, NULL);
    sh_cmd_add(client.shell, "quit", CMD_exit, NULL);

    if (! bind_init(&tmp))
	    goto fail;

    /* load client data files */
    if (! cl_parse_cfg(&tmp))
	    goto fail;

    if (! vid_init(&tmp))
	    goto fail;

    if (! tex_init(&tmp))
	    goto fail;

    if (! ui_font_init(&tmp))
	    goto fail;

    if (! con_init(&tmp))
	    goto fail;

    if (! cl_mode(&menu_callbacks, &tmp))
	    goto fail;

    /* note: everything after here we should be able to recover from */
    if (! snd_init(&tmp))
	    goto fail;

    return OK;

fail:
    g_propagate_error(err, tmp);
    return FAIL;
}

static gboolean
cl_exit(GError **err)
{
    g_async_queue_unref(client.queue);

    // vid_shutdown();
    // tex_shutdown();
    // ui_font_shutdown();

    return OK;
}

static gpointer
cl_main(gpointer data)
{
    GError *err = NULL;
    int32_t update_ticks = (1.0 / (real)CL_HZ) * 1000.0;
    int32_t render_ticks = (1.0 / (real)FPS) * 1000.0;
    GMainContext *context;
    GMainLoop *loop;
    GSource *src;

    /* initialize client stuff */
    if (! cl_init(&err)) {
	    printerr("Client failed to load: %s\n", err->message);

	g_error_free(err);

	/* if the console is up, just drop them to the console */
	if (client.console.initialized)
	    client.console.enabled = TRUE;
	else
	    cl_shutdown(1);
    }

    /* create the context for this main loop */
    context = g_main_context_new();
    assert(context != g_main_context_default());

    /* create a new main loop */
    loop = g_main_loop_new(context, FALSE);

    /* create timeout sources */
    src = g_timeout_source_new(update_ticks);
    g_source_set_priority(src, G_PRIORITY_HIGH);
    g_source_set_callback(src, cl_update, NULL, NULL);
    g_source_set_can_recurse(src, FALSE);
    g_source_attach(src, context);

    src = g_timeout_source_new(render_ticks);
    g_source_set_priority(src, G_PRIORITY_HIGH);
    g_source_set_callback(src, cl_draw, NULL, NULL);
    g_source_set_can_recurse(src, FALSE);
    g_source_attach(src, context);

    print("client thread started.\n");

    g_main_loop_run(loop);

    /* shutdown client stuff */
    if (! cl_exit(&err)) {
	    printerr("Client failed to shut down: %s\n", err->message);
	    exit(1);
    }

    return NULL;
}

int
cl_launch(char *server, gboolean fullscreen, uint16_t resx, uint16_t resy)
{
    // make sure prev_res is set for when we come out of fullscreen resolution
    vec2_set(client.prev_res, resx, resy);
    vec2_set(client.fullscreen_res, resx, resy);

    vid_set(fullscreen, resx, resy);

    cl_main(NULL);

    return 0;
}

void
cl_shutdown(int ret)
{
    snd_shutdown();

    SDL_Quit();

    /* XXX - shutdown threads */

    exit(ret);
}
