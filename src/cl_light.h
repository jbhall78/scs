#ifndef _CL_LIGHT_H
#define _CL_LIGHT_H

typedef enum {
    LIGHT_SUN,          // star light, directional
    LIGHT_LOCAL,        // local light source (eg. flashlight), directional, angular
    LIGHT_AMBIENT,      // ambient light (to prevent items from getting too dark)
} light_type_t;

typedef enum {
    LIGHT_POS_WORLD,
    LIGHT_POS_OBJECT            // NOTE: affects all objects using this light group similarly
} light_position_type_t;

typedef struct light_s light_t;
typedef struct light_group_s light_group_t;

struct light_s {
    light_group_t *group;
    light_type_t type;
    light_position_type_t pos_type;
    vec4_t pos;
    real angle;
    real power;
    vec4_t color;
    int id;     // opengl light id
};

struct light_group_s {
    int n_lights;
    slist_t     *lights;        // members of light group
};

void lighting_init(void);
void lighting_disable(void);
void lighting_enable(void);
light_group_t *light_group_new(void);
void light_group_add_light(light_group_t *group, light_t *light);
void light_group_configure(light_group_t *group);
light_t *light_new(void);
void light_set_type(light_t *light, light_type_t light_type);
void light_set_position_type(light_t *light, light_position_type_t pos_type);
void light_set_position(light_t *light, vec3_t v);
void light_set_angle(light_t *light, real angle);
void light_set_power(light_t *light, real power);
void light_set_color(light_t *light, vec4_t color);
void light_configure(light_t *light);

#endif
