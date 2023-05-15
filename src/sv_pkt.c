#include <string.h>

#include "mem.h"
#include "scs.h"
#include "shared.h"
#include "net.h"
#include "server.h"
#include "snd.h"
#include "game.h"

player_t *
sv_player_find(conn_t *conn)
{
    uint16_t i;

    for (i = 0; i < SV_PLAYERS_MAX; i++) {
	if (server.players[i].active && server.players[i].conn == conn)
	    return &server.players[i];
    }

    return NULL;
}

void
sv_send_conn_registered(player_t *player)
{
    uint32_t id = player->id;
    pkt_t *pkt = net_pkt_new(PKT_CONN_REGISTERED, TRUE);

    net_pkt_pack_uint32(pkt, 1, &id);
    net_pkt_pack_uint32(pkt, 1, &player->obj->id);

    net_send(server.net, player->conn, pkt);
}

void
sv_player_attack(conn_t *conn, player_t *player, int32_t weapon)
{
    real origins[] = { -3, 3 };
    object_t *obj;
    int8_t i = 0;
    quat_t orient;
    vec3_t pos, dir, up, right, tmp, v;

    if (weapon != 1)
	return;

    obj = player->obj;

    vec3_cp(obj->pos, pos);
    quat_to_vecs(obj->orient, dir, up, right);
    quat_cp(obj->orient, orient);
    vec3_inv(dir);


    for (i = 0; i < G_N_ELEMENTS(origins); i++) {
	obj = g_new0(object_t, 1);
	obj->phys = OBJ_PHYS_LASER;
	obj->notarget = TRUE;

	sv_spawn_obj(obj, "laser");

	quat_cp(orient, obj->orient);

	// scale our origin along the camRight axis
	vec3_cp(right, tmp);
	vec3_scale(tmp, origins[i]);

	// now subtract them to obtain the offset vector
    	vec3_cp(pos, obj->pos);
	vec3_sub(obj->pos, tmp, obj->pos);
	vec3_set(obj->posv, 0, 0, -30);

	// move them forward
	vec3_cp(dir, v);
	vec3_norm(v);
	vec3_scale(v, -obj->posv[Z]);
	vec3_add(obj->pos, v, obj->pos);

	sv_send_obj_spawn(obj, NULL);

#if 1
    	/* play a sound */
	{
	    snd_src_t *src = g_new0(snd_src_t, 1);
	    src->loop = FALSE;
	    src->id = g_rand_int(server.rand);
	    
	    sv_send_snd_spawn("short_shitty_laser.wav", src, obj, NULL);
	}
#endif
    }
}

void
sv_print(char *txt)
{
    if (! server.initialized)
	return;

    pkt_t *pkt = net_pkt_new(PKT_CONSOLE_MSG, TRUE);

    net_pkt_pack_str(pkt, strlen(txt)+1, txt);

    net_send_all(server.net, pkt);
}

static void
sv_send_obj_spawns(gpointer key, gpointer val, gpointer data)
{
    object_t *obj = val;
    conn_t *conn = data;

    assert(obj != NULL);
    assert(conn != NULL);

    sv_send_obj_spawn(obj, conn);
}

player_t *
sv_register_player(conn_t *conn, char *name)
{
    player_t *player = NULL;
    uint16_t i;

    /* find free player slot */
    for (i = 0; i < SV_PLAYERS_MAX; i++) {
	if (! server.players[i].active) {
	    player = &server.players[i];
	    break;
	}
    }

    if (! player) {
	printerr("server full");
	return NULL;
    }

    conn->sock->state = STATE_ESTABLISHED;

    player->active = TRUE;
    player->privledged = FALSE;
    player->id = i+1;
    player->conn = conn;
    player->obj = g_new0(object_t, 1);
    player->name = strdup(name);
    player->team = 0;
    player->score = 0;
    player->connect_time = time(NULL);

    print("client registered: [%d]%s\n", player->id, player->name);

    /* tell the player about all the objects we have */
    g_hash_table_foreach(server.objects, sv_send_obj_spawns, conn);

    /* insert the players object */
    sv_spawn_obj(player->obj, "hornet");

    /* inform everyone that the player joined */
    sv_send_obj_spawn(player->obj, NULL);

    /* let the player know the registration process is complete */
    sv_send_conn_registered(player);

    return player;
}

void
sv_unregister_player(conn_t *conn)
{
    player_t *player;

    player = sv_player_find(conn);

    if (! player) {
	printerr("player not registered");
	return;
    }

    /* delete the connection object */
    net_conn_delete(server.net, &conn);

    print("client disconnected: [%d]%s\n", player->id, player->name);

    server.players[player->id-1].active = FALSE; 

    if (player->obj)
	sv_frag_obj(player->obj);
    if (player->name)
	g_free(player->name);
}

void
sv_proc_pkt(conn_t *conn, pkt_t *pkt)
{
    char *name;
    uint16_t len;
    int32_t number;
    player_t *player = NULL;

    if (conn)
	player = sv_player_find(conn);

    switch (pkt->type) {
	case PKT_CONN_REGISTER:
	    if (! conn)
		break;

	    net_pkt_unpack_str(pkt, &len, &name);
	    sv_register_player(conn, name);
	    mem_free(name);
	    break;
	case PKT_PLAYER_VELOCITIES:
	    if (! player)
		break;

//	    net_pkt_unpack_int32(pkt, 1, &number);
	    {
		vec3_t posv; 
		vec3_t rotv; 

		net_pkt_unpack_real(pkt, 3, posv);
		net_pkt_unpack_real(pkt, 3, rotv);

//		printf("posv[%f/%f/%f]\n", posv[X], posv[Y], posv[Z]);

		vec3_add(player->obj->posv, posv, player->obj->posv);
		vec3_add(player->obj->rotv, rotv, player->obj->rotv);
	    }
	    break;
	case PKT_PLAYER_ATTACK:
	    if (! player)
		break;

	    net_pkt_unpack_int32(pkt, 1, &number);
	    {
		net_pkt_unpack_int32(pkt, 1, &number);
		sv_player_attack(conn, player, number);
	    }
	    break;
	case PKT_PLAYER_WARP:
	    if (! player)
		break;

	    /* currently unauthenticated, but this command requires server admin capability */
	    net_pkt_unpack_int32(pkt, 1, &number);
	    {
		net_pkt_unpack_real(pkt, 3, player->obj->pos);
//		print("player warped to: [%f,%f,%f]\n", player->obj->pos[X], player->obj->pos[Y], player->obj->pos[Z]);
	    }
	    break;
	case PKT_CONN_PING_TIMEOUT:
	case PKT_CONN_DISCONNECT:
	    sv_unregister_player(conn);
	    break;
	default:
	    print("unknown packet type: 0x%x\n", pkt->type);
	    break;
    }
}

