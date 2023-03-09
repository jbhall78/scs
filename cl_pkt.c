#include <string.h>

#include "mem.h"
#include "scs.h"
#include "shared.h"
#include "net.h"
#include "client.h"
#include "game.h"

void
cl_obj_spawn_update_targets(gpointer key, gpointer val, gpointer data)
{
    object_t *obj = val;
    uint32_t *id = key;

//    printf("wtf: 0x%08x\n", *id);

    if (obj->id == client.obj_id)
	return;

    if (obj->notarget) {
//	printf("object: [0x%08x] notarget (%d)\n", obj->id, obj->notarget);
	return;
    }
//    printf("object: [0x%08x] target (%d)\n", obj->id, obj->notarget);

    g_array_append_val(client.targets, obj->id);
}

void
cl_obj_spawn(pkt_t *pkt)
{
    uint16_t len;
    object_t *obj = g_new0(object_t, 1);
    char *str;

    net_pkt_unpack_uint32(pkt, 1, &obj->id);
    net_pkt_unpack_str(pkt, &len, &str);
    net_pkt_unpack_real(pkt, 3, obj->pos);
    net_pkt_unpack_real(pkt, 4, obj->orient);
    net_pkt_unpack_int32(pkt, 1, &obj->notarget);

    /* load the mesh object */
    obj->mesh = mesh_load(client.meshes, str);
    mem_free(str);

    print("spawning object: 0x%08x\n", obj->id);
    g_hash_table_insert(client.objects, &obj->id, obj);

    // rebuild targets hash (this could probably be done better)
    if (client.targets->len)
	g_array_remove_range(client.targets, 0, client.targets->len);
    g_hash_table_foreach(client.objects, cl_obj_spawn_update_targets, NULL);
}

void
cl_obj_update(pkt_t *pkt)
{
    uint32_t id;
    object_t *obj;

    net_pkt_unpack_uint32(pkt, 1, &id);

    if ((obj = g_hash_table_lookup(client.objects, &id)) == NULL) {
#if 1
	print("unknown object id: 0x%08x\n", id);
#endif
	return;
    }

    net_pkt_unpack_real(pkt, 3, obj->pos);
//    if (obj->id == client.obj_id)
//	printf("cl_obj_update[0x%08x]: %f/%f/%f\n", obj->id, obj->pos[X], obj->pos[Y], obj->pos[Z]);
    net_pkt_unpack_real(pkt, 4, obj->orient);

    g_hash_table_replace(client.objects, &obj->id, obj);	/* the address of this HAS to be &obj->id */
}

void
cl_obj_frag(pkt_t *pkt)
{
    uint32_t id;
    object_t *obj;

    net_pkt_unpack_uint32(pkt, 1, &id);

    if ((obj = g_hash_table_lookup(client.objects, &id)) == NULL) {
	print("unknown object id: 0x%08x\n", id);
	return;
    }

    /* XXX */
//    mesh_unload(obj->mesh);
    print("fragging object: 0x%08x\n", id);
    g_hash_table_remove(client.objects, &id);

    if (client.targets->len)
	g_array_remove_range(client.targets, 0, client.targets->len);
    g_hash_table_foreach(client.objects, cl_obj_spawn_update_targets, NULL);
}

void
cl_snd_spawn(pkt_t *pkt)
{
    uint32_t id, obj_id;
    char *name;
    int32_t loop; 
    uint16_t len;
    snd_t *snd;
    snd_src_t *src;
    object_t *obj;

    net_pkt_unpack_uint32(pkt, 1, &id);
    net_pkt_unpack_str(pkt, &len, &name);
    net_pkt_unpack_int32(pkt, 1, &loop);
    net_pkt_unpack_uint32(pkt, 1, &obj_id);

    obj = g_hash_table_lookup(client.objects, &obj_id);
    if (! obj) {
	print("warning: no object id for sound\n");
	return;
    }

    snd = snd_load(name);
    src = snd_src_spawn(snd, id, obj, loop);
    snd_src_update(src);

    snd_play(src);

    if (! loop) {
	snd_src_frag(src);
    } 

    snd_src_register(src);
}

void
cl_proc_pkt(conn_t *conn, pkt_t *pkt)
{
    char *str;
    uint16_t len;
    uint32_t n;

    switch (pkt->type) {
	case PKT_CONN_REGISTERED:
	    net_pkt_unpack_uint32(pkt, 1, &n);
	    client.player_id = (int8_t)n;
	    net_pkt_unpack_uint32(pkt, 1, &client.obj_id);
	    print("confirmed registration: [player_id: 0x%08x] [camera_id: 0x%08x]\n", client.player_id, client.obj_id);
	    client.mouse_drag_enabled = TRUE;
	    client.console.enabled = FALSE;

	    // if you change this change in cl_main.c too
	    client.console.height = client.res[HEIGHT] / 2;

	    if (client.targets->len)
		g_array_remove_range(client.targets, 0, client.targets->len);
	    g_hash_table_foreach(client.objects, cl_obj_spawn_update_targets, NULL);
	    break;
	case PKT_OBJ_SPAWN:
	    cl_obj_spawn(pkt);
	    break;
	case PKT_OBJ_UPDATE:
	    cl_obj_update(pkt);
	    break;
	case PKT_OBJ_FRAG:
	    cl_obj_frag(pkt);
	    break;
	case PKT_CONSOLE_MSG:
	    net_pkt_unpack_str(pkt, &len, &str);
	    con_add_text(str);
	    mem_free(str);
	    break;
	case PKT_SND_SPAWN:
	    cl_snd_spawn(pkt);
	    break;
	default:
	    print("unknown packet type: 0x%x\n", pkt->type);
	    break;
    }
}
