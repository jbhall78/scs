#ifndef _MESH_H
#define _MESH_H

#include <libxml/parser.h>

// use these macros to avoid compiler warnings
// from normal usage of the xml library
#define xstrcmp(a,b) strcmp((char *)a,b)
#define xxmlGetProp(a,b) xmlGetProp(a,(const xmlChar *)b)
#define xatof(a) atof((const char *)a)
#define xatoi(a) atoi((const char *)a)


#define MAX_TEXTURES_PER_MATERIAL 8

typedef struct {
    char *name;

    GArray *vertices;		/* vec3_t */
    GArray *fnorms;		/* vec3_t */
    GArray *vnorms;		/* vec3_t */
    GArray *uvcoords;		/* vec2_t */
    GPtrArray *triangles;	/* tri_t */
    GPtrArray *materials;	/* mat_t */
    GPtrArray *objects;		/* m_obj_t */

    /* bounding volume information */
    vec3_t mins, maxs;
    vec3_t box[8];
    real radius;
} m_mesh_t;

typedef struct {
    char *name;
    GArray *tris;		/* uint32_t - indices to triangles */
} m_obj_t;

typedef struct {
    char *name;
    float diffuse[4];
    float ambient[4];
    float specular[4];
    float emissive[4];
    float shininess;
    GPtrArray *textures;
    GPtrArray *normal_maps;
    gboolean lit;		/* GL_LIGHTING enabled? */
} m_mat_t;

typedef struct {
    uint32_vec3_t v;		/* vertex indices */
    uint32_vec3_t uv;		/* tex uv coords */
    uint32_vec3_t vn;		/* vertex normal indices */
    uint32_t      fn;		/* facet normal index */
    uint16_t      mat;		/* material index */
    gboolean      has_uv;
    gboolean      smooth;	/* smooth or facet normal */
    m_mesh_t	*mesh;		/* pointer to the mesh object this triangle is a part of */
} m_tri_t;

gboolean xml_mesh_load(m_mesh_t *m, char *name, GError **err);
m_mesh_t *mesh_load(GCache *cache, char *name);
void mesh_unload(GCache *cache, m_mesh_t *m);
gboolean mesh_init(GCache **cache, GError **err);
gboolean xml_mesh_parse_matlib(m_mesh_t *m, xmlNode *node, GError **err);
void mesh_draw(m_mesh_t *);
#endif
