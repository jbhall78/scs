#ifndef _TEX_H
#define _TEX_H

#include "scs.h"

typedef struct tex_s {
    char		*name;
    uint32_t		id;
    uint16_vec2_t	size;
    gboolean		alpha;
    uint8_t		bpp;
    uint32_t		format;
    void		*surf; 
} tex_t;

#endif
