#include <stdlib.h>
#include <string.h>

#include <glib.h>

#include "scs.h"
#include "shared.h"
#include "server.h"
#include "game.h"

/* global server data */
server_t server;

object_t *cube_obj;

void
sv_recv_cmds(void)
{
    thread_msg_t *msg;

    /* receive commands in our command queue */
    while ((msg = g_async_queue_try_pop(server.queue))) {
	switch (msg->code) {
	    case MSG_SHUTDOWN:
		/* send acknowledgement to caller specified queue */
		msg->code = MSG_ACK;
		g_async_queue_push(msg->data, msg);
		g_main_loop_quit(server.loop);
		break;
	    default:
		g_free(msg);
		break;
	}
    }
}

static gboolean
sv_update(gpointer data)
{
    if (! server.initialized)
	return OK;

    /* receive commands from client thread */
    if (! server.dedicated)
	sv_recv_cmds();

    /* receive packets from the wire */
    net_recv_pkts(server.net);

    /* run world tick */
    g_hash_table_foreach_remove(server.objects, sv_obj_update, NULL);

    /* send packets to clients */
    net_send_pkts(server.net);

    return OK;
}

static void sv_obj_destroy_key(gpointer key) { g_free(key); }

gboolean
sv_init(GError **err)
{
    GError *tmp = NULL;
    uint16_t i;

    /* set our current thread id */
    server.thread = g_thread_self();

    /* initialize our command shell */
    server.shell = sh_new();

    /* initialize our command queue */
    server.queue = g_async_queue_new();

    /* initialize random number generator */
    server.rand = g_rand_new();

    /* initialize players */
    for (i = 0; i < SV_PLAYERS_MAX; i++)
	server.players[i].active = FALSE;

    /* initialize objects */
    server.objects = g_hash_table_new_full(g_int_hash, g_int_equal,
	    sv_obj_destroy_key, NULL);

    /* initialize network stack */
    if (server.game_type == GAME_SINGLE)
	server.net = net_init(&net_drv_async, scs.clock, sv_proc_pkt);
    else
	server.net = net_init(&net_drv_udp, scs.clock, sv_proc_pkt);

    /* start accepting connections */
    if (! net_bind(server.net, NULL, &tmp)) {
	g_propagate_error(err, tmp);
	return FAIL;
    }

    /* JH - this data should be shared between threads */
    mesh_init(&server.meshes, NULL);

    server.initialized = TRUE;

    return OK;
}

gboolean
sv_exit(GError **err)
{
    server.initialized = FALSE;

    g_async_queue_unref(server.queue);
    server.queue = NULL;

    sh_destroy(server.shell);

    net_unbind(server.net);

    net_shutdown(server.net);
    server.net = NULL;

    print("server thread stopped.\n");

    return OK;
}

static gpointer
sv_main(gpointer data)
{
    GError *err = NULL;
    int32_t ticks = (1.0 / (real)SV_HZ) * 1000.0;
    GMainContext *context;
    GSource *src;
    thread_msg_t *msg;

    /* initialize ourselves */
    if (! sv_init(&err)) {
	msg = g_new0(thread_msg_t, 1);
	msg->code = MSG_ERROR;
	msg->data = err;
	g_async_queue_push(data, msg);
    }

    /* create the context for this main loop */
    context = g_main_context_new();
    assert(context != g_main_context_default());

    /* create a new main loop */
    server.loop = g_main_loop_new(context, FALSE);

    /* create a timeout source */
    src = g_timeout_source_new(ticks);
    g_source_set_priority(src, G_PRIORITY_HIGH);
    g_source_set_callback(src, sv_update, NULL, NULL);
    g_source_set_can_recurse(src, FALSE);
    g_source_attach(src, context);

    if (! server.dedicated && data != NULL) {
	msg = g_new0(thread_msg_t, 1);
	msg->code = MSG_ACK;
	msg->data = NULL;
	g_async_queue_push(data, msg);
    }

#if 1
    {
	object_t *obj;
	real step = 100.0;

	obj = g_new0(object_t, 1);
	vec3_set(obj->pos, 0.0, step, 0.0);
	vec3_set(obj->rotv, -1.0, 1.0, 0.0);
	sv_spawn_obj(obj, "cube");
	obj->phys = OBJ_PHYS_NONE;
	cube_obj = obj;

# if 0
     	/* play a sound */
	{
	    snd_src_t *src = g_new0(snd_src_t, 1);
	    src->loop = TRUE;
	    src->id = g_rand_int(server.rand);
	    sv_send_snd_spawn("alert_noise.wav", src, obj, NULL);
	}
# endif

#if 1
	obj = g_new0(object_t, 1);
	vec3_set(obj->pos, 0, -step, 0);
	sv_spawn_obj(obj, "hornet");

	obj = g_new0(object_t, 1);
	vec3_set(obj->pos, -step, 10, 0);
	sv_spawn_obj(obj, "fighter");

	obj = g_new0(object_t, 1);
	vec3_set(obj->pos, step, 10, 0);
	sv_spawn_obj(obj, "bomber");

	obj = g_new0(object_t, 1);
	vec3_set(obj->pos, 0, 10, step);
	sv_spawn_obj(obj, "saucer");

	obj = g_new0(object_t, 1);
	vec3_set(obj->pos, 0, 10, -step);
	sv_spawn_obj(obj, "snowflake");
#endif
    }
#endif

    print("server thread operating at %dHZ.\n", SV_HZ);

    g_main_loop_run(server.loop);

    sv_exit(NULL);

    return NULL;
}

int
sv_launch(gboolean dedicated, game_type_t type)
{
    GError *err = NULL;
    GThread *thread;

    server.dedicated = dedicated;
    server.game_type = type;

    print("launching game server...\n");

    if (dedicated) {
	sv_main(NULL);
    } else {
	GAsyncQueue *queue = g_async_queue_new();
	thread_msg_t *msg;

	thread = g_thread_create(sv_main, queue, TRUE, &err);
	if (thread == NULL) {
	    printerr("Can't create server thread: %s\n", err->message);
	    return 1;
	}

    	// wait for server thread to start
	msg = g_async_queue_pop(queue);
	if (msg->code == MSG_ERROR) {
	    err = msg->data;
	    printerr("Can't create server thread: %s\n", err->message);
	    g_error_free(err);
	    return 1;
	} else if (msg->code == MSG_ACK) {
	    ;
	} else {
	    assert(1 != 1); /* unreachable */
	}
    }

    return 0;
}

gboolean
sv_shutdown(GError **err)
{
    GAsyncQueue *queue = g_async_queue_new();
    thread_msg_t *msg = g_new0(thread_msg_t, 1);

    msg->code = MSG_SHUTDOWN;
    msg->data = queue;

    g_async_queue_push(server.queue, msg);

    // wait for server thread to shutdown
    msg = g_async_queue_pop(queue);
    if (msg->code != MSG_ACK)
	; // XXX - raise error

    g_free(msg);

    g_async_queue_unref(queue);

#if 0 /* wtf was this for? */
    /* clear the client sendq */
    while (! g_queue_is_empty(client.sendq))
	g_queue_pop_head(client.sendq); 
#endif

    return OK;
}
