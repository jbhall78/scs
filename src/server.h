#ifndef SVR_H
#define SVR_H

#include <time.h>

#include <glib.h>
#include <gio/gio.h>

#include "scs.h"
#include "shell.h"
#include "net.h"
#include "snd.h"

/* run game server at 20 fps */
#define SV_HZ	120	

/* limit game to 64 connections */
#define SV_PLAYERS_MAX  64

/* _ALL_ SERVER SIDE GLOBALS GO IN THIS STRUCT */
typedef struct server_s {
    gboolean initialized;
    gboolean dedicated;
    game_type_t game_type;

    /* thread stuff */
    GThread *thread;
    GMainLoop *loop;
    GAsyncQueue *queue;

    /* network stuff */
    net_state_t *net;
//    sock_t *sock;

    /* world stuff */
    GHashTable *objects;
    player_t players[SV_PLAYERS_MAX];

    /* data */
    GCache *meshes;

    /* misc stuff */
    GRand *rand;
    sh_state_t *shell;
} server_t;

extern server_t server;

gpointer sv_io_main(gpointer data);
void sv_print(char *txt);

/* object related functions */
gboolean sv_obj_update(gpointer key, gpointer val, gpointer data);
void sv_spawn_obj(object_t *obj, char *model);
void sv_send_obj_spawn(object_t *obj, conn_t *conn);
void sv_frag_obj(object_t *obj);

void sv_proc_pkt(conn_t *conn, pkt_t *pkt);
void sv_send_snd_spawn(char *name, snd_src_t *src, object_t *obj, conn_t *conn);

#endif
