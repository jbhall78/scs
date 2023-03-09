#include <stdlib.h>

#include <math.h>

#include <glib.h>
#include <SDL.h>
#include <SDL_opengl.h>


#include "scs.h"
#include "shared.h"
#include "client.h"
#include "sys_gl.h"

void
lighting_init(void)
{
    light_t *light;
    vec3_t pos = { -0.5, 0, 0.5 };

    client.light_group = light_group_new();

    light = light_new();
    light_set_type(light, LIGHT_LOCAL);
    light_set_position_type(light, LIGHT_POS_OBJECT);
    light_set_position(light, pos);

/* old lighting options
    float lightAmbient[]  = { 0.0, 0.0, 0.0, 1.0 };
    float lightDiffuse[]  = { 0.9, 0.9, 0.85, 1.0 };
    float lightSpecular[] = { 0.9, 0.9, 1.0, 1.0 };
*/


    light_group_add_light(client.light_group, light);
}

void
lighting_disable(void)
{
    int i;

    glDisable(GL_LIGHTING);

    // disable any existing lights
    for (i = 0; i < 8; i++) {
	glDisable(GL_LIGHT0 + i);
    }
}

void
lighting_enable(void)
{
    light_group_configure(client.light_group);
}

/*
 * A light group is a group of lights which affect world objects
 * there is usually only 1 in a scene unless something needs to
 * be specially lit.
 */
light_group_t *
light_group_new(void)
{
    light_group_t *group;
    group = malloc(sizeof(light_group_t));
    assert(group != NULL);
    memset(group, 0, sizeof(light_group_t));
    return group;
}

/* add a light to the light group */
void
light_group_add_light(light_group_t *group, light_t *light)
{
    slist_t *item;

    // set the group for future reference
    light->group = group;

    // set the opengl light id
    light->id = GL_LIGHT0 + group->n_lights;

    // create item
    item = slist_new();
    item->data = light;

    // add item to group
    group->lights = slist_add_start(group->lights, item);
    group->n_lights++;
}

/* 
 * Configure lighting options
 */
void
light_group_configure(light_group_t *group)
{
    slist_t *item;
    light_t *light;
    int i;


    // disable any existing lights
    for (i = 0; i < 8; i++) {
	glDisable(GL_LIGHT0 + i);
    }

    /* controls direction of specular reflections, 
     * this probably needs to be computed more
     * intelligently.
     */
    {
	float angle = 0.0;
	glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, &angle);
    }
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
    glColor3f(1,1,1);
    glDisable(GL_NORMALIZE);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST); 
    glShadeModel(GL_SMOOTH);
    //glDisable(GL_CULL_FACE);
    glDisable(GL_COLOR_MATERIAL);


    for (item = group->lights; item; item = item->next) {
	light = item->data;
	light_configure(light);
    }
}

light_t *
light_new(void)
{
    vec3_t zero = { 0,0,0 };
    vec4_t ones = { 1,1,1,1 };
    light_t *light;
    light = malloc(sizeof(light_t));
    assert(light != NULL);
    memset(light, 0, sizeof(light_t));

    light_set_type(light, LIGHT_AMBIENT);
    light_set_position_type(light, LIGHT_POS_WORLD);
    light_set_position(light, zero);
    light_set_power(light, 0.0);
    light_set_angle(light, 0.0);
    light_set_color(light, ones);

    return light;
}

void
light_set_type(light_t *light, light_type_t light_type)
{
    assert(light != NULL);

    light->type = light_type;
}

void
light_set_position_type(light_t *light, light_position_type_t pos_type)
{
    assert(light != NULL);

    light->pos_type = pos_type;

    if (light->pos_type == LIGHT_POS_WORLD) {
	light->pos[3] = 1.0;
    } else if (light->pos_type == LIGHT_POS_OBJECT) {
	light->pos[3] = 0.0;
    }
}

void
light_set_position(light_t *light, vec3_t v)
{
    assert(light != NULL);

    vec3_cp(v, light->pos);

    if (light->pos_type == LIGHT_POS_WORLD) {
	light->pos[3] = 1.0;
    } else if (light->pos_type == LIGHT_POS_OBJECT) {
	light->pos[3] = 0.0;
    }
}

void
light_set_angle(light_t *light, real angle)
{
    assert(light != NULL);

    light->angle = angle;
}

void
light_set_power(light_t *light, real power)
{
    assert(light != NULL);

    light->power = power;
}

void
light_set_color(light_t *light, vec4_t color)
{
    vec4_cp(color, light->color);
}

/* configure the light w/OpenGL */
void
light_configure(light_t *light)
{
    GLfloat pos[4];

    assert(light != NULL);

    glEnable(light->id);

    /* scene lighting parameters */
    GLfloat specular[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat ambient[]  = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat diffuse[]  = { 1.0, 1.0, 1.0, 1.0 };

    pos[X] = (GLfloat)light->pos[X];
    pos[Y] = (GLfloat)light->pos[Y];
    pos[Z] = (GLfloat)light->pos[Z];
    pos[W] = (GLfloat)light->pos[W];

    glLightfv(light->id, GL_SPECULAR, specular);
    glLightfv(light->id, GL_AMBIENT,  ambient);
    glLightfv(light->id, GL_DIFFUSE,  diffuse);
    glLightfv(light->id, GL_POSITION, pos);

    /* todo: 
    glLightfv(light->id, GL_SPOT_EXPONENT, light->power);
    glLightfv(light->id, GL_SPOT_DIRECTION, light->);
    */
}
