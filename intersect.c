#include <math.h>

#include "scs.h"
#include "shared.h"
#include "game.h"

#include "intersect.h"

/******************************************************************************
 *
 * Intersection Functions
 *
 *****************************************************************************/
#define EPSILON 0.000001
#define TEST_CULL
gboolean
line_tri_intersect(vec3_t orig, vec3_t dir,
                   vec3_t vert0, vec3_t vert1, vec3_t vert2,
                   real *t, real *u, real *v)
{
    vec3_t edge1, edge2, tvec, pvec, qvec;
    real det, inv_det;

    // find vectors for two edges sharing vert0 
    vec3_sub(vert1, vert0, edge1);
    vec3_sub(vert2, vert0, edge2);

    // begin calculating determinant - also used to calculate U parameter 
    vec3_cross(dir, edge2, pvec);

    // if determinant is near zero, ray lies in plane of triangle 
    det = vec3_dot(edge1, pvec);

#ifdef TEST_CULL           // define TEST_CULL if culling is desired 
    if (det < EPSILON)
        return FALSE;

    // calculate distance from vert0 to ray origin
    vec3_sub(orig, vert0, tvec);

    // calculate U parameter and test bounds
    *u = vec3_dot(tvec, pvec);
    if (*u < 0.0 || *u > det)
        return FALSE;

    // prepare to test V parameter
    vec3_cross(tvec, edge1, qvec);

    // calculate V parameter and test bounds
    *v = vec3_dot(dir, qvec);
    if (*v < 0.0 || *u + *v > det)
        return FALSE;

    // calculate t, scale parameters, ray intersects triangle
    *t = vec3_dot(edge2, qvec);
    inv_det = 1.0 / det;
    *t *= inv_det;
    *u *= inv_det;
    *v *= inv_det;
#else                    // the non-culling branch 
    if (det > -EPSILON && det < EPSILON)
        return FALSE;
    inv_det = 1.0 / det;

    // calculate distance from vert0 to ray origin
    vec3_sub(orig, vert0, tvec);

    // calculate U parameter and test bounds
    *u = vec3_dot(tvec, pvec) * inv_det;
    if (*u < 0.0 || *u > 1.0)
        return FALSE;

    // prepare to test V parameter
    vec3_cross(tvec, edge1, qvec);

    // calculate V parameter and test bounds
    *v = vec3_dot(dir, qvec) * inv_det;
    if (*v < 0.0 || *u + *v > 1.0)
        return FALSE;

    // calculate t, ray intersects triangle
    *t = vec3_dot(edge2, qvec) * inv_det;
#endif

    return TRUE;
}
#undef EPSILON
#undef TEST_CULL

#if 0
int
line_sphere_intersect(vec3_t origin, vec3_t end,
	vec3_t center, real radius, vec3_t foo, vec3_t bar)
{
    vec3_t off, v;
    real len, dist, r;

    vec3_sub(center, origin, off);
    len = (square(off[0])+square(off[1])+square(off[2]));
    r = (vec3_dot(off, end)) / len;
    vec3_cp(end, v);
//    vec3_norm(v);
    vec3_scale(v, r)
    vec3_sub(off, v, off);
    len = (square(off[0])+square(off[1])+square(off[2]));

    dist = sqrt(len);

#if 0
    if (dist <= square(radius))
	printf("%f - %f/%f/%f - %f/%f/%f ::: %f/%f\n", r, origin[0], origin[1], origin[2],
    		end[0], end[1], end[2], dist, radius);
#endif

    return dist <= square(radius);
}
#endif

#if 1

/* JH - this works, but is messy, needs cleanup */

gboolean
line_sphere_intersect(vec3_t v1, vec3_t v2, vec3_t v3, real r, vec3_t hit1, vec3_t hit2)
{

    // x1,y1,z1  P1 coordinates (point of line)
    // x2,y2,z2  P2 coordinates (point of line)
    // x3,y3,z3, r  P3 coordinates and radius (sphere)
    // x,y,z   intersection coordinates
    //
    // This function returns a pointer array which first index indicates
    // the number of intersection point, followed by coordinate pairs.
    real x1, x2, x3;
    real y1, y2, y3;
    real z1, z2, z3;
    real a, b, c, mu, i;

    x1 = v1[0];    y1 = v1[1];    z1 = v1[2];
    x2 = v2[0];    y2 = v2[1];    z2 = v2[2];
    x3 = v3[0];    y3 = v3[1];    z3 = v3[2];

    a =  square(x2 - x1) + square(y2 - y1) + square(z2 - z1);
    b =  2* ( (x2 - x1)*(x1 - x3)
	    + (y2 - y1)*(y1 - y3)
	    + (z2 - z1)*(z1 - z3) ) ;
    c =  square(x3) + square(y3) +
	square(z3) + square(x1) +
	square(y1) + square(z1) -
	2* ( x3*x1 + y3*y1 + z3*z1 ) - square(r) ;
    i =   b * b - 4 * a * c ;
    
    if ( i > 0.0 )
    {
	// first intersection
	mu = (-b + sqrt( square(b) - 4*a*c )) / (2*a);
	hit1[0] = x1 + mu*(x2-x1);
	hit1[1] = y1 + mu*(y2-y1);
	hit1[2] = z1 + mu*(z2-z1);
	// second intersection
	mu = (-b - sqrt(square(b) - 4*a*c )) / (2*a);
	hit2[0] = x1 + mu*(x2-x1);
	hit2[1] = y1 + mu*(y2-y1);
	hit2[2] = z1 + mu*(z2-z1);
    
//	printf("%f/%f/%f - %f/%f/%f\n\n", hit1[0], hit1[1], hit1[2],
//		hit2[0], hit2[1], hit2[2]);
	return TRUE;
    }

    return FALSE;
}
#endif

#if 0
// v1/v2 = line points
// v3 = center of sphere
// r = radius of sphere
int
line_sphere_intersect(vec3_t v1, vec3_t v2, vec3_t v3, real r)
{
    real x1, x2, x3;
    real y1, y2, y3;
    real z1, z2, z3;
    real a, b, c, i;

    x1 = v1[0];    y1 = v1[1];    z1 = v1[2];
    x2 = v2[0];    y2 = v2[1];    z2 = v2[2];
    x3 = v3[0];    y3 = v3[1];    z3 = v3[2];

    a = square(x2 - x1) + square(y2 - y1) + square(z2 - z1);
    b = 2 * ((x2 - x1)*(x1 - x3) +
	     (y2 - y1)*(y1 - y3) +
	     (z2 - z1)*(z1 - z3));
    c = square(x3) + square(y3) +
	square(z3) + square(x1) +
	square(y1) + square(z1) -
	2 * (x3*x1 + y3*y1 + z3*z1) - square(r);

    i = (-b + sqrt(square(b) - 4*a*c )) / (2*a);

    if (i < 0.0) {
	print("no intersect\n");
	return FALSE;
    } else if (i == 0.0) { // DOUBLE_EQ() ?
	print("one intersect\n");
	return TRUE;
    } else if (i > 0.0) {
	print("two intersects\n");
	return TRUE;
    }

    print("wtf\n");

    return FALSE;
}
#endif

real
sphere_sphere_intersect(object_t *obj1, object_t *obj2)
{
    real dist, x;
    vec3_t pos;

    vec3_sub(obj1->pos, obj2->pos, pos);
    dist = vec3_len(pos);
    
//    printf("%f/%f/%f\n", dist, obj1->mesh->radius, obj2->mesh->radius);
    x = dist - obj1->mesh->radius - obj2->mesh->radius;

    if (x < 0)
	return fabs(x);
    else
	return 0;
}

/* gamedev.net example - only works if the radius's are the same */
#if 0
gboolean
sphere_sphere_intersect(object_t *obj1, object_t *obj2)
{
    vec3_t vel, pos;
    real r, pp, pv, vv, tmin;

    // Relative velocity
    vec3_zero(vel);

    // Relative position
    vec3_zero(pos);
    vec3_sub(obj2->pos, obj1->pos, pos);

    // Minimal distance squared
    r = obj1->mesh->radius + obj2->mesh->radius;

    // dP^2-r^2
    pp = pos[X] * pos[X] + pos[Y] * pos[Y] + pos[Z] * pos[Z] - r*r;
    // (1) Check if the spheres are already intersecting
    if (pp < 0)
	return TRUE;
    // dP*dV
    pv = pos[X] * vel[X] + pos[Y] * vel[Y] + pos[Z] * vel[Z];
    // (2) Check if the spheres are moving away from each other
    if (pv >= 0)
	return FALSE;

    // dV^2
    vv = vel[X] * vel[X] + vel[Y] * vel[Y] + vel[Z] * vel[Z];
    // (3) Check if the spheres can intersect within 1 frame
    if ((pv + vv) <= 0 && (vv + 2 * pv + pp) >= 0)
	return TRUE;

    // tmin = -dP*dV/dV*2 
    // the time when the distance between the spheres is minimal
    tmin = -pv/vv;

    // Discriminant/(4*dV^2) = -(dp^2-r^2+dP*dV*tmin)
    return (pp + pv * tmin > 0);
}
#endif
