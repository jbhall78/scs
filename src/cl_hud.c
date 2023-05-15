#include <math.h>

#include <SDL_opengl.h>

#include "scs.h"
#include "shared.h"
#include "client.h"
#include "net.h"
#include "mathlib.h"
#include "sys_gl.h"
#include "game.h"

static tex_t *crosshair;
static font_t *fnt;

/*
 * Radar Functions
 */
void
cl_radar_draw_circle(double radius, double rx, double ry)
{
    real x, y;
    real theta;

    glLineWidth(3);
//    glEnable(GL_COLOR_MATERIAL);
//    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);

    glColor4d(0.0, 0.0, 0.1, 0.7);
    glBegin (GL_POLYGON);
	for (theta=0.0; theta<6.29; theta+=0.314/2) {
	    y = sin(theta);
	    x = cos(theta);

	    x = rx + x * radius;
	    y = ry + y * radius;

	    glVertex2(x, y);
	}
    glEnd();

    /* Draw border */
    glColor3d(0.6, 0.0, 0.0);
    glBegin (GL_LINE_LOOP);
    for (theta=0.0; theta<6.29; theta+=0.314/2) {
	y = sin (theta);
	x = cos (theta);

	x = rx + x * radius;
	y = ry + y * radius;

	glVertex2(x, y);
    }
    glEnd();
}

/*
 * cl_radar_get_coords
 *
 * This was one of the last things I worked on while developing this game.
 * I have not cleaned the code up yet.
 *
 * This function takes the position of another object in space relative to
 * the player's ship and returns the screen coordinates of the object
 * which are to be overlayed on two circular graphs (one for forward
 * and one for aft) in the top left and right corners of the screen.
 *
 * @param object in space
 * @param x coordinate
 * @param y coordinate
 */
void
cl_radar_get_coords(object_t *obj, real *x, real *y)
{
    vec2_t front, back;
    real *ptr;
    real radius = 56;
    vec3_t tmpPlane[3];
    vec3_t loc, upVec, rightVec, fwdVec;
    quat_t orient;
    vec4_t FBplane;
    vec4_t LRplane;
    double distance;
    char *FB, *LR;
    object_t *cam;

    if ((cam = g_hash_table_lookup(client.objects, &client.obj_id)) != NULL) {
	vec3_cp(cam->pos, loc);

	quat_cp(cam->orient, orient);
    }

    // get our radar screen coords
    front[0] = radius;
    front[1] = client.ortho[HEIGHT] - radius;

    back[0] = client.ortho[WIDTH]  - radius;
    back[1] = client.ortho[HEIGHT] - radius;

    // get our direction vectors so we can build a plane
    quat_to_vecs(orient, fwdVec, upVec, rightVec);

    vec3_norm(fwdVec);
    vec3_norm(upVec);
    vec3_norm(rightVec);

    // NOTE: add plane building functions to mathlib.c and document them

    // build FBplane & convert our plane into normal/constant form
    {
	vec3_t edge1, edge2;

	vec3_cp(loc, tmpPlane[0]);
	vec3_add(loc, upVec, tmpPlane[1]);
	vec3_add(loc, rightVec, tmpPlane[2]);


	vec3_sub(tmpPlane[1], tmpPlane[0], edge1);
	vec3_sub(tmpPlane[2], tmpPlane[0], edge2);
	vec3_cross(edge1, edge2, FBplane);
	vec3_norm(FBplane);
	FBplane[3] = vec3_dot(FBplane, tmpPlane[0]);
    }

    // build LRplane & convert our plane into normal/constant form
    {
	vec3_t edge1, edge2;

	vec3_cp(loc, tmpPlane[0]);
	vec3_add(loc, upVec, tmpPlane[1]);
	vec3_add(loc, fwdVec, tmpPlane[2]);


	vec3_sub(tmpPlane[1], tmpPlane[0], edge1);
	vec3_sub(tmpPlane[2], tmpPlane[0], edge2);
	vec3_cross(edge1, edge2, LRplane);
	vec3_norm(LRplane);
	LRplane[3] = vec3_dot(LRplane, tmpPlane[0]);
    }

    // now test to see which side our object is on
    { 

	distance = vec3_dot(FBplane, obj->pos) - FBplane[3];
	if (distance < 0) {
	    *x = front[0];
	    *y = front[1];
	    ptr = front;
	    FB = "front";
	} else {
	    *x = back[0];
	    *y = back[1];
	    ptr = back;
	    FB = "back";
	}
	distance = vec3_dot(LRplane, obj->pos) - LRplane[3];
	if (distance < 0) {
	    LR = "left";
	} else {
	    LR = "right";
	}
//	printf("debug: %s, %s\n", FB, LR);
    }



    // lets get the angles and look at them
    {
	    double xA, yA;
	{
	    vec3_t fwd, up, right, dir;

	    quat_to_vecs(orient, fwd, up, right);

	    vec3_sub(loc, obj->pos, dir);
	    vec3_norm(dir);		// direction vector from our location to the object's position

	    vec3_norm(fwd);
	    vec3_norm(dir);
	    vec3_norm(right);
	    vec3_norm(up);

	    vec3_inv(up);

	    xA=acos(vec3_dot(right, dir));

	    yA=acos(vec3_dot(up, dir));

	}
	{
	    double scanline;
	    double sx, sy;
	    double len, xp, yp;
	    double a, b, c;

	    yA *= RAD2DEG; // convert to degrees
	    xA *= RAD2DEG; // convert to degrees

	    xA = 180 - xA;

	    //printf("angles: %s/%s x[%f] y[%f]\n", FB, LR, xA, yA);

	    a = radius*2.0;
	    b = yA*a;
	    c = b/180.0;

	    scanline = (yA*(radius*2.0))/180.0; // convert yA/180 to x/(r*2)
	    scanline -= radius; // figure out the scanline for line of circle
	    sx = sqrt( radius*radius - scanline*scanline ); // circle calc
	    len = abs((-sx) - sx); // get distance between points on line
	    xp = xA*len/180; // convert xA/180 to xp/len
	    *x = ptr[0] + (len/2) - xp; // offset p from origin of circle


	    scanline = (xA*(radius*2.0))/180.0; // convert yA/180 to x/(r*2)
	    scanline -= radius; // figure out the scanline for line of circle
	    sy = sqrt( radius*radius - scanline*scanline ); // circle calc
	    len = abs((-sy) - sy); // get distance between points on line
	    yp = yA*len/180; // convert xA/180 to xp/len
	    *y = ptr[1] + (len/2) - yp; // offset p from origin of circle

	}
    }
}

void
cl_radar_draw_objs(gpointer key, gpointer val, gpointer data)
{
    object_t *obj1 = val;
    object_t *obj2;
    real rx = 0, ry = 0;
    real size = 4;

    if (obj1->id == client.obj_id)
	return;

    if (obj1->notarget)
	return;
    
    cl_radar_get_coords(obj1, &rx, &ry);

//    glDisable(GL_LIGHTING);
//    glEnable(GL_COLOR_MATERIAL);
    glPointSize(size);
    glColor3f(0,0,1);
    
    glBegin(GL_POINTS);
    glVertex2(rx, ry);
    glEnd();

    obj2 = g_hash_table_lookup(client.objects, &client.obj_id);
    assert(obj2 != NULL);

    if (obj1->id == obj2->target_id) {
	size -= 1;
	glColor3f(1,1,0);
	glLineWidth(1.0);
	glBegin(GL_LINES);

	glVertex2(rx-size, ry+size);
	glVertex2(rx-size, ry+size-size/2);

	glVertex2(rx-size, ry+size);
	glVertex2(rx-size+size/2, ry+size);

	glVertex2(rx+size, ry+size);
	glVertex2(rx+size-size/2, ry+size);

	glVertex2(rx+size, ry+size);
	glVertex2(rx+size, ry+size-size/2);


	glVertex2(rx-size, ry-size);
	glVertex2(rx-size, ry-size+size/2);

	glVertex2(rx-size, ry-size);
	glVertex2(rx-size+size/2, ry-size);

	glVertex2(rx+size, ry-size);
	glVertex2(rx+size-size/2, ry-size);

	glVertex2(rx+size, ry-size);
	glVertex2(rx+size, ry-size+size/2);
	glEnd();
    }
}

void
cl_radar_draw(void)
{
    real rx = 0, ry = 0;
    real radius;

    radius = 56;

    rx = radius;
    ry = client.ortho[HEIGHT] - radius;
    cl_radar_draw_circle(radius, rx, ry);

    rx = client.ortho[WIDTH] - radius;
    ry = client.ortho[HEIGHT] - radius;
    cl_radar_draw_circle(radius, rx, ry);

    g_hash_table_foreach(client.objects, cl_radar_draw_objs, NULL);
}

void
cl_crosshair_draw(void)
{
    real width  = 48;
    real height = 48;
    real left, right, top, bot;
    vec4_t color;
    int32_vec2_t pos;
    

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glDisable(GL_LIGHTING);

    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, crosshair->id);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    vec4_set(color, 0.45, 0.45, 0.45, 0.75);
    glColor4v(color);

    left  = client.ortho[WIDTH]/2 - width/2;
    right = client.ortho[WIDTH]/2 - width/2 + width;
    top   = client.ortho[HEIGHT]/2 - height/2 + height;
    bot   = client.ortho[HEIGHT]/2 - height/2;

    glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex2(left, bot);
	    
	glTexCoord2f(1, 0);
	glVertex2(right, bot);
    
	glTexCoord2f(1, 1);
	glVertex2(right, top);
    
	glTexCoord2f(0, 1);
	glVertex2(left, top);
    glEnd();

    if (client.mouse_glide) {
	int32_t max = 192;
	real xstep = (real)client.ortho[WIDTH]/client.res[WIDTH];
	real ystep = (real)client.ortho[HEIGHT]/client.res[HEIGHT];

	SDL_GetMouseState(&pos[X], &pos[Y]);
	if (pos[X] >= client.center[X] + max/2)
	    pos[X] = client.center[X] + max/2;
	else if (pos[X] <= client.center[X] - max/2)
	    pos[X] = client.center[X] - max/2;

	if (pos[Y] >= client.center[Y] + max/2)
	    pos[Y] = client.center[Y] + max/2;
	else if (pos[Y] <= client.center[Y] - max/2)
	    pos[Y] = client.center[Y] - max/2;

	left  = xstep*pos[X]-width/2;
	right = xstep*pos[X]-width/2 + width;
	top   = ystep*pos[Y]-height/2 + height;
	bot   = ystep*pos[Y]-height/2;

	vec4_set(color, 1.0, 1.0, 1.0, 1.0);
	glColor4v(color);
	glBegin(GL_QUADS);
	    glTexCoord2f(0, 0);
	    glVertex2(left, bot);

	    glTexCoord2f(1, 0);
	    glVertex2(right, bot);

	    glTexCoord2f(1, 1);
	    glVertex2(right, top);

	    glTexCoord2f(0, 1);
	    glVertex2(left, top);
	glEnd();
    }

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
//    glEnable(GL_LIGHTING);
}

void
cl_target_computer_draw(void)
{
    real width  = 384;
    real height = 172;
    real radius = 16;
    real left, right, top, bot;
    real rx, ry, theta, x, y;
    vec4_t color = { 0, 0.231, 0.529, 0.7 };
    int fntsiz = 8;
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glDisable(GL_LIGHTING);


    /*
     * Draw Curved Box
     */
    glColor4v(color);

    left  = client.ortho[WIDTH]/2 - width/2;
    right = client.ortho[WIDTH]/2 - width/2 + width;
    bot   = 8;
    top   = height+bot;

    glBegin(GL_POLYGON);
	rx = right-radius;
	ry = top-radius;
	for (theta=6.29/4*0; theta<6.29/4*1; theta+=0.314) {
	    y = sin(theta);
	    x = cos(theta);
	    x = rx + x * radius;
	    y = ry + y * radius;
	    glVertex2(x, y);
	}
	glVertex2(left+radius, top);
	rx = left+radius;
	ry = top-radius;
	for (theta=6.29/4*1; theta<6.29/4*2; theta+=0.314) {
	    y = sin(theta);
	    x = cos(theta);
	    x = rx + x * radius;
	    y = ry + y * radius;
	    glVertex2(x, y);
	}
	rx = left+radius;
	ry = bot+radius;
	for (theta=6.29/4*2; theta<6.29/4*3; theta+=0.314) {
	    y = sin(theta);
	    x = cos(theta);
	    x = rx + x * radius;
	    y = ry + y * radius;
	    glVertex2(x, y);
	}
	glVertex2(left+radius, bot);
	glVertex2(right-radius, bot);
	rx = right-radius;
	ry = bot+radius;
	for (theta=6.29/4*3; theta<6.29/4*4; theta+=0.314) {
	    y = sin(theta);
	    x = cos(theta);
	    x = rx + x * radius;
	    y = ry + y * radius;
	    glVertex2(x, y);
	}
	glVertex2(right, top-radius);
    glEnd();


    /* Draw Indicators on target computer */
    {
	object_t *obj1, *obj2;

	obj1 = g_hash_table_lookup(client.objects, &client.obj_id);
	assert(obj1 != NULL);
	obj2 = g_hash_table_lookup(client.objects, &obj1->target_id);

	if (obj2) {
	    x = left + width/2 - ui_font_get_strwidth(fnt, fntsiz, strlen(obj2->mesh->name))/2;
	    y = top - ui_font_get_height(fnt, fntsiz) - 2;

	    vec4_set(color, 0.18, 0.60, 0.60, 1);
	    ui_font_draw_text(fnt, x, y, fntsiz, color, obj2->mesh->name);
	}
    }

    x = left + 5;
    y = bot  + 70;
    vec4_set(color, 0.18, 0.60, 0.60, 1);
    ui_font_draw_text(fnt, x, y, fntsiz, color, "SHD");

    x = x + 3;
    y = y - ui_font_get_height(fnt, 6)-2;
    vec4_set(color, 0.18, 1.00, 1.00, 1);
    ui_font_draw_text(fnt, x, y, fntsiz, color, "100%%");

    x = left + 5;
    y = bot + 40;
    vec4_set(color, 0.18, 0.60, 0.60, 1);
    ui_font_draw_text(fnt, x, y, fntsiz, color, "HULL");

    x = x + 3;
    y = y - ui_font_get_height(fnt, fntsiz)-2;
    vec4_set(color, 0.18, 1.00, 1.00, 1);
    ui_font_draw_text(fnt, x, y, fntsiz, color, "100%%");

    x = left + 5;
    y = bot + ui_font_get_height(fnt, fntsiz);
    vec4_set(color, 0.18, 0.60, 0.60, 1);
    ui_font_draw_text(fnt, x, y, fntsiz, color, "No Cargo");

    x = right - ui_font_get_strwidth(fnt, fntsiz, strlen("SYS")) - 5;
    y = bot + 70;
    vec4_set(color, 0.18, 0.60, 0.60, 1);
    ui_font_draw_text(fnt, x, y, fntsiz, color, "SYS");

    x = right - ui_font_get_strwidth(fnt, fntsiz, strlen("100%")) - 5 - 3; 
    y = y - ui_font_get_height(fnt, fntsiz) - 2;
    vec4_set(color, 0.18, 1.00, 1.00, 1);
    ui_font_draw_text(fnt, x, y, fntsiz, color, "100%%");

    x = right - ui_font_get_strwidth(fnt, fntsiz, strlen("DIST")) - 5;
    y = bot + 40;
    vec4_set(color, 0.18, 0.60, 0.60, 1);
    ui_font_draw_text(fnt, x, y, fntsiz, color, "DIST");

    {
	real distance = 0;
	vec3_t tmp;
	object_t *cam, *obj;
	char buf[BUFSIZ];

	cam = g_hash_table_lookup(client.objects, &client.obj_id);
	if (cam != NULL) {
	    obj = g_hash_table_lookup(client.objects, &cam->target_id);
	    
	    if (obj != NULL) {
		vec3_sub(cam->pos, obj->pos, tmp);
		distance = vec3_len(tmp);
		distance /= 100;
	    }
	}
	    
    	sprintf(buf, "%.02f", distance);
	
	x = right - ui_font_get_strwidth(fnt, fntsiz, strlen(buf)) - 5 - 3;
	y = y - ui_font_get_height(fnt, fntsiz)-2;
	vec4_set(color, 0.18, 1.00, 1.00, 1);
	ui_font_draw_text(fnt, x, y, fntsiz, color, buf);
    }

    glDisable(GL_BLEND);
//    glEnable(GL_LIGHTING);

}

void
cl_target_draw(void)
{
    vec2_t mins, maxs;
    mat4x4_t m1, m2, m3;
    int32_t viewport[4];
    int32_t i;
    vec3_t v;
    object_t *obj, *cam;

    // multiply the modelview matrix with our ships matrix
    cam = g_hash_table_lookup(client.objects, &client.obj_id);
    if (! cam)
	return;

    quat_to_mat(cam->orient, m1);
    mat4x4_translate(m1, -cam->pos[X], -cam->pos[Y], -cam->pos[Z]);

    obj = g_hash_table_lookup(client.objects, &cam->target_id);
    if (! obj)
	return;

    quat_to_mat(obj->orient, m2);

    mat4x4_id(m3);
    mat4x4_translate(m3, obj->pos[X], obj->pos[Y], obj->pos[Z]);

    mat4x4_mult(m1, m3, m1);
    mat4x4_mult(m1, m2, m1);

    viewport[0] = viewport[1] = 0;
    viewport[2] = client.ortho[WIDTH];
    viewport[3] = client.ortho[HEIGHT];

    mins[0] = mins[1] = 65000;
    maxs[0] = maxs[1] = 0;
    for (i = 0; i < 8; i++) {
        project(obj->mesh->box[i], m1, client.proj_mat, viewport, v);
        if (v[X] < mins[0])        mins[0] = v[X];
        if (v[X] > maxs[0])        maxs[0] = v[X];

        if (v[Y] < mins[1])        mins[1] = v[Y];
        if (v[Y] > maxs[1])        maxs[1] = v[Y];
    }
	    

    if (v[Z] > 0 && v[Z] < 1 && v[X] > 0 && v[Y] > 0) {
//	glDisable(GL_LIGHTING);
//	glEnable(GL_COLOR_MATERIAL);
        glColor3f(1,1,0);
        glBegin(GL_LINES);
            glVertex2d(mins[0], mins[1]);
            glVertex2d(maxs[0], mins[1]);

            glVertex2d(maxs[0], mins[1]);
            glVertex2d(maxs[0], maxs[1]);

            glVertex2d(maxs[0], maxs[1]);
            glVertex2d(mins[0], maxs[1]);

            glVertex2d(mins[0], maxs[1]);
            glVertex2d(mins[0], mins[1]);
        glEnd();
    }
}

void
cl_target_computer3d_draw(void)
{
    mat4x4_t mat;
    object_t *cam, *obj;

    cam = g_hash_table_lookup(client.objects, &client.obj_id);
    if (client.targets->len == 0 || cam == NULL)
	return;

    obj = g_hash_table_lookup(client.objects, &cam->target_id);
    if (! obj)
	return;

    lighting_enable();

    glColor4f(1,1,1,1);
    
    glTranslatef(0.0, -75, -100);
    //glTranslatef(0.0, -25, -100);
    //glTranslatef(0.0, -37.5, -150);
    quat_to_mat(cam->orient, mat);
    glMultMatrix(mat);

    quat_to_mat_transpose(obj->orient, mat);
    glMultMatrix(mat);
    
//    glEnable(GL_LIGHTING);

    glPushMatrix();
    mesh_draw(obj->mesh);
    glPopMatrix();
}

void
cl_hud_init(void)
{
    crosshair = tex_load("crosshair.png");
    fnt = ui_font_get("computer");
}
