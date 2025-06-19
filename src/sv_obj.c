#include <string.h>
#include <math.h>

#include <glib.h>

#include "scs.h"
#include "shared.h"
#include "net.h"
#include "server.h"
#include "snd.h"
#include "game.h"

#include "timer.h"
#include "intersect.h"

void
sv_send_obj_frag(object_t *obj)
{
    pkt_t *pkt = net_pkt_new(PKT_OBJ_FRAG, TRUE);

    net_pkt_pack_uint32(pkt, 1, &obj->id);

    net_send_all(server.net, pkt);
}

void
sv_send_obj_spawn(object_t *obj, conn_t *conn)
{
    pkt_t *pkt = net_pkt_new(PKT_OBJ_SPAWN, TRUE);

    net_pkt_pack_uint32(pkt, 1, &obj->id);
    net_pkt_pack_str(pkt, strlen(obj->mesh->name)+1, obj->mesh->name);
    net_pkt_pack_real(pkt, 3, obj->pos);
    net_pkt_pack_real(pkt, 4, obj->orient);
//    printf("notarget: %d\n", obj->notarget);
    net_pkt_pack_int32(pkt, 1, &obj->notarget);

    if (conn)
	net_send(server.net, conn, pkt);
    else
	net_send_all(server.net, pkt);
}

void
sv_send_snd_spawn(char *name, snd_src_t *src, object_t *obj, conn_t *conn)
{
    pkt_t *pkt = net_pkt_new(PKT_SND_SPAWN, TRUE);

    net_pkt_pack_uint32(pkt, 1, &src->id);
    net_pkt_pack_str(pkt, strlen(name)+1, name);
    net_pkt_pack_int32(pkt, 1, &src->loop);
    net_pkt_pack_uint32(pkt, 1, &obj->id);

    if (conn)
	net_send(server.net, conn, pkt);
    else
	net_send_all(server.net, pkt);
}

void
sv_obj_update_col_laser(object_t *obj1, object_t *obj2)
{
    vec3_t up, right, fwd, l1, l2, v, h1, h2;
    vec4_t vnear, vfar;

    if (obj1->phys == OBJ_PHYS_LASER && obj2->phys == OBJ_PHYS_LASER)
	return;

    /* make sure only 1 object is a laser, so we don't do this test twice */
    if (obj2->phys != OBJ_PHYS_LASER)
	return;

#if 0
    if (strcmp(obj1->mesh->name, "hornet") == 0)
	return;
#endif

    /* object 2 is the laser */

    // NOTE: this needs to be updated, if the laser moves too fast it will pass
    //       completely through the sphere avoiding any collision
    //
    //       this also goes for the object we are trying to hit, if it moves too
    //       fast the collision test wont work either.
    //
    //       there are two ways to solve this, perform collision detection on each
    //       "frame" in between each physics frame, which would perhaps be easier
    //       from the coding perspective, we wouldn't have to create complicated
    //       "trailed" objects for collision testing, we would instead perform the 
    //       simpler calculations more times, which could end up being more cpu
    //       intensive. as the objects become more complicated and when you want the
    //       collisions to be more accurate, this looks like the correct solution
    //
    //       imagine trying to create something similar to the following for a 
    //       humanoid object:
    //
    //       the line sphere detection would end up looking like a longer line from
    //       the starting point of the starting position, to the ending point of 
    //       the ending position of the line,
    //       the sphere would look like a half sphere, a cylindrical tube and another
    //       half sphere

    // build the first point of the line
    quat_to_vecs(obj2->orient, fwd, up, right);
    vec3_cp(obj2->pos, l1);
    vec3_cp(fwd, v);
    vec3_norm(v);
    vec3_scale(v, obj2->mesh->radius);
    vec3_add(l1, v, l1);

    // build the second point of the line
    vec3_cp(obj2->pos, l2);
    vec3_cp(fwd, v);
    vec3_norm(v);
    vec3_scale(v, -obj2->mesh->radius);
    vec3_add(l2, v, l2);

    //return;
    if (! line_sphere_intersect(l1, l2, obj1->pos, obj1->mesh->radius, h1, h2))
	return;

#if 1
    // figure out which hit is closer
    { 
	vec3_t v1, v2, tmp;
	
	vec3_sub(h1, obj1->pos, v1);
	vec3_sub(h2, obj1->pos, v2);

	if (vec3_len(v2) > vec3_len(v1)) {
	    vec3_cp(h2, tmp);
	    vec3_cp(h1, h2);
	    vec3_cp(tmp, h1);
	}
    }

    // build nearplane & convert our plane into normal/constant form
    {
	vec3_t edge1, edge2;
	vec3_t tmp[3];
	
	vec3_cp(h1, tmp[0]);
	vec3_add(h1, up, tmp[1]);
	vec3_add(h1, right, tmp[2]);
	
	vec3_sub(tmp[1], tmp[0], edge1);
	vec3_sub(tmp[2], tmp[0], edge2);
	vec3_cross(edge1, edge2, vnear);
	vec3_norm(vnear);
	vnear[W] = vec3_dot(vnear, tmp[0]);
    }
    // build farplane & convert our plane into normal/constant form
    {
	vec3_t edge1, edge2;
	vec3_t tmp[3];
	
	vec3_cp(h2, tmp[0]);
	vec3_add(h2, up, tmp[1]);
	vec3_add(h2, right, tmp[2]);
	
	vec3_sub(tmp[1], tmp[0], edge1);
	vec3_sub(tmp[2], tmp[0], edge2);
	vec3_cross(edge1, edge2, vfar);
	vec3_norm(vfar);
	vfar[W] = vec3_dot(vfar, tmp[0]);
    }



    {
	real dist1, dist2, dist3, dist4;
	
	dist1 = vec3_dot(vnear, l1) - vnear[W];
	dist2 = vec3_dot(vnear, l2) - vnear[W];

	dist3 = vec3_dot(vfar,  l1) - vfar[W];
	dist4 = vec3_dot(vfar,  l2) - vfar[W];

	//print("[%f/%f/%f/%f]\n", dist1, dist2, dist3, dist4);
	if ((dist1 < 0 || dist3 < 0) && dist2 > 0 && dist4 > 0)
	    print("collision: %s\n", obj1->mesh->name);
	else
	    return;
    }
#endif

#if 1
    /* play a sound */
    {
	snd_src_t *src = g_new0(snd_src_t, 1);
	src->loop = FALSE;
	src->id = g_rand_int(server.rand);

	sv_send_snd_spawn("general_thud.wav", src, obj1, NULL);
    }
#endif

}

void
sv_obj_update_col(gpointer key, gpointer val, gpointer data)
{
    real ret, accel1, accel2;
    object_t *obj1 = data;
    object_t *obj2 = val;
    vec3_t v;

    if (obj1->id == obj2->id)
	return;

    if (obj1->phys == OBJ_PHYS_LASER || obj2->phys == OBJ_PHYS_LASER) {
	sv_obj_update_col_laser(obj1, obj2);
	return;
    }
	
    if (! (ret = sphere_sphere_intersect(obj1, obj2)))
	return;

    accel1 = vec3_len(obj1->posv);
    accel2 = vec3_len(obj2->posv);

    if (accel1 > accel2) 
	return;

#if 1
    /* play a sound */
    {
	snd_src_t *src = g_new0(snd_src_t, 1);
	src->loop = FALSE;
	src->id = g_rand_int(server.rand);

	sv_send_snd_spawn("bang_gunishsound.wav", src, obj1, NULL);
    }
#endif

//    print("(%f) BEFORE: %s %f/%f/%f <> %s %f/%f/%f\n", ret, obj1->mesh->name, obj1->posv[0], obj1->posv[1], obj1->posv[2], obj2->mesh->name, obj2->posv[0], obj2->posv[1], obj2->posv[2]);

    vec3_sub(obj1->pos, obj2->pos, v);

    vec3_norm(v);
    vec3_scale(v, (accel2 - accel1) * 2);
    vec3_add(obj1->posv, v, obj1->posv); 

//    print("AFTER: %s %f/%f/%f <> %s %f/%f/%f\n", obj1->mesh->name, obj1->posv[0], obj1->posv[1], obj1->posv[2], obj2->mesh->name, obj2->posv[0], obj2->posv[1], obj2->posv[2]);

    return;
}

gboolean
sv_obj_update_pos_orient(object_t *obj)
{
    vec3_t dir[3];
    quat_t q;
    real maxpos = 3.0;
    real maxrot = 4.0;
    real accel = 0.01;
    real accelrotz = 0.30;
    real accelrotxy = 0.80;
    real mod, off;
    uint8_t i;

    quat_to_vecs(obj->orient, dir[2], dir[1], dir[0]);
    if (obj->phys == OBJ_PHYS_LASER) {
	vec3_t v;
	vec3_cp(dir[Z], v);
	vec3_norm(v);
	vec3_scale(v, obj->posv[Z]);
	vec3_add(obj->pos, v, obj->pos);

	if (++obj->frame == 100)
	    return TRUE;
    } else {
	for (i = 0; i < 3; i++) {
	    if (REAL_EQ(obj->posv[i], 0))
		continue;

	    mod = obj->posv[i] / fabs(obj->posv[i]);
	    
	    if (fabs(obj->posv[i]) >= maxpos)
		obj->posv[i] = maxpos * mod;
	    
	    vec3_scale(dir[i], obj->posv[i]);
	    vec3_add(dir[i], obj->pos, obj->pos);
	    
	    if (obj->phys != OBJ_PHYS_NONE) {
		off = fabs(obj->posv[i]) - accel;
		if (off < 0)
		    off = 0;
		obj->posv[i] = off * mod;
	    }
	}
    }

extern object_t *cube_obj;
if (obj == cube_obj) {
    static sys_timer_t *timer = NULL;

    if (timer == NULL) {
	timer = sys_timer_new(scs.clock);
	sys_timer_set_interval_ms(timer, 1000);
	sys_timer_set_repeating(timer, TRUE);
	sys_timer_reset(timer);
    }
    if (sys_timer_is_ready(timer)) {
	quat_set3(q, 45.0, 0, 0);
	quat_mult(obj->orient, q);

	quat_norm(obj->orient);
    }

    return FALSE;
}


    if (! REAL_EQ(obj->rotv[X], 0)) {
    	mod = obj->rotv[X] / fabs(obj->rotv[X]);
	
	if (fabs(obj->rotv[X]) >= maxrot)
	    obj->rotv[X] = maxrot * mod;
	
	quat_set3(q, obj->rotv[X], 0, 0);
	quat_mult(obj->orient, q);

#if 1
	if (obj->phys != OBJ_PHYS_NONE) {
	    off = fabs(obj->rotv[X]) - accelrotxy;
	    if (off < 0)
		off = 0;
	    obj->rotv[X] = off * mod;
	}
#else
	obj->rotv[X] = 0;
#endif
    }
    if (! REAL_EQ(obj->rotv[Y], 0)) {
    	mod = obj->rotv[Y] / fabs(obj->rotv[Y]);
	
	if (fabs(obj->rotv[Y]) >= maxrot)
	    obj->rotv[Y] = maxrot * mod;
	
	quat_set3(q, 0, obj->rotv[Y], 0);
	quat_mult(obj->orient, q);

#if 1
	if (obj->phys != OBJ_PHYS_NONE) {
	    off = fabs(obj->rotv[Y]) - accelrotxy;
	    if (off < 0)
		off = 0;
	    obj->rotv[Y] = off * mod;
	}
#else
	obj->rotv[Y] = 0;
#endif
    }
    if (! REAL_EQ(obj->rotv[Z], 0)) {
    	mod = obj->rotv[Z] / fabs(obj->rotv[Z]);
	
	if (fabs(obj->rotv[Z]) >= maxrot)
	    obj->rotv[Z] = maxrot * mod;
	
	quat_set3(q, 0, 0, obj->rotv[Z]);
	quat_mult(obj->orient, q);

	if (obj->phys != OBJ_PHYS_NONE) {
	    off = fabs(obj->rotv[Z]) - accelrotz;
	    if (off < 0)
		off = 0;
	    obj->rotv[Z] = off * mod;
	}
    }

    quat_norm(obj->orient);

    return FALSE;
}

gboolean
sv_obj_update(gpointer key, gpointer val, gpointer data)
{
    uint32_t *id = key;
    object_t *obj = val;
    pkt_t *pkt;
    gboolean reap;

    g_hash_table_foreach(server.objects, sv_obj_update_col, obj);

    reap = sv_obj_update_pos_orient(obj);

    if (reap) {
	sv_send_obj_frag(obj);

	/* XXX - */
	g_free(obj);

	return TRUE;
    }

    pkt = net_pkt_new(PKT_OBJ_UPDATE, FALSE);

    net_pkt_pack_uint32(pkt, 1, id);
//	printf("sv_obj_update: 0x%08x %f/%f/%f\n", obj->id, obj->pos[X], obj->pos[Y], obj->pos[Z]);
    net_pkt_pack_real(pkt, 3, obj->pos);
    net_pkt_pack_real(pkt, 4, obj->orient);

    net_send_all(server.net, pkt);

    return FALSE;
}

void
sv_spawn_obj(object_t *obj, char *model)
{
    uint32_t *id = g_new0(uint32_t, 1);

    do {
	*id = g_rand_int(server.rand);
    } while (g_hash_table_lookup(server.objects, id));

    obj->id = *id;
    obj->mesh = mesh_load(server.meshes, model);
    quat_id(obj->orient);

//    vec3_set(obj->pos, 0.0, 1.8, 20.0);

    g_hash_table_insert(server.objects, id, obj);

}

void
sv_frag_obj(object_t *obj)
{
    assert(obj != NULL);

    sv_send_obj_frag(obj);

#if 0
    if (obj->mesh)
	g_free(obj->model);
#endif

    g_hash_table_remove(server.objects, &obj->id);
}
