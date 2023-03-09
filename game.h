#ifndef GAME_H
#define GAME_H

#include "mesh.h"
#include "net.h"

typedef struct { 
    uint32_t id;
    uint8_t phys;
    uint32_t frame;
    vec3_t pos;
    quat_t orient;
    vec3_t posv, rotv;  /* velocities */
    m_mesh_t *mesh;
    gboolean notarget;
    uint32_t target_id;
} object_t;

typedef struct {
    int8_t id;
    conn_t *conn;
    object_t *obj;
    gboolean active;
    gboolean privledged;
    char *name;
    uint8_t team;
    int32_t score;
    time_t connect_time;
} player_t;

#endif
