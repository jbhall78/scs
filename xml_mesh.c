#include <string.h>

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "scs.h"
#include "shared.h"
#include "mesh.h"

int16_t
xml_mesh_lookup_matid(m_mesh_t *m, char *name)
{
    int i;

    for (i = 0; i < m->materials->len; i++) {
	m_mat_t *mat = g_ptr_array_index(m->materials, i);
	if (xstrcmp(mat->name, name) == 0)
	    return i;
    }

//    print("WARNING: no material (%s) for %s\n", name, m->name);

    return -1;
}
	
static gboolean
xml_mesh_parse_vindices(m_mesh_t *m, m_obj_t *obj, m_tri_t *tri, xmlNode *node, GError **err)
{
    xmlNode *p1, *p2;
    unsigned char *prop;
    m_tri_t *tri2;
    int32_vec4_t vindices;
    int32_vec4_t nindices;
    int32_vec4_t uvindices[MAX_TEXTURES_PER_MATERIAL];
    int i, j, idx;

    /* read in the indices into the indices arrays */
    i = 0;
    for (p1 = node; p1; p1 = p1->next) {
	if (p1->type != XML_ELEMENT_NODE)
	    continue;
	    
	if (xstrcmp(p1->name, "vertex") != 0) 
	    continue;

      	if (! (prop = xxmlGetProp(p1, "v"))) {
	    g_set_error(err, SCS_ERROR, SCS_ERROR_XML,
		    "no v property on vertex tag");
	    return FAIL;
	}
	vindices[i] = xatoi(prop);
	xmlFree(prop);

      	if ((prop = xxmlGetProp(p1, "vn"))) {
	    nindices[i] = xatoi(prop);
	    tri->smooth = TRUE;
	    xmlFree(prop);
	}

	j = 0;
	for (p2 = p1->children; p2; p2 = p2->next) {
	    if (p2->type != XML_ELEMENT_NODE)
		continue;

	    if (xstrcmp(p2->name, "vt") != 0)
		continue;

	    if (! (prop = xxmlGetProp(p2, "uv"))) {
		g_set_error(err, SCS_ERROR, SCS_ERROR_XML,
		    	"no uv property on vt tag");
		return FAIL;
	    }

	    tri->has_uv = TRUE;

	    uvindices[j][i] = xatoi(prop);
	    xmlFree(prop);

	    if (++j == MAX_TEXTURES_PER_MATERIAL) {
		print("WARNING: too many uv coords per vertex");
		break;
	    }
	}
	i++;
    }

    /* add another triangle if needed */
    if (i == 4) {
	tri2 = g_new0(m_tri_t, 1);
	tri2->smooth = tri->smooth;
	tri2->mat    = tri->mat;
	tri2->fn     = tri->fn;
	tri2->has_uv = tri->has_uv;
	tri2->mesh   = m;

	tri2->v[X]   = vindices[0];
	tri2->v[Y]   = vindices[1];
	tri2->v[Z]   = vindices[2];
	if (tri2->has_uv) {
	    tri2->uv[X]  = uvindices[0][0];
	    tri2->uv[Y]  = uvindices[0][1];
	    tri2->uv[Z]  = uvindices[0][2];
	}
	tri2->vn[X]  = nindices[0];
	tri2->vn[Y]  = nindices[1];
	tri2->vn[Z]  = nindices[2];

	g_ptr_array_add(m->triangles, tri2);
	idx = m->triangles->len-1;
	g_array_append_val(obj->tris, idx);

	tri->v[X] = vindices[0];
	tri->v[Y] = vindices[2];
	tri->v[Z] = vindices[3];
	if (tri->has_uv) {
	    tri->uv[X] = uvindices[0][0];
	    tri->uv[Y] = uvindices[0][2];
	    tri->uv[Z] = uvindices[0][3];
	}
	tri->vn[X] = nindices[0];
	tri->vn[Y] = nindices[2];
	tri->vn[Z] = nindices[3];
    } else if (i == 3) {
	/* no triangulation needed */
	tri->v[X] = vindices[0];
	tri->v[Y] = vindices[1];
	tri->v[Z] = vindices[2];
	if (tri->has_uv) {
	    tri->uv[X] = uvindices[0][0];
	    tri->uv[Y] = uvindices[0][1];
	    tri->uv[Z] = uvindices[0][2];
	}
	tri->vn[X] = nindices[0];
	tri->vn[Y] = nindices[1];
	tri->vn[Z] = nindices[2];
    } else if (i == 2) {
	print("WARNING: edge detected!");
    } else if (i == 1) {
	print("WARNING: point detected!");
    }

    return OK;

}

static gboolean
xml_mesh_parse_faces(m_mesh_t *m, m_obj_t *obj, xmlNode *node, GError **err)
{
    xmlNode *p = NULL;
    unsigned char *prop;
    GError *tmp = NULL;
    m_tri_t *tri;
    int idx;

    for (p = node; p; p = p->next) {
	if (p->type != XML_ELEMENT_NODE)
	    continue;
	    
	if (xstrcmp(p->name, "face") != 0) 
	    continue;

	/* read properties */
	tri = g_new0(m_tri_t, 1);
	tri->mesh = m;
      	if ((prop = xxmlGetProp(p, "normal"))) {
	    tri->smooth = FALSE;
	    tri->fn     = xatoi(prop);
	    xmlFree(prop);
	}
      	if ((prop = xxmlGetProp(p, "material"))) {
	    tri->mat = xml_mesh_lookup_matid(m, (char *)prop);
	    xmlFree(prop);
	}

	/* parse faces */
	if (! xml_mesh_parse_vindices(m, obj, tri, p->children, &tmp)) {
	    g_free(tri);
	    g_propagate_error(err, tmp);
	    return FAIL;
	}

	g_ptr_array_add(m->triangles, tri);
	idx = m->triangles->len-1;
	g_array_append_val(obj->tris, idx);
    }

    return OK;
}

static gboolean
xml_mesh_parse_objs(m_mesh_t *m, xmlNode *node, GError **err)
{
    xmlNode *p = NULL;
    m_obj_t *obj;
    unsigned char *prop;
    GError *tmp = NULL;

    for (p = node; p; p = p->next) {
	if (p->type != XML_ELEMENT_NODE)
	    continue;
	    
	if (xstrcmp(p->name, "object") != 0) 
	    continue;

	/* read properties */
      	if (! (prop = xxmlGetProp(p, "name"))) {
	    g_set_error(err, SCS_ERROR, SCS_ERROR_XML,
		    "no name property on object tag");
	    return FAIL;
	}

	obj = g_new0(m_obj_t, 1);
	obj->name = strdup((char *)prop);
	obj->tris = g_array_new(FALSE, FALSE, sizeof(uint32_t));
	xmlFree(prop);

	/* parse faces */
	if (! xml_mesh_parse_faces(m, obj, p->children, &tmp)) {
	    g_free(obj->name);
	    g_array_free(obj->tris, FALSE);
	    g_free(obj);
	
	    g_propagate_error(err, tmp);
	    return FAIL;
	}

	g_ptr_array_add(m->objects, obj);
    }

    return OK;
}

static gboolean
xml_mesh_parse_vnorms(m_mesh_t *m, xmlNode *node, GError **err)
{
    xmlNode *p = NULL;
    vec3_t v;
    unsigned char *prop;

    for (p = node; p; p = p->next) {
	if (p->type != XML_ELEMENT_NODE)
	    continue;
	    
	if (xstrcmp(p->name, "normal") != 0) 
	    continue;
	vec3_zero(v);

	if ((prop = xxmlGetProp(p, "x"))) {
	    v[X] = xatof(prop);
    	    xmlFree(prop);
	} 
	if ((prop = xxmlGetProp(p, "y"))) {
	    v[Y] = xatof(prop);
    	    xmlFree(prop);
	} 
	if ((prop = xxmlGetProp(p, "z"))) {
	    v[Z] = xatof(prop);
    	    xmlFree(prop);
	}

	g_array_append_val(m->vnorms, v);
    }

    return OK;
}

static gboolean
xml_mesh_parse_fnorms(m_mesh_t *m, xmlNode *node, GError **err)
{
    xmlNode *p = NULL;
    vec3_t v;
    unsigned char *prop;

    for (p = node; p; p = p->next) {
	if (p->type != XML_ELEMENT_NODE)
	    continue;
	    
	if (xstrcmp(p->name, "normal") != 0)
	    continue;

	vec3_zero(v);
	
	if ((prop = xxmlGetProp(p, "x"))) {
	    v[X] = xatof(prop);
    	    xmlFree(prop);
	} 
	if ((prop = xxmlGetProp(p, "y"))) {
	    v[Y] = xatof(prop);
    	    xmlFree(prop);
	} 
	if ((prop = xxmlGetProp(p, "z"))) {
	    v[Z] = xatof(prop);
    	    xmlFree(prop);
	}
	
    	g_array_append_val(m->fnorms, v);
    }

    return OK;
}

static gboolean
xml_mesh_parse_uvcoords(m_mesh_t *m, xmlNode *node, GError **err)
{
    xmlNode *p = NULL;
    vec2_t uv;
    unsigned char *prop;

    for (p = node; p; p = p->next) {
	if (p->type != XML_ELEMENT_NODE)
	    continue;
	    
	if (xstrcmp(p->name, "uv") != 0)
	    continue;

	vec2_zero(uv);
	
	if ((prop = xxmlGetProp(p, "u"))) {
	    uv[U] = xatof(prop);
    	    xmlFree(prop);
	} 
	if ((prop = xxmlGetProp(p, "v"))) {
	    uv[V] = xatof(prop);
    	    xmlFree(prop);
	} 
	
    	g_array_append_val(m->uvcoords, uv);
    }

    return OK;
}

static gboolean
xml_mesh_parse_vertices(m_mesh_t *m, xmlNode *node, GError **err)
{
    xmlNode *p = NULL;
    vec3_t v;
    unsigned char *prop;

    for (p = node; p; p = p->next) {
	if (p->type != XML_ELEMENT_NODE)
	    continue;
	    
	if (xstrcmp(p->name, "vertex") != 0)
	    continue;

	vec3_zero(v);
	
	if ((prop = xxmlGetProp(p, "x"))) {
	    v[X] = xatof(prop);
    	    xmlFree(prop);
	} 
	if ((prop = xxmlGetProp(p, "y"))) {
	    v[Y] = xatof(prop);
    	    xmlFree(prop);
	} 
	if ((prop = xxmlGetProp(p, "z"))) {
	    v[Z] = xatof(prop);
    	    xmlFree(prop);
	}
	
    	g_array_append_val(m->vertices, v);
    }

    return OK;
}

static gboolean
xml_mesh_parse(m_mesh_t *m, xmlNode *root, GError **err)
{
    xmlNode *p = NULL;
    GError *tmp = NULL;

    if (xstrcmp(root->name, "mesh") != 0) {
	g_set_error(err, SCS_ERROR, SCS_ERROR_XML, 
		"root node of wrong type: %s", root->name);
	return FAIL;
    }

    /* allocate our mesh data */
    m->vertices  = g_array_new(FALSE, FALSE, sizeof(vec3_t));
    m->fnorms    = g_array_new(FALSE, FALSE, sizeof(vec3_t));
    m->vnorms    = g_array_new(FALSE, FALSE, sizeof(vec3_t));
    m->uvcoords  = g_array_new(FALSE, FALSE, sizeof(vec2_t));
    m->triangles = g_ptr_array_new();
    m->materials = g_ptr_array_new();
    m->objects   = g_ptr_array_new();

    /* loop over elements and pass them off to the proper handlers */
    for (p = root->children; p; p = p->next) {
	if (p->type != XML_ELEMENT_NODE)
	    continue;
	    
	if (xstrcmp(p->name, "vertices") == 0) {
    	    if (! xml_mesh_parse_vertices(m, p->children, &tmp))
		goto fail;
	} else if (xstrcmp(p->name, "texture-coords") == 0) {
	    if (! xml_mesh_parse_uvcoords(m, p->children, &tmp))
		goto fail;
	} else if (xstrcmp(p->name, "facet-normals") == 0) {
	    if (! xml_mesh_parse_fnorms(m, p->children, &tmp))
		goto fail;
	} else if (xstrcmp(p->name, "vertex-normals") == 0) {
	    if (! xml_mesh_parse_vnorms(m, p->children, &tmp))
		goto fail;
#ifndef DEDICATED
	} else if (xstrcmp(p->name, "material-lib") == 0) {
	    if (! xml_mesh_parse_matlib(m, p, &tmp)) 
		goto fail;
#endif
	} else if (xstrcmp(p->name, "objects") == 0) {
	    if (! xml_mesh_parse_objs(m, p->children, &tmp))
		goto fail;
	}
    }

#if 0
    /* dump vertices */
    {
	real *v;
	int i;

	for (i = 0; i < m->vertices->len; i++) {
	    v = g_array_index(m->vertices, vec3_t, i);
	    print("vertices[%d]: %f/%f/%f\n", i, v[X], v[Y], v[Z]);
	}
    }
    /* dump uvcoords */
    {
	real *v;
	int i;

	for (i = 0; i < m->uvcoords->len; i++) {
	    v = g_array_index(m->uvcoords, vec2_t, i);
	    print("uvcoords[%d]: %f/%f\n", i, v[U], v[V]);
	}
    }
#endif

    return OK;
fail:
    g_propagate_error(err, tmp);
    return FAIL;
}

gboolean
xml_mesh_load(m_mesh_t *mesh, char *name, GError **err)
{
    GError *tmp = NULL;
    char buf[BUFSIZ];
    FILE *f;
    xmlDoc *doc;
    xmlNode *root;

    snprintf(buf, BUFSIZ, "%s.obj.xml", name);

    f = opendatafile(FILE_OBJ, buf, &tmp);
    if (! f) {
	g_propagate_error(err, tmp);
	return FAIL;
    }

    if (! (doc = xmlReadFd(fileno(f), "http://hellcoast.org", NULL,
		    XML_PARSE_RECOVER|XML_PARSE_NONET|XML_PARSE_NOBLANKS))) {
	g_set_error(err, SCS_ERROR, SCS_ERROR_XML,
		"error parsing: %s", buf);
	return FAIL;
    }

    root = xmlDocGetRootElement(doc);

    /* build mesh out of xml file */
    if (! xml_mesh_parse(mesh, root, &tmp)) {
	g_propagate_error(err, tmp);
	return FAIL;
    }

    xmlFreeDoc(doc);
//    xmlCleanupParser();

    print("loaded: (name: %s) (vertices: %d) (triangles: %d) (objects: %d)\n", mesh->name, mesh->vertices->len, mesh->triangles->len, mesh->objects->len);

    return OK;
}
