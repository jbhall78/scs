#ifndef _INTERSECT_H
#define _INTERSECT_H

#include <glib.h>

#include "scs.h"
#include "net.h"

real sphere_sphere_intersect(object_t *obj1, object_t *obj2);
gboolean line_sphere_intersect(vec3_t v1, vec3_t v2, vec3_t v3, real r, vec3_t hit1, vec3_t hit2);

#endif
