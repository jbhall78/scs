#include <math.h>

#include <glib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "scs.h"
#include "shared.h"
#include "ui.h"
#include "client.h"
#include "sys_gl.h"

/*
 * drawing functions
 */
void
ui_draw_cursor(widget_t *w)
{
    widget_root_t *wr = w->data;
    real left, right, top, bot;
    vec3_t color = { 1.0, 1.0, 1.0 };

    assert(wr != NULL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, wr->cursor->id);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glColor3v(color);

    left  = wr->mpos[X];
    right = wr->mpos[X] + wr->cursor->size[WIDTH];
    top   = wr->mpos[Y];
    bot   = wr->mpos[Y] - wr->cursor->size[HEIGHT];

    glBegin(GL_QUADS);
	glTexCoord2f(0,  0);
	glVertex2d(left, bot);
	    
	glTexCoord2f(1, 0);
	glVertex2d(right, bot);
    
	glTexCoord2f(1, 1);
	glVertex2d(right, top);
    
	glTexCoord2f(0,  1);
	glVertex2d(left, top);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
}

void
ui_draw_axis(vec3_t pos, double len)
{
    vec3_t color;
    vec3_t dir;
    vec3_t a;

    glLineWidth(3.0);

    color[RED] = 1.0;
    color[GREEN] = 0.0;
    color[BLUE] = 0.0;
    glColor3v(color);
    glBegin(GL_LINES);
        glVertex3f(pos[X], pos[Y], pos[Z]);
        glVertex3f(pos[X] + len, pos[Y], pos[Z]);
    glEnd();
    color[RED] = 0.0;
    color[GREEN] = 1.0;
    color[BLUE] = 0.0;
    glColor3v(color);
    glBegin(GL_LINES);
        glVertex3f(pos[X], pos[Y], pos[Z]);
        glVertex3f(pos[X], pos[Y] + len, pos[Z]);
    glEnd();
    color[RED] = 0.0;
    color[GREEN] = 0.0;
    color[BLUE] = 1.0;
    glColor3v(color);
    glBegin(GL_LINES);
        glVertex3f(pos[X], pos[Y], pos[Z]);
        glVertex3f(pos[X], pos[Y], pos[Z] + len);
    glEnd();
}

void
ui_draw_arrow(direction_t direction, uint16_t padding, widget_posv_t pos, widget_sizev_t size)
{
    float top, bot, left, right, cx, cy;

    top = pos[Y] + size[HEIGHT] - padding*2;
    bot = pos[Y] + padding*2;
    left = pos[X] + padding*2;
    right = pos[X] + size[WIDTH] - padding*2;
    cx = left + ((right - left) / 2);
    cy = bot + ((top - bot) / 2);

    // i could get more complicated with this
    // by applying rotation to a matrix &
    // using an angle as an argument
    // this gets what i want more precisely
    // though
    switch(direction) {
	case NORTH:
	    glBegin(GL_TRIANGLES);
	    glVertex2f(left, bot);
	    glVertex2f(right, bot);
	    glVertex2f(cx, top);
	    glEnd();
	    break;
	case SOUTH:
	    glBegin(GL_TRIANGLES);
	    glVertex2f(cx, bot);
	    glVertex2f(right, top);
	    glVertex2f(left, top);
	    glEnd();
	    break;
	case WEST:
	    glBegin(GL_TRIANGLES);
	    glVertex2f(right, bot);
	    glVertex2f(right, top);
	    glVertex2f(left, cy);
	    glEnd();
	    break;
	case EAST:
	    glBegin(GL_TRIANGLES);
	    glVertex2f(left, bot);
	    glVertex2f(right, cy);
	    glVertex2f(left, top);
	    glEnd();
	    break;
	default:
	    break;
    }
}

// scale: distance between lines
// steps: number of boxes in grid 
// TODO: make work on all axis combinations
void
ui_draw_3d_grid(real GRID_SCALE, real GRID_XSTEPS, real GRID_ZSTEPS)
{
    double zExtent = GRID_SCALE * GRID_ZSTEPS;
    double xExtent = GRID_SCALE * GRID_XSTEPS;
    int loopX, loopZ;

    glPushMatrix();
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.6758, 0.8438, 0.8984);	// light cyan -> white w/a little gray
    glLineWidth(1.0);
    glBegin(GL_LINES);
    for (loopX = -GRID_XSTEPS; loopX <= GRID_XSTEPS; loopX++) {
	double xLocal = GRID_SCALE * loopX;

	glVertex3f(xLocal, 0.0, -zExtent);
	glVertex3f(xLocal, 0.0,  zExtent);
    }
    for (loopZ = -GRID_ZSTEPS; loopZ <= GRID_ZSTEPS; loopZ++) {
	double zLocal = GRID_SCALE * loopZ;

	glVertex3f(-xExtent, 0.0, zLocal);
	glVertex3f( xExtent, 0.0, zLocal);
    }
    glEnd();
    glPopMatrix();
}

/* draws a strange curvy pattern on a plane */
void
ui_draw_strange_2d_object(void)
{
    int i, j;

    glColor3f(0,0,1);

    double radius = 32;

    for (i = 0; i < 360; i+= 15) {
        glBegin(GL_LINES);
            for (j = 0; j < 180; j += 5) {
                double x, y; //, z;

                x = sin(j * DEG2RAD) * sin(i * DEG2RAD) * (radius / 2);
                y = cos(j * DEG2RAD) * cos(i * DEG2RAD) * (radius / 2);
                glVertex2f(x, y);

                x = sin((j + 5) * DEG2RAD) * sin(i * DEG2RAD) * (radius / 2);
                y = cos((j + 5) * DEG2RAD) * cos(i * DEG2RAD) * (radius / 2);
                glVertex2f(x, y);
            }
        glEnd();
    }
}

/* draw a spiral */
void
ui_draw_spiral_wireframe(real radius, real length) 
{
    real i;
    vec3_t v;
    double ang = 0;

    vec3_zero(v);

    glPushMatrix();
//    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glColor3f(1.0, 1.0, 0.0);

    for (i = 0; i < length; i += 1.0) {
	glBegin(GL_LINES);
	glVertex3f(v[X], v[Y], -v[Z]);
	v[X] = sin(ang * DEG2RAD) * (radius / 2);
	v[Y] = cos(ang * DEG2RAD) * (radius / 2);
	v[Z] += 1;
	ang += 10;
	glVertex3f(v[X], v[Y], -v[Z]);
	glEnd();
    }
    glPopMatrix();
}

// radius: radius of sphere
// long_ang: angle between each longitude line
// lat_ang: angle between each latitude line
void
ui_draw_uv_sphere_wireframe(real radius, real long_ang, real lat_ang, vec3_t color_long, vec3_t color_lat)
{
    real i, j;

    glPushMatrix();
    glColor3v(color_long);

    /* longitude lines */
    for (i = 0; i <= 360; i += long_ang) {
	real xx, yy;

	glPushMatrix();
	// try to accomplish without glrotate
	glRotatef(i, 0, 1, 0);

        glBegin(GL_LINES);
            for (j = 0; j < 360; j += 5) {
                double x, y, z;

                x = sin(j * DEG2RAD) * (radius / 2);
                y = cos(j * DEG2RAD) * (radius / 2);
  		z = 0;
                glVertex3f(x, y, z);

                x = sin((j + 5) * DEG2RAD) * (radius / 2);
                y = cos((j + 5) * DEG2RAD) * (radius / 2);
  		z = 0;
                glVertex3f(x, y, z);
            }
        glEnd();
	glPopMatrix();
    }
    glPopMatrix();

    glColor3v(color_lat);
    glPushMatrix();
    static int printed = 0;
    for (i = 0; i <= 360; i+= lat_ang) {
	double xx, yy;
	double a, b, c, d, e, f;

	glPushMatrix();

	a = M_PI / 360;
	b = a * i;
	c = cos(b);
	d = sin(b);
	e = d * (radius / 2);
	glTranslatef(0, c * (radius / 2), 0);

/*
	if (! printed)
	    printf("i: %f, a: %f, b: %f, c: %f, d: %f, e: %f, t: %f\n", i, a, b, c, d, e, c * (radius / 2));
*/

        glBegin(GL_LINES);
            for (j = 0; j < 360; j += 5) {
                double x, y, z;

                x = sin(j * DEG2RAD) * e;
                y = 0;
  		z = cos(j * DEG2RAD) * e; 
                glVertex3f(x, y, z);

                x = sin((j + 5) * DEG2RAD) * e;
                y = 0;
  		z = cos((j + 5) * DEG2RAD) * e;
                glVertex3f(x, y, z);
            }
        glEnd();
	glPopMatrix();
    }

    glPopMatrix();
    printed = 1;
}

static void
ui_draw_digital_char(digital_char_type_t *char_type, digital_num_parts_t *parts, real x, real y, real height, real width)
{
    if (*char_type == DIGITAL_CHAR_COLON) {
	real cx;
	real cy1, cy2;
	real radius = width / 10;
	real i;
	real xx, yy;

	cx  = width / 2 - (radius / 2);
	cy1 = height / 4 * 1;
	cy2 = height / 4 * 3;

	// draw two circles at cx,cy1 & cx,cy2
	glBegin(GL_POLYGON);
	for (i = 0.0; i < M_PI*2; i += (M_PI*2/360*30)) {
	    xx = x + cx + sin(i) * (radius / 2);
	    yy = y + cy1 + cos(i) * (radius / 2);
	    glVertex2f(xx, yy);
	}
	glEnd();

	glBegin(GL_POLYGON);
	for (i = 0.0; i < M_PI*2; i += (M_PI*2/360*30)) {
	    xx = x + cx + sin(i) * (radius / 2);
	    yy = y + cy2 + cos(i) * (radius / 2);
	    glVertex2f(xx, yy);
	}
	glEnd();

	return;
    } else if (*char_type == DIGITAL_CHAR_NUM) {
	real xpadding = width / 10; //5.0;
	real ypadding = height / 15; // 5.0;
	real extrusion_width = xpadding;	// height of 45degree triangle
	real line_width = height / 8; // 15.0;
	real line_height = line_width;

/*
	static int printed = 0;
	if (printed < 10)
	    printf("%d%d%d%d%d%d%d\n", 
parts->mid_top,
parts->left_top,
parts->right_top,
parts->mid_mid,
parts->left_bot,
parts->right_bot,
parts->mid_bot);
	printed++;
*/


	/* left */
	if (parts->left_top) {
	    real left, right, top, top_e, bottom, bottom_e, center;

	    left     = x + xpadding;
	    right    = x + xpadding + line_width;
	    center   = x + xpadding + (line_width / 2);
	    top_e    = y + height - ypadding - extrusion_width;
	    top      = y + height - ypadding - extrusion_width * 2;
	    bottom   = y + (height / 2) + extrusion_width;
	    bottom_e = y + (height / 2);

	    glBegin(GL_POLYGON);
	        glVertex2f(center, bottom_e);
		glVertex2f(right, bottom);
		glVertex2f(right, top);
		glVertex2f(center, top_e);
		glVertex2f(left, top);
		glVertex2f(left, bottom);
	    glEnd();
	}
	if (parts->left_bot) {
	    real left, right, top, top_e, bottom, bottom_e, center;

	    left     = x + xpadding;
	    right    = x + xpadding + line_width;
	    center   = x + xpadding + (line_width / 2);
	    top_e    = y + (height / 2);
	    top      = y + (height / 2) - extrusion_width;
	    bottom   = y + ypadding + extrusion_width*2;
	    bottom_e = y + ypadding + extrusion_width;

	    glBegin(GL_POLYGON);
	        glVertex2f(center, bottom_e);
		glVertex2f(right, bottom);
		glVertex2f(right, top);
		glVertex2f(center, top_e);
		glVertex2f(left, top);
		glVertex2f(left, bottom);
	    glEnd();
	}
	/* right */
	if (parts->right_top) {
	    real left, right, top, top_e, bottom, bottom_e, center;

	    left     = x + width  - xpadding - line_width;
	    right    = x + width  - xpadding;
	    center   = x + width  - xpadding - (line_width / 2);
	    top_e    = y + height - ypadding - extrusion_width;
	    top      = y + height - ypadding - extrusion_width * 2;
	    bottom   = y + (height / 2) + extrusion_width;
	    bottom_e = y + (height / 2);

	    glBegin(GL_POLYGON);
	        glVertex2f(center, bottom_e);
		glVertex2f(right, bottom);
		glVertex2f(right, top);
		glVertex2f(center, top_e);
		glVertex2f(left, top);
		glVertex2f(left, bottom);
	    glEnd();
	}
	if (parts->right_bot) {
	    real left, right, top, top_e, bottom, bottom_e, center;

	    left     = x + width  - xpadding - line_width;
	    right    = x + width  - xpadding;
	    center   = x + width  - xpadding - (line_width / 2);
	    top_e    = y + (height / 2);
	    top      = y + (height / 2) - extrusion_width;
	    bottom   = y + ypadding + extrusion_width*2;
	    bottom_e = y + ypadding + extrusion_width;

	    glBegin(GL_POLYGON);
	        glVertex2f(center, bottom_e);
		glVertex2f(right, bottom);
		glVertex2f(right, top);
		glVertex2f(center, top_e);
		glVertex2f(left, top);
		glVertex2f(left, bottom);
	    glEnd();
	}
	/* middle */
	if (parts->mid_top) {
	    real left_e, left, right, right_e, top, bottom, center;
	    left_e  = x + xpadding * 2;
	    left    = x + xpadding * 2 + extrusion_width;
	    right_e = x + width - xpadding * 2;
	    right   = x + width - xpadding * 2 - extrusion_width;
	    top     = y + height - ypadding;
	    bottom  = y + height - ypadding - line_height;
	    center  = y + height - ypadding - (line_height / 2);

	    glBegin(GL_POLYGON);
	        glVertex2f(left_e, center);
		glVertex2f(left, bottom);
		glVertex2f(right, bottom);
		glVertex2f(right_e, center);
		glVertex2f(right, top);
		glVertex2f(left, top);
	    glEnd();
	} 
	if (parts->mid_mid) {
	    real left_e, left, right, right_e, top, bottom, center;
	    left_e  = x + xpadding * 2;
	    left    = x + xpadding * 2 + extrusion_width;
	    right_e = x + width - xpadding * 2;
	    right   = x + width - xpadding * 2 - extrusion_width;
	    center  = y + (height / 2);
	    top     = y + (height / 2) + (line_height / 2);
	    bottom  = y + (height / 2) - (line_height / 2);

	    glBegin(GL_POLYGON);
	        glVertex2f(left_e, center);
		glVertex2f(left, bottom);
		glVertex2f(right, bottom);
		glVertex2f(right_e, center);
		glVertex2f(right, top);
		glVertex2f(left, top);
	    glEnd();
	}
	if (parts->mid_bot) {
	    real left_e, left, right, right_e, top, bottom, center;
	    left_e  = x + xpadding*2;
	    left    = x + xpadding*2 + extrusion_width;
	    right_e = x + width - xpadding*2;
	    right   = x + width - xpadding*2 - extrusion_width;
	    center  = y + ypadding + (line_height / 2);
	    top     = y + ypadding + line_height;
	    bottom  = y + ypadding;

	    glBegin(GL_POLYGON);
	        glVertex2f(left_e, center);
		glVertex2f(left, bottom);
		glVertex2f(right, bottom);
		glVertex2f(right_e, center);
		glVertex2f(right, top);
		glVertex2f(left, top);
	    glEnd();
	}
    }
}

void
ui_draw_digital_text(char *str, int _x, int _y, int _width, int _height)
{
    int i, len;
    char c;
    real height, width, x, y;
    real px;
    /*  _
     * |_|.
     * |_|.
     */

    // make sure width & height are divisible by two
    // otherwise things aren't pixel perfect
    assert(_height % 2 == 0);
    assert(_width % 2 == 0);

    // convert to floating point now
    // to ensure no errors during casts
    height = (real)_height;
    width = (real)_width;
    x = (real)_x;
    y = (real)_y;
    px = x;

    digital_char_type_t char_type;
    digital_num_parts_t num_parts;

//    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.0, 1.0, 0.0);

    len = strlen(str);
    for (i = 0; i < len; i++) {
	memset(&num_parts, 0, sizeof(digital_num_parts_t));
	c = str[i];
	switch (c) {
	    case '1':
		char_type = DIGITAL_CHAR_NUM;
		num_parts.right_top = 1;
		num_parts.right_bot = 1;
		break;
	    case '2':
		char_type = DIGITAL_CHAR_NUM;
		num_parts.mid_top   = 1;
		num_parts.right_top = 1;
		num_parts.mid_mid   = 1;
		num_parts.left_bot  = 1;
		num_parts.mid_bot   = 1;
		break;
	    case '3':
		char_type = DIGITAL_CHAR_NUM;
		num_parts.mid_top   = 1;
		num_parts.right_top = 1;
		num_parts.mid_mid   = 1;
		num_parts.right_bot = 1;
		num_parts.mid_bot   = 1;
		break;
	    case '4':
		char_type = DIGITAL_CHAR_NUM;
		num_parts.right_top = 1;
		num_parts.left_top  = 1;
		num_parts.mid_mid   = 1;
		num_parts.right_bot = 1;
		break;
	    case '5':
		char_type = DIGITAL_CHAR_NUM;
		num_parts.mid_top   = 1;
		num_parts.left_top  = 1;
		num_parts.mid_mid   = 1;
		num_parts.right_bot = 1;
		num_parts.mid_bot   = 1;
		break;
	    case '6':
		char_type = DIGITAL_CHAR_NUM;
		num_parts.mid_top   = 1;
		num_parts.left_top  = 1;
		num_parts.mid_mid   = 1;
		num_parts.left_bot  = 1;
		num_parts.right_bot = 1;
		num_parts.mid_bot   = 1;
		break;
	    case '7':
		char_type = DIGITAL_CHAR_NUM;
		num_parts.mid_top   = 1;
		num_parts.right_top = 1;
		num_parts.right_bot = 1;
		break;
	    case '8':
		char_type = DIGITAL_CHAR_NUM;
		num_parts.mid_top   = 1;
		num_parts.left_top  = 1;
		num_parts.right_top = 1;
		num_parts.mid_mid   = 1;
		num_parts.left_bot  = 1;
		num_parts.right_bot = 1;
		num_parts.mid_bot   = 1;
		break;
	    case '9':
		char_type = DIGITAL_CHAR_NUM;
		num_parts.mid_top   = 1;
		num_parts.left_top  = 1;
		num_parts.right_top = 1;
		num_parts.mid_mid   = 1;
		num_parts.right_bot = 1;
		num_parts.mid_bot   = 1;
		break;
	    case '0':
		char_type = DIGITAL_CHAR_NUM;
		num_parts.mid_top   = 1;
		num_parts.left_top  = 1;
		num_parts.right_top = 1;
		num_parts.left_bot  = 1;
		num_parts.right_bot = 1;
		num_parts.mid_bot   = 1;
		break;
	    case ':':
		char_type = DIGITAL_CHAR_COLON;
		break;
	    default:
		print("warning: cannot draw char with digital font: '%c'\n", c);
		continue;
		break;
	}
	ui_draw_digital_char(&char_type, &num_parts, px, y, height, width);
	px += width;
    }
}

// mode= 0=wireframe 1=fill 2=texture
#if 0
void
ui_draw_earth(real x, real y, real z, real radius, int mode, real long_ang, real lat_ang, vec3_t color_long, vec3_t color_lat)
{
    char buf[BUFSIZ];
    real rotation_speed = 0.25;
    static GLUquadric *quad = NULL;

    // configure draw style & normals
    if (quad == NULL) {
	quad = gluNewQuadric();
	if (mode == 0) {
	    gluQuadricDrawStyle(quad, GLU_LINE);
	} else if (mode == 1) {
	    gluQuadricDrawStyle(quad, GLU_FILL);
    	    gluQuadricNormals(quad, GLU_SMOOTH);
	} else if (mode == 2) {
	    gluQuadricDrawStyle(quad, GLU_FILL);
    	    gluQuadricNormals(quad, GLU_SMOOTH);
    	    gluQuadricTexture(quad, 1);
	}
    }

    // draw sphere
    glPushMatrix();

    // configure material
    m_mat_t *mat = g_new0(m_mat_t, 1);
    mat->lit = TRUE;
    mat->ambient[R] = 0;
    mat->ambient[G] = 0;
    mat->ambient[B] = 0;
    mat->ambient[A] = 0;
    mat->diffuse[R] = 0;
    mat->diffuse[G] = 0;
    mat->diffuse[B] = 0;
    mat->diffuse[A] = 0;
    mat->specular[R] = 0.1;
    mat->specular[G] = 0.1;
    mat->specular[B] = 0.1;
    mat->specular[A] = 0.0;
    // self-lit object
    mat->emissive[R] = 0.0;
    mat->emissive[G] = 0.0;
    mat->emissive[B] = 0.0;
    mat->shininess = 16.1;


    vec4_t pos;
    vec4_set(pos, 0.45, -0.10, 0.45, 0);
    glLightfv(GL_LIGHT0, GL_POSITION, pos);

    // send texture data to opengl
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat->ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat->diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat->specular);
    glMaterialfv(GL_FRONT, GL_EMISSION, mat->emissive);
    glMaterialf(GL_FRONT, GL_SHININESS, mat->shininess);

//    glEnable(GL_LIGHTING);
//    glEnable(GL_LIGHT0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    if (mode == 2) {
	glBindTexture(GL_TEXTURE_2D, tex_test->id);
	glEnable(GL_TEXTURE_2D);

//	glMatrixMode(GL_TEXTURE);
//	glLoadIdentity();
    }

    // apply translation
    glTranslatef(x, y, z);

    // rotate & spin object
    {
	static real rotation = 0;
	rotation += rotation_speed;
	if (rotation >= 360) {
	    rotation = 0;
	}

	glRotatef(180.0, 0, 0, 1);
	//glRotatef(90.0, 1.0, 0, 0);
	glRotatef(90 - 23.6, 1.0, 0, 0);
	glRotatef(rotation,0.0, 0.0, 1);
    }
//    glEnable(GL_LIGHTING);
    //glEnable(GL_POLYGON_SMOOTH);
    //glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

//    glEnable(GL_POLYGON_OFFSET_FILL);
//    glPolygonOffset(1.0, 1.0);
//    gluSphere(quad, radius, 360, 360);
//    glDisable(GL_POLYGON_OFFSET_FILL);

//    glDisable(GL_LIGHTING);
//    glDisable(GL_LIGHT0);
    glColor3f(1.0, 1.0, 1.0);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    gluSphere(quad, radius, 360, 360);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


//    glDisable(GL_LIGHTING);
    if (mode == 2) {
	glDisable(GL_TEXTURE_2D);
    }
    glClear(GL_DEPTH_BUFFER_BIT);       // we dont want stuff in the world interfering

    glPopMatrix();

    glPushMatrix();
    // apparently, the texcoords gluSphere generates are screwed up
    {
	static real rotation = 0;
	rotation += rotation_speed;
	if (rotation >= 360) {
	    rotation = 0;
	}

	glTranslatef(x, y, z);
	//glRotatef(180.0, 0, 0, 1);
	//glRotatef(90.0, 1.0, 0, 0);
	glRotatef(23.6, 1.0, 0, 0);

	// fix long/lat lines
	// -10 degree looks ok after looking at a few map images
	glRotatef(-10, 0.0, 1.0, 0);

	// apply movement rotation
	glRotatef(rotation, 0.0, 1.0, 0);
    }
    glLineWidth(1.0);
    ui_draw_uv_sphere_wireframe(radius*2, long_ang, lat_ang, color_long, color_lat);
    glPopMatrix();

    glDisable(GL_LIGHT0);
}
#endif

    //glEnable(GL_POLYGON_SMOOTH);
    //glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

void
ui_draw_earth(real x, real y, real z, real radius, int mode, real long_ang, real lat_ang, vec3_t color_long, vec3_t color_lat)
{
    char buf[BUFSIZ];
//    real axis_rotation = 23.6; // standard globe axis
    real axis_rotation = 0; // standard globe axis
//    real rotation_speed = 0.01; // slow "realistic" rotation
    real rotation_speed = 0.1;  // demo rotation
    static GLUquadric *quad = NULL;
    static m_mat_t *mat = NULL;
    static tex_t *tex = NULL;
    static real rotation = 0;
    quat_t q;
    mat4x4_t m;

    lighting_enable();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    // object lighting parameters 
    if (mat == NULL)
	mat = g_new0(m_mat_t, 1);
    assert(mat != NULL);

    // configure opengl quadric
    if (quad == NULL) {
	quad = gluNewQuadric();
	if (mode == 0) {
	    gluQuadricDrawStyle(quad, GLU_LINE);
	} else if (mode == 1) {
	    gluQuadricDrawStyle(quad, GLU_FILL);
    	    gluQuadricNormals(quad, GLU_SMOOTH);
	} else if (mode == 2) {
	    gluQuadricDrawStyle(quad, GLU_FILL);
    	    gluQuadricNormals(quad, GLU_SMOOTH);
    	    gluQuadricTexture(quad, 1);
	}
    }
    assert(quad != NULL);

    // texture material
    mat->lit = TRUE;
    mat->ambient[RED] = 0;
    mat->ambient[GREEN] = 0;
    mat->ambient[BLUE] = 0;
    mat->ambient[ALPHA] = 1;
    mat->diffuse[RED] = 1;
    mat->diffuse[GREEN] = 0.9;
    mat->diffuse[BLUE] = 0.8;
    mat->diffuse[ALPHA] = 1;
    mat->specular[RED] = 1.0;
    mat->specular[GREEN] = 1.0;
    mat->specular[BLUE] = 1.0;
    mat->specular[ALPHA] = 1.0;
    mat->emissive[RED] = 0.0;
    mat->emissive[GREEN] = 0.0;
    mat->emissive[BLUE] = 0.0;
    mat->emissive[ALPHA] = 1.0;
    mat->shininess = 16.1;
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat->ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat->diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat->specular);
    glMaterialfv(GL_FRONT, GL_EMISSION, mat->emissive);
    glMaterialf(GL_FRONT, GL_SHININESS, mat->shininess);

    if (mode == 2) {
	if (tex == NULL) {
	    tex = tex_load("earth/hires.png");
	}
	assert(tex != NULL);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex->id);
    }

    // set rotational parameters
    rotation += rotation_speed;
    if (rotation >= 360) {
	rotation = 0;
    }

    // earth 
    glPushMatrix();
        // rotate sphere
        glRotatef(180.0, 0, 0, 1);			// Z
        glRotatef(90 - axis_rotation, 1.0, 0, 0);	// X
        glRotatef(rotation,0.0, 0.0, 1);		// Z

        // draw sphere
        glColor3f(1.0, 1.0, 1.0);

	// use polygon offset feature to draw sphere & long/lat overlay on top
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(2.0, 2.0);
        gluSphere(quad, radius, 360, 360);
        glDisable(GL_POLYGON_OFFSET_FILL);
    glPopMatrix();

    lighting_disable();

    if (mode == 2)
	glDisable(GL_TEXTURE_2D);

    // longitude / latitude lines 
    glPushMatrix();
	glRotatef(axis_rotation, 1.0, 0, 0);		// X

	// fixup long/lat lines
	// -10 degree looks ok after looking at a few map images
	glRotatef(-10, 0.0, 1.0, 0);			// Y

	// apply movement rotation
	glRotatef(rotation, 0.0, 1.0, 0);		// Y

        // draw long/lat lines
        glLineWidth(1.0);

	// use polygon offset feature to draw sphere & long/lat overlay on top
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        ui_draw_uv_sphere_wireframe(radius*2, long_ang, lat_ang, color_long, color_lat);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glPopMatrix();

    lighting_enable();

    // atmosphere material
    mat->lit = TRUE;
    mat->ambient[RED] = 0;
    mat->ambient[GREEN] = 0;
    mat->ambient[BLUE] = 0;
    mat->ambient[ALPHA] = 0;
    mat->diffuse[RED] = 0;
    mat->diffuse[GREEN] = 0.8;
    mat->diffuse[BLUE] = 1;
    mat->diffuse[ALPHA] = 0.2;
    mat->specular[RED] = 0.0;
    mat->specular[GREEN] = 0.0;
    mat->specular[BLUE] = 0.0;
    mat->specular[ALPHA] = 0.0;
    mat->emissive[RED] = 0.0;
    mat->emissive[GREEN] = 0.0;
    mat->emissive[BLUE] = 0.0;
    mat->emissive[ALPHA] = 0.0;
    mat->shininess = 16.1;
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat->ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat->diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat->specular);
    glMaterialfv(GL_FRONT, GL_EMISSION, mat->emissive);
    glMaterialf(GL_FRONT, GL_SHININESS, mat->shininess);

    // atmosphere 
    glDisable(GL_TEXTURE_2D);
    glPushMatrix();
        // draw sphere
        glColor3f(1.0, 1.0, 1.0);

	// use polygon offset feature to draw sphere & long/lat overlay on top
        gluSphere(quad, radius + 1000, 360, 360);
    glPopMatrix();

    /* done */
}
