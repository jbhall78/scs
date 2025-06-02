#ifndef _SCS_H
#define _SCS_H

#include <inttypes.h>
#include <time.h>
#include <assert.h>

#include <glib.h>
#include <gio/gio.h>
#include <gnet.h>

#include "mathlib.h"
#include "clock.h"

#include "slist.h"
#include "dlist.h"

#define OK	1
#define FAIL	0

/* errors */
typedef enum {
    SCS_ERROR_MISC = 0x0000,
    SCS_ERROR_VID  = 0x0001,
    SCS_ERROR_UTIL = 0x0002,
    SCS_ERROR_TEX  = 0x0003,
    SCS_ERROR_NET  = 0x0004,
    SCS_ERROR_SH   = 0x0005,
    SCS_ERROR_XML  = 0x0006,
    SCS_ERROR_SND  = 0x0007,
    SCS_ERROR_SYS  = 0x0008,
    SCS_ERROR_GL   = 0x0009,
} scs_error_t;

typedef enum {
    FILE_DEFAULT = 0x00,
    FILE_FONT    = 0x01,
    FILE_TEXTURE = 0x02,
    FILE_OBJ     = 0x03,
    FILE_MTL     = 0x04,
    FILE_SOUND   = 0x05,
    FILE_CONF    = 0x06,
} filetype_t;

typedef enum {
    GAME_SINGLE,
    GAME_INTERNET,
    GAME_LAN,
    GAME_SERVER,
    GAME_PLAYBACK,
} game_type_t;

typedef struct {
    GList *entities;
} scene_t;

typedef struct scs {
    clock_driver_t *clock;
} scs_t;

extern scs_t scs;

#define g_abort()	g_assert(0);


#endif
