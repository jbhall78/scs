#include <string.h>

#include <math.h>

#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "scs.h"
#include "shared.h"
#include "client.h"
#include "mesh.h"
#include "sys_gl.h"
#include "game.h"

/* XXX */
static int32_t starfield;

#define SND_COMPUTER_NEXT "computer_next.wav"
#define SND_COMPUTER_PREV "computer_prev.wav"

static snd_t *snd_computer_next;
static snd_t *snd_computer_prev;

static void
game_build_starfield(int nstars, double spread)
{
    int i, j;
    double pointsizes[] = { 1.0, 2.0, 3.0 };
    int nsizes = 3;

    glEnable(GL_BLEND);		// for alpha on billboards

    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST); 
    glColor3f(1.0, 1.0, 1.0);
    for (j = 0; j < nsizes; j++) {
        glPointSize(pointsizes[j]);

        glBegin(GL_POINTS);
        for (i = 0; i < (nstars/3); i++) { // close enough =)
            double x, y, z;

            x = -.5 * spread + rand() * 1.2 * ((double)spread/RAND_MAX);
            y = -.5 * spread + rand() * 1.2 * ((double)spread/RAND_MAX);
            z = -.5 * spread + rand() * 1.2 * ((double)spread/RAND_MAX);


            glVertex3f(x, y, z);
        }
        glEnd();
    }
    glDisable(GL_BLEND);	// for alpha on billboards
}

void
game_send_player_velocities(conn_t *conn, vec3_t pos, vec3_t rot)
{
    int32_t id = client.player_id;

    pkt_t *pkt = net_pkt_new(PKT_PLAYER_VELOCITIES, FALSE);
//    net_pkt_pack_int32(pkt, 1, &id);
    net_pkt_pack_real(pkt, 3, pos);
    net_pkt_pack_real(pkt, 3, rot);

//    if (pos[X] > 0 || pos[Y] > 0 || pos[Z] > 0) {
//	printf("cl_posv[%f/%f/%f]\n", pos[X], pos[Y], pos[Z]);
//    }

    net_send(client.net, client.conn, pkt); 
}

gboolean
CMD_rotate(sh_state_t *st, int argc, char **argv, GError **err)
{
    vec3_t angles;
    int8_t mod = 0;
    static real step = 3.00;

    if (argc != 2) {
	    g_set_error(err, SCS_ERROR, SCS_ERROR_SH,
		    "usage: %s <left|right>", argv[0]);
	    return FAIL;
    }

    if (strcmp(argv[1], "left") == 0)
	    mod = -1;
    else if (strcmp(argv[1], "right") == 0)
	    mod = 1;
    else
	    g_abort();

    vec3_set(angles, 0, 0, mod * step);

    client.rotv[Z] = angles[Z];

    return OK;
}

gboolean
CMD_move(sh_state_t *st, int argc, char **argv, GError **err)
{
    static real step = 0.20;

    if (argc != 2) {
	    g_set_error(err, SCS_ERROR, SCS_ERROR_SH,
		    "usage: %s <fwd|back|left|right|up|down>", argv[0]);
	    return FAIL;
    }

    if (strcmp(argv[1], "fwd") == 0)
	    client.posv[Z] = -step;
    else if (strcmp(argv[1], "back") == 0)
	    client.posv[Z] = step;
    else if (strcmp(argv[1], "left") == 0)
	    client.posv[X] = -step;
    else if (strcmp(argv[1], "right") == 0)
	    client.posv[X] = step;
    else if (strcmp(argv[1], "up") == 0)
	    client.posv[Y] = step;
    else if (strcmp(argv[1], "down") == 0)
	    client.posv[Y] = -step;

    return OK;
}

gboolean
CMD_toggle_hud(sh_state_t *st, int argc, char **argv, GError **err)
{
    client.hud_enabled ^= 1;
    return OK;
}

gboolean
CMD_fov(sh_state_t *st, int argc, char **argv, GError **err)
{
    real fov;

    if (argc != 2) {
	    g_set_error(err, SCS_ERROR, SCS_ERROR_SH, "usage: %s <value>", argv[0]);
	    return FAIL;
    }

    fov = atof(argv[1]);

    client.fov = fov;

    vid_perspective(client.fov,
                    client.res[X] / client.res[Y],
                    client.znear, client.zfar);

    return OK;
}

gboolean
CMD_warp(sh_state_t *st, int argc, char **argv, GError **err)
{
    vec3_t v, loc;
    int i;
    int32_t id = client.player_id;
    pkt_t *pkt;
    object_t *cam;

    if (argc != 5) {
	    g_set_error(err, SCS_ERROR, SCS_ERROR_SH, "usage: %s <abs|rel> <X> <Y> <Z>", argv[0]);
	    return FAIL;
    }

    // convert arguments
    for (i = 0; i < 3; i++) {
	// convert to strtod and check for errors (make lib/wrapper function)
	//if ((v[i] = atof(argv[2 + i])) == NAN) {
	    v[i] = atof(argv[2 + i]);
/*	    g_set_error(err, SCS_ERROR, SCS_ERROR_SH, "Invalid numeric argument: %s", argv[2 + i]);
	    return FAIL;
	} */
    }

    if (strcmp(argv[1], "abs") == 0) {
	    vec3_cp(v, loc);
    } else if (strcmp(argv[1], "rel") == 0) {
	    vec3_t up, right, fwd;

	/* relative positioning means we need to obtain the direction vectors
	 * from the camera and add (or subtract) the relative position to the 
	 * correct vector
	 */
	cam = g_hash_table_lookup(client.objects, &client.obj_id);
	if (cam == NULL)
	    return OK;

	quat_to_vecs(cam->orient, fwd, up, right);
	vec3_norm(fwd);
	vec3_scale(fwd, v[Z]);
	vec3_norm(up);
	vec3_scale(up, v[Y]);
	vec3_norm(right);
	vec3_scale(right, v[X]);

	vec3_cp(cam->pos, loc);
	vec3_add(loc, fwd, loc);
	vec3_add(loc, up, loc);
	vec3_add(loc, right, loc);
//	print("currently at: [%f,%f,%f] warping to [%f,%f,%f]\n", cam->pos[X], cam->pos[Y], cam->pos[Z], loc[X], loc[Y], loc[Z]);
    } else {
	    g_set_error(err, SCS_ERROR, SCS_ERROR_SH, "Invalid Argument: %s, expected abs or rel", argv[0]);
	    return FAIL;
    }

    // create warp packet
    pkt = net_pkt_new(PKT_PLAYER_WARP, TRUE);
    net_pkt_pack_int32(pkt, 1, &id);
    net_pkt_pack_real(pkt, 3, loc);
    net_send(client.net, client.conn, pkt);

    return OK;
}

gboolean
CMD_target(sh_state_t *st, int argc, char **argv, GError **err)
{
    int32_t i;
    object_t *obj;
    snd_src_t *src;
    snd_t *snd = NULL;

    if (argc != 2) {
	    g_set_error(err, SCS_ERROR, SCS_ERROR_SH,
		    "usage: %s <next|prev>", argv[0]);
	    return FAIL;
    }

    obj = g_hash_table_lookup(client.objects, &client.obj_id);
    assert(obj != NULL);

    if (client.targets->len == 0)
	    return OK;

    for (i = 0; i < client.targets->len; i++) {
	    if (g_array_index(client.targets, uint32_t, i) == obj->target_id) 
	        break;
    }

    if (i == client.targets->len)
	    i = 0;

    if (strcmp(argv[1], "next") == 0) {
	    if (++i > client.targets->len-1)
	        obj->target_id = g_array_index(client.targets, uint32_t, 0);
	    else
	        obj->target_id = g_array_index(client.targets, uint32_t, i);

	    if (! snd_computer_next)
		    snd_computer_next = snd_load(SND_COMPUTER_NEXT);

	    snd = snd_computer_next;
    } else if (strcmp(argv[1], "prev") == 0) {
	    if (--i < 0)
    	    obj->target_id = g_array_index(client.targets, uint32_t,
	    	    client.targets->len-1);
    	else
    	    obj->target_id = g_array_index(client.targets, uint32_t, i);

    	if (! snd_computer_prev)
    		snd_computer_prev = snd_load(SND_COMPUTER_PREV);

    	snd = snd_computer_prev;
    }
    src = snd_src_spawn(snd, g_random_int(), obj, FALSE);
    snd_src_update(src);
    snd_play(src);
    snd_src_frag(src);
    snd_src_register(src);

    return OK;
}

gboolean
CMD_attack1(sh_state_t *st, int argc, char **argv, GError **err)
{
    int32_t id = client.player_id;
    int32_t i  = 1;
    pkt_t *pkt;

    pkt = net_pkt_new(PKT_PLAYER_ATTACK, TRUE);
    net_pkt_pack_int32(pkt, 1, &id);
    net_pkt_pack_int32(pkt, 1, &i);	// weapon id

    net_send(client.net, client.conn, pkt);

    return OK;
}

gboolean
game_load(GError **err)
{
    GError *tmp = NULL;
    char *host = NULL;
    uint16_t port = 0;
    real record;

    client.console.enabled = TRUE;
    print("loading scs engine...\n");

    sh_cmd_add(client.shell, "rotate", CMD_rotate, NULL);
    sh_cmd_add(client.shell, "move", CMD_move, NULL);
    sh_cmd_add(client.shell, "target", CMD_target, NULL);
    sh_cmd_add(client.shell, "attack1", CMD_attack1, NULL);
    sh_cmd_add(client.shell, "warp", CMD_warp, NULL);
    sh_cmd_add(client.shell, "toggle_hud", CMD_toggle_hud, NULL);
    sh_cmd_add(client.shell, "fov", CMD_fov, NULL);

    // texture rendering
    if (! sh_var_exists(client.shell, "r_textures"))
	    sh_var_add(client.shell, "r_textures", NULL);
    sh_var_set_num(client.shell, "r_textures", 1, NULL);

    // normal rendering
    if (! sh_var_exists(client.shell, "r_normals"))
	    sh_var_add(client.shell, "r_normals", NULL);
    sh_var_set_num(client.shell, "r_normals", 0, NULL);

    SDL_WarpMouseInWindow(client.window, client.center[X], client.center[Y]);

#ifndef DEBUG
    //SDL_WM_GrabInput(SDL_GRAB_ON);
#endif
    client.mouse_drag_enabled = TRUE;
    client.mouse_sensitivity = 0.01;
    client.mouse_invert = TRUE;
    client.mouse_glide = TRUE;
    client.mouse_glide_sensitivity = 0.004;

    client.player_id = -1;

    client.targets = g_array_new(FALSE, TRUE, sizeof(uint32_t));
    client.hud_enabled = 1;

    /* launch server if needed */
    if (client.game_type == GAME_SINGLE) {
	    if (sv_launch(FALSE, client.game_type) != 0)
    	    return FAIL;
        } else if (client.game_type == GAME_SERVER) {
            if (sv_launch(FALSE, client.game_type) != 0)
    	        return FAIL;

         	/* add server/port variables if they dont exist */
    	    if (! sh_var_exists(client.shell, "server"))
	            sh_var_add(client.shell, "server", NULL);
    	    sh_var_set_str(client.shell, "server", "127.0.0.1", NULL);

        	if (! sh_var_exists(client.shell, "port"))
    	        sh_var_add(client.shell, "port", NULL);
    	    sh_var_set_num(client.shell, "port", SCS_NET_PORT, NULL);
        } 

        /* initialize network stack */
        if (client.game_type == GAME_SINGLE)
	        client.net = net_init(&net_drv_async, scs.clock, cl_proc_pkt);
        else if (client.game_type == GAME_PLAYBACK) {
	        char *demo;

	    /* grab demo variable */
	    demo = sh_var_get_str(client.shell, "demo");
	    if (! demo) {
	        print("demo selected!\n");
	        return FAIL;
	    }

	    client.net = net_init(&net_drv_vcr, scs.clock, cl_proc_pkt);
	    client.net->filename = demo;
    } else {
	    /* grab internet variables */
	    host = sh_var_get_str(client.shell, "server");
	    if (! host) {
	        print("no hostname to connect to!\n");
	        return FAIL;
	    }
	    port = (uint16_t)sh_var_get_num(client.shell, "port");
	    if (! (port > 0))
	        port = SCS_NET_PORT;	

	    print("connecting to: %s:%d\n", host, port);

	    client.net = net_init(&net_drv_udp, scs.clock, cl_proc_pkt);
    }

    /* decide if we want to record the session */
    if (client.game_type != GAME_PLAYBACK) {
	    record = sh_var_get_num(client.shell, "record");
	    if (record)
	        client.net->recording = TRUE;
    }

    /* connect to server */
    if (! (client.conn = net_connect(client.net, host, port, &tmp))) {
	    printerr("connect: %s\n", tmp->message);
	    return FAIL;
    }

    // initialize lighting
    lighting_init();
    
    /* JH - this data should be shared between client & server threads */
    mesh_init(&client.meshes, NULL);

    cl_hud_init();

    /* JH - use a GL call list until we get a better geometry pipeline */
    starfield = glGenLists(1);
    glNewList(starfield, GL_COMPILE);
    //game_build_starfield(16000, 15000);
        game_build_starfield(8000, 15000);
    glEndList();

    client.initialized = TRUE;

    return OK;
}

gboolean
game_unload_objs(gpointer key, gpointer val, gpointer data)
{
    return TRUE;
}

gboolean
game_unload(GError **err)
{
    client.console.height = client.res[HEIGHT];

    client.initialized = FALSE;

    net_disconnect(client.net, client.conn);
    client.conn = NULL;
    client.obj_id = 0;

    g_hash_table_foreach_remove(client.objects, game_unload_objs, NULL);

    if (client.targets->len)
	    g_array_remove_range(client.targets, 0, client.targets->len);

    if (client.game_type == GAME_SINGLE || client.game_type == GAME_SERVER)
	    sv_shutdown(NULL);

    if (snd_computer_next)
	    snd_unload(snd_computer_next);
    if (snd_computer_prev)
	    snd_unload(snd_computer_prev);

#ifndef DEBUG
    //SDL_WM_GrabInput(SDL_GRAB_OFF);
#endif

    return OK;
}

void
game_update_mouse(void)
{
    vec2_t delta;
    vec3_t angles;
    int32_vec2_t pos;
    real sens;

    if (! client.mouse_drag_enabled)
        return;

    if (client.conn->sock->state != STATE_ESTABLISHED)
	    return;

    vec2_zero(pos);
    SDL_GetMouseState((int *)&pos[X], (int *)&pos[Y]);

    vec2_sub(pos, client.center, delta);

    if (REAL_EQ(delta[X], 0.0) && REAL_EQ(delta[Y], 0.0))
        return;

    vec2_zero(angles);

    if (! client.mouse_glide) {
	    SDL_WarpMouseInWindow(client.window, client.center[X], client.center[Y]);
	    sens = client.mouse_sensitivity;
    } else {
	    int32_t max = 192;

	    if (pos[X] >= client.center[X] + max/2)
	        pos[X] = client.center[X] + max/2;
	    else if (pos[X] <= client.center[X] - max/2)
	        pos[X] = client.center[X] - max/2;

	    if (pos[Y] >= client.center[Y] + max/2)
	        pos[Y] = client.center[Y] + max/2;
	    else if (pos[Y] <= client.center[Y] - max/2)
	        pos[Y] = client.center[Y] - max/2;

	    SDL_WarpMouseInWindow(client.window, pos[X], pos[Y]);
	    sens = client.mouse_glide_sensitivity;
    }

    if (! REAL_EQ(delta[X], 0))
	    angles[X] = sens * delta[X];

    if (! REAL_EQ(delta[Y], 0))
	    angles[Y] = sens * delta[Y] * (client.mouse_invert ? -1.0 : 1.0);

    angles[Z] = 0;
//    angles[Z] = client.mouse_sensitivity * delta[X];

    /* send a packet to server with angles */

    // this is counterintuitive, but correct
    client.rotv[Y] = angles[X];
    client.rotv[X] = angles[Y];
}

static gboolean
game_update(GError **err)
{
    if (! client.console.enabled) {
	    game_update_mouse();

	    if (! REAL_EQ(client.posv[X], 0) ||
	        ! REAL_EQ(client.posv[Y], 0) ||
	        ! REAL_EQ(client.posv[Z], 0) ||
	        ! REAL_EQ(client.rotv[X], 0) ||
	        ! REAL_EQ(client.rotv[Y], 0) ||
	        ! REAL_EQ(client.rotv[Z], 0)) {

	        game_send_player_velocities(client.conn, client.posv, client.rotv);

	        vec3_zero(client.posv);
	        vec3_zero(client.rotv);
	    }
	    bind_update();
    }

    if (client.snd_initialized) {
	    object_t *obj = g_hash_table_lookup(client.objects, &client.obj_id);

	    /* update the sound listener position */
	    if (obj) {
	        vec3_cp(obj->pos, client.snd_listener.pos);
	        vec3_cp(obj->orient, client.snd_listener.orient);
	        vec3_cp(obj->posv, client.snd_listener.posv);

	        snd_update(&client.snd_listener);
	        snd_update_srcs();
	        snd_src_reap();
	    }
    }

    return OK;
}

/**
 *
 * Callback function which is called on each object in the universe when
 * it is to be drawn.
 *
 * @param object id
 * @param the object itself
 * @param nothing
 *
 */
void
game_draw_objs(gpointer key, gpointer val, gpointer data)
{
/*    mat4x4_t m;
    object_t *obj = val;
    vec3_t zero;

    vec3_zero(zero);

    if (obj->id == client.obj_id)
	return;

    glPushMatrix();
        glTranslatef(obj->pos[X], obj->pos[Y], obj->pos[Z]);
        quat_to_mat_transpose(obj->orient, m);
	glMultMatrix(m);
    	mesh_draw(obj->mesh);
//	ui_draw_axis(zero, obj->mesh->radius * 2);
    glPopMatrix();*/
    mat4x4_t m;
    object_t *obj = val;
    vec3_t zero;

    vec3_zero(zero);

    if (obj->id != client.obj_id) { // Only draw if it's NOT the client's object
        glPushMatrix();
            glTranslatef(obj->pos[X], obj->pos[Y], obj->pos[Z]);
            quat_to_mat_transpose(obj->orient, m);
            glMultMatrix(m);
            mesh_draw(obj->mesh);
        //  ui_draw_axis(zero, obj->mesh->radius * 2);
        glPopMatrix();
    }
    // If obj->id == client.obj_id, the if block is skipped, no push/pop occurs,
    // and stack balance is maintained.
}

void
game_print_objects(gpointer key, gpointer val, gpointer data)
{
    object_t *obj = val;
    gint *k = key;
    printf("0x%08x 0x%08x  [%f/%f/%f]\n", *k, obj->id, obj->pos[X], obj->pos[Y], obj->pos[Z]);
}


/**
 * Draws the 3D universe and the HUD
 */
static gboolean 
game_draw(GError **err)
{
    object_t *cam;
    mat4x4_t m;

    if (client.obj_id == 0) {
	    printf("camera not initialized\n");
	    return OK;
    }

    // try to obtain the camera object
    cam = g_hash_table_lookup(client.objects, &client.obj_id);

    if (cam == NULL) {
	    g_hash_table_foreach(client.objects, game_print_objects, NULL);
	    printf("no camera! [0x%08x]\n", client.obj_id);
	    exit(1);
	    return OK;
    }

    // --- VERY FIRST THING IN YOUR FRAME RENDERING FUNCTION ---
#if 0    
    GLenum initial_err;
    while ((initial_err = glGetError()) != GL_NO_ERROR) {
//            printf("DEBUG: Clearing initial OpenGL error at frame start: 0x%x\n", initial_err);
    }
    // --- END CLEARING ---
#endif

    glViewport(0, 0, client.res[WIDTH], client.res[HEIGHT]);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear all buffers

    // --- Phase 1: Render Starfield (Always around Camera) ---
    // The starfield needs its own projection because the main scene's might be changed later.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (real)client.res[WIDTH] / (real)client.res[HEIGHT], 0.1, 10000000.0); // Use same perspective as main scene
    glGetFloatv(GL_PROJECTION_MATRIX, client.proj_mat); // Assuming proj_mat is a mat4x4_t (GLfloat[16] or GLdouble[16])

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); // Start with an identity modelview for the starfield

    // Apply ONLY camera's orientation (rotation) to the starfield's modelview.
    // This makes the starfield rotate with the camera's view direction.
    quat_to_mat(cam->orient, m);
    glMultMatrix(m);

    // Now, translate the starfield geometry *by the camera's world position*.
    // This makes the starfield appear centered on the camera's actual location,
    // effectively cancelling out the camera's movement for the starfield's visuals.
    glTranslatef(cam->pos[X], cam->pos[Y], cam->pos[Z]);

    lighting_disable();

    glDisable(GL_DEPTH_TEST);  // Ensure starfield is drawn behind everything
    glDepthMask(GL_FALSE);     // Ensure starfield doesn't write to depth buffer

    glCallList(starfield); // Draw your starfield (assumed to be defined around origin)

    glEnable(GL_DEPTH_TEST);   // Re-enable for subsequent 3D objects
    glDepthFunc(GL_LEQUAL);    // Standard depth function
    glDepthMask(GL_TRUE);      // Re-enable depth writing

    /* --- Phase 2: Render Main 3D Game Scene --- */

    // Set 3D Modelview (Main Camera)
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); // Start with identity

    // Apply camera's full transform (rotation and inverse translation)
    quat_to_mat(cam->orient, m);
    mat4x4_translate(m, -cam->pos[X], -cam->pos[Y], -cam->pos[Z]); // Apply inverse position
    glMultMatrix(m); // Apply the full camera matrix

    // --- Draw your main 3D scene elements ---
    lighting_disable();

    ui_draw_3d_grid(10.0, 50, 50);

    // Debug axis - ensure it uses the main depth settings unless you specifically want GL_ALWAYS
    //glDepthFunc(GL_ALWAYS); // Use GL_ALWAYS to ensure it's always on top of scene
    vec3_t zero;
    vec3_zero(zero);
    ui_draw_axis(zero, 500);
    //glDepthFunc(GL_LEQUAL); // Restore depth func

    ui_draw_spiral_wireframe(32.0, 500.0);
    lighting_enable();

    g_hash_table_foreach(client.objects, game_draw_objs, NULL); // Your main objects

#if 1
    // Earth
    vec3_t clong, clat;
    vec3_set(clong, 0.25, 0, 0.0);
    vec3_set(clat, 0.25, 0, 0.0);
    glPushMatrix();
        glTranslatef(128000,-60000,0);
        ui_draw_earth(0,0,0, 60000.0, 2, 5.0, 5.0, clong, clat);
    glPopMatrix();
    // --- End Main 3D Scene ---
#endif

    // --- 2D HUD Drawing Setup ---
#if 1
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Use client.ortho for your virtual resolution here
    glOrtho(0.0, client.ortho[WIDTH], 0.0, client.ortho[HEIGHT], -1.0, 1.0); // Simple Z-range for 2D

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); // Identity for 2D HUD modelview

    // Disable depth testing and writing for 2D HUD elements
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    // --- Draw your 2D HUD components ---
    lighting_disable(); // HUD is typically unlit

    if (client.hud_enabled) {
        cl_target_draw();
    }
    {
        struct tm date_time;
        time_t t;
        char buf[BUFSIZ];
        t = time(NULL);
        localtime_r(&t, &date_time);
        snprintf(buf, BUFSIZ, "%02d:%02d:%02d", date_time.tm_hour, date_time.tm_min, date_time.tm_sec);
        ui_draw_digital_text(buf, client.ortho[WIDTH] - 16 * 12, 0 + 26, 16, 26);
    }
    if (client.hud_enabled) {
        cl_radar_draw();
        cl_crosshair_draw();
        cl_target_computer_draw(); // Assuming this is a 2D component
    }

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);


    // --- End 2D HUD ---
#endif

#if 1
    // BEFORE drawing the 3D HUD object:
    // 1. Temporarily disable writing to the main depth buffer.
    // 2. Set depth function to GL_ALWAYS so it draws over everything.
    //glDisable(GL_DEPTH_TEST);   // Temporarily disable depth testing for the entire HUD setup (safety)
    //glDepthFunc(GL_ALWAYS); // <-- Re-introduce this here
    //glDepthMask(GL_FALSE);      // Do NOT write to the depth buffer for the HUD
    // (We will enable depth test and change depth func *within* the HUD object's drawing if needed for internal occlusion)

    // --- 3D HUD Object Drawing Setup ---
    // Outer 2D Orthographic Projection (defines screen space for the 3D HUD object)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, client.ortho[WIDTH], 0, client.ortho[HEIGHT], -1.0, 100.0); // Sufficient Z-range

    // Outer 2D Modelview (for positioning the 3D object on the 2D screen)
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Enable depth testing for the 3D HUD object
    //glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_ALWAYS);
    //glDepthMask(GL_TRUE); // Ensure its internal depth works

    lighting_enable(); // Enable if your 3D HUD object is lit

    glClear(GL_DEPTH_BUFFER_BIT);

    // Position the "origin" for this 3D HUD object on the 2D screen
    glPushMatrix(); // Push outer 2D modelview
        glTranslatef(client.ortho[WIDTH] / 2.0f, client.ortho[HEIGHT] / 16.0f, 0.0f); // Example position
        glScalef(50.0f, 50.0f, 50.0f); // Scale the miniature 3D scene

        // Set up the INNER 3D Perspective for the miniature object
        glMatrixMode(GL_PROJECTION);
        glPushMatrix(); // Push outer 2D projection
            glLoadIdentity();
            gluPerspective(15.0, (real)client.ortho[WIDTH]/(real)client.ortho[HEIGHT], 0.1, 1000.0); // Small FOV, 1:1 aspect ratio

            // Set up the INNER 3D Modelview for the miniature object's camera
            glMatrixMode(GL_MODELVIEW);
            glPushMatrix(); // Push translated/scaled modelview
                glLoadIdentity();
                glTranslatef(0.0f, -55.0f, -500.0f); // "Camera" for the miniature object

                //glEnable(GL_DEPTH_TEST);
                //glDepthFunc(GL_LEQUAL); // Or GL_LEQUAL, typical for internal 3D occlusion

                // Draw the actual 3D HUD object
                if (client.hud_enabled) {
                    cl_target_computer3d_draw();
                }

                //glDepthFunc(GL_ALWAYS); // Or whatever was appropriate for the outer HUD context
                //glDisable(GL_DEPTH_TEST); // Disable depth test again before popping matrices

            glPopMatrix(); // Pop inner 3D Modelview
        glMatrixMode(GL_PROJECTION);
        glPopMatrix(); // Pop inner 3D Projection
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix(); // Pop outer 2D Modelview

    //glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LEQUAL); // Restore to standard for next frame
    //glDepthMask(GL_TRUE);   // Re-enable depth writing for main scene next frame

    // --- End 3D HUD Object ---
#endif

    glFlush();
    return OK;
}

/**
 * callback for processing SDL keyboard events
 *
 * @param SDL's keyboard event structure
 */
static gboolean
game_key(SDL_KeyboardEvent *k, GError **err)
{
    if (k->type == SDL_KEYDOWN) {
	    switch (k->keysym.sym) {
	        case SDLK_ESCAPE:
		        cl_mode(&menu_callbacks, NULL);
		        break;
	        case SDLK_F1:
		        client.mouse_drag_enabled ^= 1;
		        break;
	        default:
		        bind_key(k);
		        break;
	    }
    } else {
	    bind_key(k);
    }

    return OK;
}

/**
 * callback for processing SDL mouse button events
 *
 * @param SDL's mouse event structure
 */
static gboolean
game_mbutton(SDL_MouseButtonEvent *m, GError **err)
{
    bind_mbutton(m);
    return OK;
}

/**
 * Specify our event callback functions
 */
cl_callbacks_t game_callbacks = {
    game_load,
    game_unload,
    game_update,
    game_draw,
    game_key,
    NULL, /* .text */
    game_mbutton,
    NULL, /* .mwheel */
    NULL, /* .mmotion */
    NULL, /* .jbutton */
    NULL, /* .jmotion */
    NULL, /* .jhat */
    NULL, /* .resize */
};
