/* 
 * Name: cl_newton.c
 * 
 * Description: Newtonian Gravity Simulation Demo
 *
 * Copyright: (c) 2011 Jason Hall <jbhall78@gmail.com>
 *
 */
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include <math.h>

#include <glib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "scs.h"
#include "shared.h"
#include "client.h"
#include "sys_gl.h"

#include "config.h"

#include "mesh.h"
#include "sys_gl.h"

#include "array.h"
#include "timer.h"

#include "thread.h"
#include "thread_func.h"

#include "random.h"

#define MASS_TO_RADIUS(mass) MAX(sqrt(mass / M_PI), 2)

void update_particles(void);

static widget_t *root = NULL;
static widget_sizev_t ScreenSize;

array_t *particles;		// working copy
array_t *particles_prev;	// used for rendering

thread_group_t *tg;

static tex_t *tex;

typedef struct { 
    mutex_t *lock;
    bool    active;
    double  mass;
    vec2_t  pos;
    vec2_t  vel;
    double  r;
    vec4_t  col;
} particle_t;

particle_t *largest_particle;
double largest_mass = 0;

particle_t *largest_particle_prev;
double largest_mass_prev = 0;

int double_buffer = 1;

void
particle_init(particle_t *particle, double mass, double x, double y, vec4_t color)
{
    particle->lock = mutex_new(MUTEX_FAST);
    particle->active = true;
    particle->mass = mass;
    particle->pos[X] = x;
    particle->pos[Y] = y;
    particle->r = MASS_TO_RADIUS(mass);
/*
    particle->r = particle->mass / 10;
    if (particle->r < 1.0)
	particle->r = 1.0;
*/
    vec4_cp(color, particle->col);
//    printf("0x%08x\n", particle);
}

void
demo_exit(widget_t *w, void *data)
{
    cl_mode(&menu_callbacks, NULL);
}

static gboolean
demo_key(SDL_KeyboardEvent *k, GError **err)
{
    GError *tmp = NULL;
    gboolean handled = FALSE;
    int ret;

    if ((ret = ui_key(root, k, &handled, &tmp)) != OK) {
        g_propagate_error(err, tmp);
        return ret;
    }

    if (handled == TRUE)
	return OK;

    if (k->type == SDL_KEYUP)
	return OK;

    switch (k->keysym.sym) {
	case SDLK_ESCAPE:
	    demo_exit(NULL, NULL);
	    break;
	default:
	    break;
    }

    return OK;
}

static gboolean
demo_mbutton(SDL_MouseButtonEvent *b, GError **err)
{
    return ui_mbutton(root, b, err);
}

static gboolean
demo_mmotion(SDL_MouseMotionEvent *m, GError **err)
{
    return ui_mmotion(root, m, err);
}

static gboolean
demo_update(GError **err)
{
    update_particles();
    return ui_update(root, err);
}

static void
demo_draw_grid(void)
{
    vec3_t color;

    color[R] = 0.4;
    color[G] = 0.4;
    color[B] = 0.4;

    glDisable(GL_POINT_SMOOTH);
    glLineWidth(1.0);
    glPointSize(1.0);
    glColor3v(color);

#if 0 /* opengl line stipple sucks, it can't handle patterns which arent divisible by 2 */
//    uint16_t pattern = 43690; /* 1010101010101010*/
//    uint16_t pattern = 4369; /* 0001000100010001 */
//    uint16_t pattern = 257; /* 0001000100010001 */
//    uint16_t pattern = 33825; /* 0001000100010001 */
    uint16_t pattern = 13107;

    glLineStipple(0,pattern);
    glEnable(GL_LINE_STIPPLE);
    glBegin(GL_LINES);
    x = 0;
    for (y = 0; y < ScreenSize[HEIGHT]; y += 50) {
        glVertex2f(x, y);
        glVertex2f(ScreenSize[WIDTH], y);
    }
    y = 0;
    for (x = 0; x < ScreenSize[WIDTH]; x += 50) {
        glVertex2f(x, y);
        glVertex2f(x, ScreenSize[HEIGHT]);
    }
    glEnd();
    glDisable(GL_LINE_STIPPLE);
#endif

    real x, y, w, h, grid = 30, stipple = 5;
    w = ScreenSize[WIDTH];
    h = ScreenSize[HEIGHT];

    glBegin(GL_POINTS);
    for (x = 0; x < w; x += grid) {
        for (y = 0; y < h; y += stipple) {
            glVertex2f(x, y);
        }
    }
    for (y = 0; y < h; y += grid) {
        for (x = 0; x < w; x += stipple) {
            glVertex2f(x, y);
        }
    }
    glEnd();

    vec3_t axis;
    axis[X] = ScreenSize[WIDTH] / 2;
    axis[Y] = ScreenSize[HEIGHT] / 2;
    axis[Z] = 0;

//    ui_draw_axis(axis, ScreenSize[WIDTH]);
}

void
_update_particle_positions(int pos, int size)
{
    int i, j;
    particle_t  *p1, *p2;
    int max = pos+size;
    struct timespec start, end;

//    printf("set: %d -> %d\n", start, start+size);

    scs.clock->read(&start);
    for (i = pos; i < max; i++) {
	p1 = (particle_t *)array_idx(particles, i);
	if (p1->active != true)
	    continue;

	for (j = 0; j < particles->len; j++) {
	    p2 = (particle_t *)array_idx(particles, j);
	    if (p2->active != true)
		continue;

	    // don't calculate against ourself
	    if (p2 == p1)
		continue;

	    vec2_add(p1->pos, p1->vel, p1->pos);
	    if (vec2_len(p1->vel) > 100.0) {
		printf("wtf\n");
	    }
#if 0
	    clamp(p1->pos[X], 0.0, ScreenSize[WIDTH] - 1);
	    clamp(p1->pos[Y], 0.0, ScreenSize[HEIGHT] - 1);
#endif

	}
    }
    scs.clock->read(&end);
//    printf("loop time(%d,%d): %lldms\n", pos, size, sys_timer_delta_ms(&start, &end));
}

void
_update_particle_velocities(int pos, int size)
{
//    double GRAVITY = 0.0010;
    //double GRAVITY = 0.0005;
//    double GRAVITY = 0.07;
// original    double GRAVITY = 0.06; // was good when the prog was buggy
    double GRAVITY = 0.04; // good for demos
    //double GRAVITY = 6.674 * pow(10, -11);	// real gravity?
    //double GRAVITY = 0.6674;  // strange
    int i, j;
    vec2_t dir;
    particle_t  *p1, *p2;
    real len, force;
    double mass_mult = 1.0;
    int max = pos+size;
    struct timespec start, end;

//    printf("set: %d -> %d\n", start, start+size);

    scs.clock->read(&start);
    for (i = pos; i < max; i++) {
	p1 = (particle_t *)array_idx(particles, i);
	if (p1->active != true)
	    continue;


	for (j = 0; j < particles->len; j++) {
	    p2 = (particle_t *)array_idx(particles, j);

	    if (p2 == p1)
		continue;
restart:
	    if (p1->active != true)
		continue;
	    if (p2->active != true)
		continue;

	    // obtain a direction vector pointing torwards ...
	    vec2_sub(p1->pos, p2->pos, dir);

	    // take the direction vector and calculate the distance between the objects
	    len = vec2_len(dir);

	    // unitize the direction vector
	    vec2_norm(dir);

#if 1
	    if (len <= p1->r+p2->r) {
		if (mutex_try_lock(p1->lock) != 0) {
		    goto restart;
		}
		if (mutex_try_lock(p2->lock) != 0) {
		    mutex_unlock(p1->lock);
		    goto restart;
		}

		// calculate momentum
		vec2_scale(p1->vel, p1->mass);
		vec2_scale(p2->vel, p2->mass);

		vec2_add(p1->vel, p2->vel, p1->vel);
		vec2_div(p1->vel, (p1->mass + p2->mass) * mass_mult );

		if (p2->mass > p1->mass)
		    vec2_cp(p2->pos, p1->pos);

		p1->mass += p2->mass;
		p1->r = MASS_TO_RADIUS(p1->mass);

		if (p1->mass > largest_mass) {
		    largest_mass = p1->mass;
		    largest_particle = p1;
		}

		p2->active = false;

		mutex_unlock(p2->lock);
		mutex_unlock(p1->lock);
		continue;
	    }
#endif
	    // calculate gravitational force based upon the opposite objects mass and the distance between the objects
	    force = -(GRAVITY * p2->mass) / (len * len);

	    // multiply the direction vector components by the gravitational force
	    vec2_scale(dir, force);

	    // apply the gravitational force exerted upon us by the other object
	    vec2_add(p1->vel, dir, p1->vel);
//	    vec2_add(p1->pos, p1->vel, p1->pos); .. whoops accidently still had updating our position here.
	}
    }
    scs.clock->read(&end);
//    printf("loop time(%d,%d): %lldms\n", pos, size, sys_timer_delta_ms(&start, &end));
}

void
update_particles(void)
{
    int i;
    int total = particles->len;
    int threads = tg->max_simul_threads;

    int n = total / threads;
    int per_thread = n;
    int extra = total % threads;

    static int step = 0;

    switch (step) {
	case 0x0:
	    //printf("velocities -->\n");
	    for (i = 0; i < threads; i++) {
		if (i+1 == threads) {
		    update_particle_velocities(i * per_thread, n + extra);
		} else {
		    update_particle_velocities(i * per_thread, n);
		}
	    }

	    // wait for all threads to complete before continuing
	    //printf("waiting ...\n");
	    //sleep(1);
	    //thread_workers_wait(tg);
	    step++;
	    return;
	case 0x1:
	    if (tg->workers_running != 0)
		return;
	    step++;
	case 0x2:
	    //printf("positions -->\n");
	    for (i = 0; i < threads; i++) {
		if (i+1 == threads) {
		    update_particle_positions(i * per_thread, n + extra);
		} else {
		    update_particle_positions(i * per_thread, n);
		}
	    }

	    //printf("waiting ...\n");
	    //sleep(1);
	    //thread_workers_wait(tg);
	    step++;
	    return;
	case 0x3:
	    if (tg->workers_running == 0) {
		if (double_buffer) { 
		    array_cp(particles, particles_prev);
		    largest_particle_prev = largest_particle;
		    largest_mass_prev = largest_mass;
		}
//		printf("end -->\n");
		step = 0;
	    }
	    return;
	default:
	    return;
    }
/*
    scs.clock->read(&start);
    for (i = 0; i < particles->len; i++) {
	p1 = array_idx(particles, i);
	if (p1->active != true)
	    continue;

	update_particle(p1);
    }
    scs.clock->read(&end);
    printf("loop time: %dms\n", sys_timer_delta_ms(&start, &end));
*/
}

void
draw_particles(void)
{
    int i;
    int x, y;
    array_t *parts;

    if (double_buffer) 
	parts = particles_prev;
    else 
	parts = particles;
//    glEnable(GL_POINT_SMOOTH);
//    glPointSize(2.0);
//    glColor3f(0.0, 0.5, 1.0);
    for (i = 0; i < parts->len; i++) {
	particle_t *p = (particle_t *)array_idx(parts, i);
	if (p->active != true)
	    continue;

	int r = MASS_TO_RADIUS(p->mass);
	/*
	//	glPointSize(MAX(p->mass / 35,1));
	glPointSize(MAX(p->r, 1));
	glBegin(GL_POINTS);
	x = (int)p->pos[X];
	y = (int)p->pos[Y];
#if 0
clamp(x, 0, ScreenSize[WIDTH]);
clamp(y, 0, ScreenSize[HEIGHT]);
#else
clamp(x, p->pos[X] - ScreenSize[WIDTH]/2, p->pos[X] + ScreenSize[WIDTH]/2);
clamp(y, p->pos[Y] - ScreenSize[HEIGHT]/2, p->pos[Y] + ScreenSize[HEIGHT]/2);
#endif
glVertex2i(x, y);
glEnd();
	 */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	static m_mat_t *mat = NULL;

	/* object lighting parameters */
	if (mat == NULL) { 
	    mat = g_new0(m_mat_t, 1);
	    assert(mat != NULL);
	    mat->lit = TRUE;
	    mat->ambient[RED] = 0;
	    mat->ambient[GREEN] = 0;
	    mat->ambient[BLUE] = 0;
	    mat->ambient[ALPHA] = 1;
	    mat->diffuse[RED] = 1;
	    mat->diffuse[GREEN] = 1;
	    mat->diffuse[BLUE] = 1;
	    mat->diffuse[ALPHA] = 1;
	    mat->specular[RED] = 1.0;
	    mat->specular[GREEN] = 1.0;
	    mat->specular[BLUE] = 1.0;
	    mat->specular[ALPHA] = 1.0;
	    mat->emissive[RED] = 1.0;
	    mat->emissive[GREEN] = 1.0;
	    mat->emissive[BLUE] = 1.0;
	    mat->emissive[ALPHA] = 1.0;
	    mat->shininess = 16.1;
	}
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat->ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat->diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat->specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, mat->emissive);
	glMaterialf(GL_FRONT, GL_SHININESS, mat->shininess);


	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex->id);


	x = (int)p->pos[X];
	y = (int)p->pos[Y];
	clamp(x, p->pos[X] - ScreenSize[WIDTH]/2, p->pos[X] + ScreenSize[WIDTH]/2);
	clamp(y, p->pos[Y] - ScreenSize[HEIGHT]/2, p->pos[Y] + ScreenSize[HEIGHT]/2);
	glColor3f(1.0,1.0,1.0);
	glBegin(GL_QUADS);
	    glTexCoord2f(0.0, 0.0);
    	    glVertex2i(x-r, y-r);
	    glTexCoord2f(1.0, 0.0);
	    glVertex2i(x+r, y-r);
	    glTexCoord2f(1.0, 1.0);
	    glVertex2i(x+r, y+r);
	    glTexCoord2f(0.0, 1.0);
	    glVertex2i(x-r, y+r);
	glEnd();
	glDisable(GL_TEXTURE_2D);

    }

}

static gboolean
demo_draw(GError **err)
{
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glDisable(GL_DEPTH_TEST);
    lighting_disable();

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
	glLoadIdentity();

	    glOrtho(0.0, ScreenSize[WIDTH],
		    0.0, ScreenSize[HEIGHT],
		    -1.0, 100.0);

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
#if 1
	    glLoadIdentity();

	    // draw background grid
	    demo_draw_grid();

/*
	    // clear the depth buffer so the grid just appears to float
	    // in the background and not intersect with other objects
    	    glClear(GL_DEPTH_BUFFER_BIT);
	    glEnable(GL_DEPTH_TEST);
	    lighting_enable();

	    vec3_t color_long, color_lat;
	    vec3_set(color_long, 0.25, 0, 0);
	    vec3_set(color_lat,  0.25, 0, 0);
	    glPushMatrix();
	    glTranslatef(ScreenSize[X]/2, ScreenSize[Y]/2, -125);
	    ui_draw_earth(ScreenSize[X]/2, ScreenSize[Y]/2, -125, 125, 2, 20.0, 20.0, color_long, color_lat);
	    glPopMatrix();
*/
//if (largest_mass > 1000 && largest_particle != NULL) {

	    double mass;
	    particle_t *largest;

	    if (double_buffer) {
		mass = largest_mass_prev;
		largest = largest_particle_prev;
	    } else {
		mass = largest_mass;
		largest = largest_particle;
	    }

	    if (mass > 30 && largest != NULL) {
		glPushMatrix();
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		particle_t *p = largest;
		glOrtho(p->pos[X] - ScreenSize[WIDTH]/2, p->pos[X] + ScreenSize[WIDTH]/2,
			p->pos[Y] - ScreenSize[HEIGHT]/2, p->pos[Y] + ScreenSize[HEIGHT]/2,
			-1.0, 100.0);
		glMatrixMode(GL_MODELVIEW);

		draw_particles();

		//glMatrixMode(GL_PROJECTION);

		glPopMatrix();
	    } else {

		draw_particles();
	    }

	    // rinse, repeat
    	    glClear(GL_DEPTH_BUFFER_BIT);
	    glDisable(GL_DEPTH_TEST);

	    lighting_disable();

#endif 
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	// draw user interface
	glPushMatrix();
 	    glMatrixMode(GL_PROJECTION);
	    glLoadIdentity();
	        glOrtho(0.0, ScreenSize[WIDTH],
	         	0.0, ScreenSize[HEIGHT],
		        -1.0, 100.0);
	        ui_draw(root, NULL);
        glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    return OK;
}

static gboolean
demo_ui_load(GError **err)
{
    tex_t *tex;
    widget_t *w;
    widget_posv_t pos;
    widget_sizev_t size;
    int step = 32;

    vec2_zero(pos);

    // initialize user interface subsystem
    root = ui_init(pos, ScreenSize, NULL);

    // configure mouse cursor
    tex = tex_load("cursor.png");
    ui_widget_root_set_cursor(root, tex);

    // draw a button
    vec2_set(pos, step, (1 * step));
    vec2_set(size, 150, step);
    w = ui_widget_button_new(root, "Back", pos, size, demo_exit);

    // texture select
/*
    int height = 20;

    vec2_set(size, 200, height);
    vec2_set(pos, ScreenSize[WIDTH] - size[WIDTH] - height, ScreenSize[HEIGHT] - (2 * height));
    w = ui_widget_select_new(root, pos, size);

    GList *files = getdatafilelist(FILE_TEXTURE, "earth", FALSE, &tmp);
    if (files == NULL) {
	g_propagate_error(err, tmp);
	return FAIL;
    }
    GList *file;

    for (file = files; file; file = file->next) {
	old_file_t *f = file->data;
	ui_widget_select_append(w, f->name, file->data);
//	print("adding: %s\n", f->name);
    }
*/

    return OK;
}

static void
particles_free_cb(void *data)
{
    particle_t *particle = data;

    if (particle->lock != NULL)
	mutex_free(particle->lock);
}

static gboolean
demo_load(GError **err)
{
    GError *tmp = NULL;

    largest_mass = 0.0;
    largest_particle = NULL;

    SDL_EnableKeyRepeat(250, 30);
    SDL_WarpMouse(0, 0);

    // set our screen size
    vec2_cp(client.res, ScreenSize);

//    tex_test = tex_load("testmap.png");
//    tex_test = tex_load("earth/lores.png");
//    tex_test = tex_load("earth/earth.png");

    if (! demo_ui_load(&tmp)) {
	g_propagate_error(err, tmp);
	return FAIL;
    }

    lighting_init();

    client.initialized = TRUE;

    tex = tex_load("particle.png"); 

    particles = array_new(particle_t, NULL, ScreenSize[WIDTH] * ScreenSize[HEIGHT], particles_free_cb);

    printf("allocated: %d particles\n", ScreenSize[WIDTH] * ScreenSize[HEIGHT]);
#if 0
//printf("11111111\n");
//    array_alloc(particles, ScreenSize[WIDTH] * ScreenSize[HEIGHT]);
//printf("22222222\n");
    int i, j = 0;
    vec4_t color;
    vec4_set(color, 0.0, 1.0, 0.0, 1.0);
    int row = 0;
    int spc = 10;
    for (j = 0; j < ScreenSize[HEIGHT]; j += spc, row++) {
    	for (i = 0; i < ScreenSize[WIDTH]; i += spc) {
	    particle_t *p = (particle_t *)array_idx(particles, i * ScreenSize[HEIGHT] + j);
	    double x = (double)i;
	    double y = (double)j;
	    if (row % 2)
		x += (double)((double)spc/2.0);
	    particle_init(p, 1, x, y, color);
//	    if (i == 0 &&  j < 10)
//	    printf("particle: 0x%08x %f %f\n", p, p->pos[X], p->pos[Y]);
	}
    }
    particles->len = ScreenSize[WIDTH] * ScreenSize[HEIGHT];
/*
    printf("verify: %d\n", particles->len);
    for (i = 0; i < 10; i++) {
	particle_t *p = array_idx(particles, i);
	    printf("particle: 0x%08x %f %f\n", p, p->pos[X], p->pos[Y]);
    }
*/
#endif

#define NUM_PARTICLES 4096
    rnd_open(RANDOM_DEV_FAST);

    int i, x, y;
    particle_t *p;
    vec4_t color;
    vec4_set(color, 0.0, 1.0, 0.0, 1.0);

    for (i = 0; i < NUM_PARTICLES; i++) {
	do {
	    x = rnd_number(0, ScreenSize[WIDTH]-1);
	    y = rnd_number(0, ScreenSize[HEIGHT]-1);

	    p = (particle_t *)array_idx(particles, y * ScreenSize[HEIGHT] + x);
	} while (p->active == true);
	particle_init(p, 1.0, (double)x, (double)y, color);
//	printf("initializing particle: [%d]  @  %f, %f\n", i, (double)x, (double)y);
    }
    particles->len = ScreenSize[WIDTH] * ScreenSize[HEIGHT];

    if (double_buffer) { 
	//particles_prev = array_dup(particles);
	particles_prev = array_new(particle_t, NULL, ScreenSize[WIDTH] * ScreenSize[HEIGHT], NULL);
	array_cp(particles, particles_prev);
    }

    printf("particles initialized\n");


    // since we are double buffered and want to put something on the screen immediately, do this:
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    SDL_GL_SwapWindow(client.window);
    if (! demo_draw(&tmp)) {
	g_propagate_error(err, tmp);
	return FAIL;
    }
    SDL_GL_SwapWindow(client.window);

    tg = thread_group_new();
    thread_group_spawn_workers(tg);

/*
    // draw a single frame 
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    demo_draw
    SDL_GL_SwapBuffers();
*/


    return OK;
}

static gboolean
demo_unload(GError **err)
{
    thread_group_stop_workers(tg);

    SDL_EnableKeyRepeat(0,0);

    array_free(particles);
//    array_free(particles_prev);

    client.initialized = FALSE;

    return OK;
}

cl_callbacks_t newton_demo_callbacks = { 
    demo_load,
    demo_unload,
    demo_update,
    demo_draw,
    demo_key,
    demo_mbutton,
    demo_mmotion,
    NULL, /* .jbutton */
    NULL, /* .jmotion */
    NULL, /* .jhat */
};
