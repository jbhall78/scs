#include <string.h>

#include <glib.h>

#include "scs.h"
#include "shared.h"
#include "mesh.h"
#include "client.h"

/***************************************************************************
 *                                                                         *
 * MESH RENDER FUNCTIONS                                                   *
 *                                                                         *
 ***************************************************************************/

/**
 * @brief draw a triangle
 *
 * this function draws a triangle, however, it needs to reworked to simply
 * add the triangle to the polygon pipeline
 *
 * @param mesh structure of the object the triangle is on
 * @param triangle to draw
 *
 */
#ifndef DEDICATED
#include <SDL_opengl.h>
#include "sys_gl.h"

void
mesh_draw_tri(m_mesh_t *m, m_tri_t *tri)
{
    m_mat_t *mat;
    int32_t i, j;
    tex_t *tex;
    real *v;
    int32_t r_textures, r_normals;
    static int32_t counter = 0;

    r_textures = (uint32_t)sh_var_get_num(client.shell, "r_textures");
    r_normals  = (uint32_t)sh_var_get_num(client.shell, "r_normals");

    /* JH - print messages should be rate limited or something
     *      this message generally means the blender export code is broken
     */
    if (tri->mat == 65535 && counter++ >= 100000) {
	print("WARNING: no material attached to triangle\n");
	counter = 0;
	return;
    }
    mat = g_ptr_array_index(m->materials, tri->mat);

    if (mat->lit) {
	glEnable(GL_LIGHTING);
    } else {
//	printf("unlit\n");
	glDisable(GL_LIGHTING);
    }

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat->ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat->diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat->specular);
    glMaterialfv(GL_FRONT, GL_EMISSION, mat->emissive);
    glMaterialf(GL_FRONT, GL_SHININESS, mat->shininess);

		GLint original_matrix_mode;

    if (r_textures == 1 && mat->textures->len > 0) {
	for (i = 0; i < mat->textures->len; i++) {
	    tex = g_ptr_array_index(mat->textures, i);

	    glBindTexture(GL_TEXTURE_2D, tex->id);
	    glEnable(GL_TEXTURE_2D);

	    /* BUG: This should be a numeric comparison */
/*	    if (strncmp(tex->name, "laser", 5) == 0) {
	    	glBlendFunc(GL_ADD,GL_ADD);
		glEnable(GL_BLEND); */
	   /* } else */ if (strncmp(tex->name, "envmap", 6) == 0) {
		//glBlendFunc(GL_ONE,GL_ONE);
	    	//glBlendFunc(GL_ADD,GL_ADD); was uncommented but found later it was generated an OpenGL error
		//glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glEnable(GL_TEXTURE_GEN_S);
	  	glEnable(GL_TEXTURE_GEN_T);

		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	    	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	    } else {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);


                glGetIntegerv(GL_MATRIX_MODE, &original_matrix_mode); // <-- ADD THIS LINE

	//	glPushAttrib(GL_TRANSFORM_BIT); //
		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
		
	    }


	    glBegin(GL_TRIANGLES);
	    for (j = 0; j < 3; j++) {
		if (tri->smooth)
		    v = g_array_index(m->vnorms, vec3_t, tri->vn[j]);
		else
		    v = g_array_index(m->fnorms, vec3_t, tri->fn);
		glNormal3v(v);
		if (tri->has_uv) {
		    v = g_array_index(m->uvcoords, vec2_t, tri->uv[j]);
		    glTexCoord2v(v);
		}
		v = g_array_index(m->vertices, vec3_t, tri->v[j]);
		glVertex3v(v);
	    }
	    glEnd();

	    if (strncmp(tex->name, "envmap", 6) == 0) {
		glDisable(GL_TEXTURE_GEN_S);
	  	glDisable(GL_TEXTURE_GEN_T);
                glDisable(GL_BLEND);
	    } else {
	//	glPopAttrib();
	        glDisable(GL_BLEND);
		                glMatrixMode(original_matrix_mode); // <-- ADD THIS LINE

	    }
	}
    } else {
	glBegin(GL_TRIANGLES);
	for (i = 0; i < 3; i++) {
	    if (tri->smooth)
		v = g_array_index(m->vnorms, vec3_t, tri->vn[i]);
	    else
		v = g_array_index(m->fnorms, vec3_t, tri->fn);
	    glNormal3v(v);

	    v = g_array_index(m->vertices, vec3_t, tri->v[i]);
	    glVertex3v(v);
	}
	glEnd();
    }


    if (mat->textures->len > 0) {
	tex = g_ptr_array_index(mat->textures, 0);
	glDisable(GL_TEXTURE_2D);
    }

    /* draw normals */
    if (r_normals == 1 && tri->smooth) {
	vec3_t e;
	real *n;
	int k;

	glDisable(GL_LIGHTING);
	glColor3f(0,1,0);
    	glLineWidth(1.0);
	glBegin(GL_LINES);
	for (k = 0; k < 3; k++) {
	    n = g_array_index(m->vnorms, vec3_t, tri->vn[k]);
	    v = g_array_index(m->vertices, vec3_t, tri->v[k]);
	    glVertex3v(v);
	    vec3_cp(n, e);
	    vec3_norm(e);
	    vec3_scale(e, 0.30);
	    vec3_add(v, e, e);
	    glVertex3v(e);
	}
	glEnd();
	glEnable(GL_LIGHTING);
    } else if (r_normals == 1) {
	vec3_t e;
	real *n;
	int k;

	glDisable(GL_LIGHTING);
	glColor3f(1,0,1);
    	glLineWidth(1.0);
	glBegin(GL_LINES);
	for (k = 0; k < 3; k++) {
	    //n = g_array_index(m->vnorms, vec3_t, tri->vn[k]);
            n = g_array_index(m->fnorms, vec3_t, tri->fn);
	    v = g_array_index(m->vertices, vec3_t, tri->v[k]);

	    glVertex3v(v);
	    vec3_cp(n, e);
	    vec3_norm(e);
	    vec3_scale(e, 0.30);
	    vec3_add(v, e, e);
	    glVertex3v(e);
	}
	glEnd();
	glEnable(GL_LIGHTING);
    }

    glDisable(GL_LIGHTING);
}


/**
 * draw a mesh
 * @param mesh object
 */
#if 0
void
mesh_draw(m_mesh_t *m)
{
    m_obj_t *obj;
    m_tri_t *tri;
    int i, j;
    uint32_t t;

    glDisable(GL_COLOR_MATERIAL);
    glColor3f(1.0, 1.0, 1.0);

#if 0
    if (strcmp(m->name, "laser") == 0)
	glDisable(GL_LIGHTING);
    else
	glEnable(GL_LIGHTING);
#endif

    /* this is a quick & dirty mesh rendering function, hopefully we will
       only use it for testing */

    /* the real function needs to cull the object to see if it really
       needs to be drawn, and put the geometry into a list to be drawn. */
    for (i = 0; i < m->objects->len; i++) {
	obj = g_ptr_array_index(m->objects, i);

	for (j = 0; j < obj->tris->len; j++) {
	    t   = g_array_index(obj->tris, uint32_t, j);
	    tri = g_ptr_array_index(m->triangles, t);

	    mesh_draw_tri(m, tri);
	}
    }
}
#endif
#endif

void
mesh_draw(m_mesh_t *m)
{
    GLenum err; // Declare error variable

    // --- CHECK 1: Before any operations in mesh_draw itself ---
    err = glGetError();
    if (err != GL_NO_ERROR) {
        printf("DEBUG: mesh_draw: ERROR at START (before glDisable): 0x%x\n", err);
    }

    glDisable(GL_COLOR_MATERIAL); // This is your reported line (202)
    // --- CHECK 2: Immediately after the reported problematic line ---
    err = glGetError();
    if (err != GL_NO_ERROR) {
        printf("DEBUG: mesh_draw: ERROR after glDisable(GL_COLOR_MATERIAL): 0x%x\n", err);
    }

    glColor3f(1.0, 1.0, 1.0);
    // --- CHECK 3: After glColor3f ---
    err = glGetError();
    if (err != GL_NO_ERROR) {
        printf("DEBUG: mesh_draw: ERROR after glColor3f: 0x%x\n", err);
    }

    // ... (skipped #if 0 block) ...

    for (int i = 0; i < m->objects->len; i++) {
        m_obj_t *obj = g_ptr_array_index(m->objects, i);

        for (int j = 0; j < obj->tris->len; j++) {
            uint32_t t   = g_array_index(obj->tris, uint32_t, j);
            m_tri_t *tri = g_ptr_array_index(m->triangles, t);

            // --- CHECK 4: Before calling mesh_draw_tri ---
            err = glGetError();
            if (err != GL_NO_ERROR) {
                printf("DEBUG: mesh_draw: ERROR before mesh_draw_tri for obj %d tri %d: 0x%x\n", i, j, err);
            }

            mesh_draw_tri(m, tri);

            // --- CHECK 5: After calling mesh_draw_tri ---
            err = glGetError();
            if (err != GL_NO_ERROR) {
                printf("DEBUG: mesh_draw: ERROR after mesh_draw_tri for obj %d tri %d: 0x%x\n", i, j, err);
            }
        }
    }
}


/***************************************************************************
 *                                                                         *
 * MESH LOADING FUNCTIONS                                                  *
 *                                                                         *
 ***************************************************************************/

/**
 * calculate the dimensions of the bounding box & sphere of a mesh
 * @param mesh to calculate & store the data in
 */
void
mesh_build_volumes(m_mesh_t *m)
{
    uint32_t i, j;
    real *v, max, len;

    for (i = 0; i < m->vertices->len; i++) {
	v = g_array_index(m->vertices, vec3_t, i);

	for (j = 0; j < 3; j++) {
	    if (v[j] < m->mins[j])
		m->mins[j] = v[j];
	    else if (v[j] > m->maxs[j])
		m->maxs[j] = v[j];
	}
    }

    /* build vertices of bounding box */
    m->box[0][0] = m->mins[0];
    m->box[0][1] = m->mins[1];
    m->box[0][2] = m->mins[2];

    m->box[1][0] = m->mins[0];
    m->box[1][1] = m->maxs[1];
    m->box[1][2] = m->mins[2];

    m->box[2][0] = m->maxs[0];
    m->box[2][1] = m->maxs[1];
    m->box[2][2] = m->mins[2];

    m->box[3][0] = m->maxs[0];
    m->box[3][1] = m->mins[1];
    m->box[3][2] = m->mins[2];

    m->box[4][0] = m->mins[0];
    m->box[4][1] = m->mins[1];
    m->box[4][2] = m->maxs[2];

    m->box[5][0] = m->mins[0];
    m->box[5][1] = m->maxs[1];
    m->box[5][2] = m->maxs[2];

    m->box[6][0] = m->maxs[0];
    m->box[6][1] = m->maxs[1];
    m->box[6][2] = m->maxs[2];

    m->box[7][0] = m->maxs[0];
    m->box[7][1] = m->mins[1];
    m->box[7][2] = m->maxs[2];

    /* now figure out radius for bounding sphere */
    for (i = 0, max = 0; i < 3; i++) {
	len = m->maxs[i] - m->mins[i];
	if (len > max) {
	    max = len;
	}
    }
    m->radius = max / 2;
#if 0
    mesh->radius += (max * 0.05); // pad by 5%
#endif
}

/**
 * load mesh to memory from disk
 *
 * @param mesh object to load into
 * @param name of mesh to load
 * @param exception object
 *
 * @return OK for success FAIL on failure
 */
gboolean
mesh_load_obj(m_mesh_t *mesh, char *name, GError **err)
{
    GError *tmp = NULL;

    if (! xml_mesh_load(mesh, name, &tmp)) {
	g_propagate_error(err, tmp);
	return FAIL;
    }

    mesh_build_volumes(mesh);

    return OK;
}


/***************************************************************************
 *                                                                         *
 * MESH CACHE FUNCTIONS                                                    *
 *                                                                         *
 ***************************************************************************/

/**
 * @brief load mesh to cache from disk
 *
 * callback function which loads a mesh from disk to the mesh cache 
 *
 * @param name of mesh to load
 * @return mesh object
 */
static gpointer
mesh_cache_val_new(gpointer key)
{
    char *name = key;
    m_mesh_t *mesh;
    GError *err = NULL;

    mesh = g_new0(m_mesh_t, 1);
    mesh->name = strdup(name);

    if (! mesh_load_obj(mesh, name, &err)) {
	printerr("Couldnt load mesh: %s: %s\n", name, err->message);
	return NULL;
    }

    return mesh;
}

/**
 * @brief removes a mesh from cache
 *
 * callback function which frees the memory used by a mesh in the mesh cache
 * it is called when the object is removed from the cache
 *
 * @param mesh to remove
 */
static void
mesh_cache_val_destroy(gpointer val)
{
    m_mesh_t *mesh = val;

    /* JH - a lot of data in the mesh structure is not traversed & freed
     *      please fix
     */

    g_free(mesh->name);
    g_free(mesh);
}

/**
 * free the key of a mesh cache entry (the mesh name)
 * @param string to free
 */
static void mesh_cache_key_destroy(gpointer key) { g_free(key); }
/**
 * duplicate a key of a mesh cache entry (the mesh name)
 * @param string to duplicate
 * @return pointer to the string
 */
static gpointer mesh_cache_key_dup(gpointer key) { return strdup(key); }

/* API Functions */

/**
 * initialize the mesh subsystem
 *
 * it is OK to call this from other modules
 *
 * @param[out] empty pointer which will be filled with a new mesh cache object
 * @param[out] exception object
 */
gboolean
mesh_init(GCache **cache, GError **err)
{
    *cache = g_cache_new(mesh_cache_val_new,
	             mesh_cache_val_destroy,
		     mesh_cache_key_dup,
		     mesh_cache_key_destroy,
		     g_str_hash,
		     g_direct_hash,
		     g_str_equal);

    print("mesh system initialized\n");

    return OK;
}

/**
 * insert a mesh into the mesh cache
 *
 * it is OK to call this from other modules
 *
 * @param[in] cache object to load the mesh into
 * @param[in] name of mesh to load
 */
m_mesh_t *
mesh_load(GCache *cache, char *name)
{
    return g_cache_insert(cache, name);
}

/**
 * delete a mesh from the mesh cache
 *
 * it is OK to call this from other modules
 */
void
mesh_unload(GCache *cache, m_mesh_t *m)
{
    g_cache_remove(cache, m->name);
}
