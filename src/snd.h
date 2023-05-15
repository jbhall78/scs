#ifndef _SND_H
#define _SND_H

#include "mathlib.h"
#include "game.h"

typedef struct {
    vec3_t pos;
    quat_t orient;
    vec3_t posv;
} snd_listener_t;

typedef struct {
    char *name;
    uint32_t buf_id;
} snd_t;

typedef struct {
    uint32_t id;
    uint32_t src_id;
    gboolean loop;
    snd_t *snd;
    object_t *obj;

    gboolean fragged;
} snd_src_t;

gboolean snd_init(GError **err);
void snd_update(snd_listener_t *listener);
snd_t *snd_load(char *name);
void snd_unload(snd_t *snd);
void snd_src_update(snd_src_t *src);
void snd_update_srcs(void);
void snd_play(snd_src_t *src);
snd_src_t *snd_src_spawn(snd_t *snd, int32_t id, object_t *obj, gboolean loop);
void snd_src_reap(void);
void snd_src_frag(snd_src_t *src);
void snd_src_register(snd_src_t *src);

void snd_shutdown(void);

#endif
