#include <string.h>

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "scs.h"
#include "shared.h"
#include "mesh.h"
#include "client.h"
#include "server.h"
#include "tex.h"

static gboolean
xml_mesh_mtl_parse_texture(m_mesh_t *m, m_mat_t *mat, xmlNode *node, GError **err)
{
    unsigned char *prop;

    if (! (prop = xxmlGetProp(node, "file"))) {
	g_set_error(err, SCS_ERROR, SCS_ERROR_XML,
		"no file property on texture tag");
	return FAIL;
    }
    
#ifndef DEDICATED
    tex_t *tex;
    tex = tex_load((char *)prop);
    g_ptr_array_add(mat->textures, tex);
#endif
    
    xmlFree(prop);

    return OK;
}

static gboolean
xml_mesh_mtl_parse_lit(m_mesh_t *m, m_mat_t *mat, xmlNode *node, GError **err)
{
    unsigned char *prop;

    if (! (prop = xxmlGetProp(node, "value"))) {
	g_set_error(err, SCS_ERROR, SCS_ERROR_XML,
		"no value property on lit tag");
	return FAIL;
    }
    mat->lit = xatoi(prop);

    xmlFree(prop);

    return OK;
}


static gboolean
xml_mesh_mtl_parse_shininess(m_mesh_t *m, m_mat_t *mat, xmlNode *node, GError **err)
{
    unsigned char *prop;

    if (! (prop = xxmlGetProp(node, "value"))) {
	g_set_error(err, SCS_ERROR, SCS_ERROR_XML,
		"no value property on shininess tag");
	return FAIL;
    }
    mat->shininess = xatof(prop);

    xmlFree(prop);

    return OK;
}

static gboolean
xml_mesh_mtl_parse_color(m_mesh_t *m, m_mat_t *mat, xmlNode *node, GError **err)
{
    vec4_t c;
    unsigned char *prop;

    vec4_zero(c);
    
    if ((prop = xxmlGetProp(node, "r"))) {
	c[R] = xatof(prop);
	xmlFree(prop);
    } 
    if ((prop = xxmlGetProp(node, "g"))) {
	c[G] = xatof(prop);
	xmlFree(prop);
    } 
    if ((prop = xxmlGetProp(node, "b"))) {
	c[B] = xatof(prop);
	xmlFree(prop);
    }
    if ((prop = xxmlGetProp(node, "a"))) {
	c[A] = xatof(prop);
	xmlFree(prop);
    }
    if (! (prop = xxmlGetProp(node, "name"))) {
	g_set_error(err, SCS_ERROR, SCS_ERROR_XML,
		"no name property on color tag");
	return FAIL;
    }
    
    if (xstrcmp(prop, "ambient") == 0) 
	vec4_cp(c, mat->ambient);
    else if (xstrcmp(prop, "diffuse") == 0)
	vec4_cp(c, mat->diffuse);
    else if (xstrcmp(prop, "specular") == 0)
	vec4_cp(c, mat->specular);
    else if (xstrcmp(prop, "emissive") == 0)
	vec4_cp(c, mat->emissive);
    
    xmlFree(prop);

    return OK;
}

static gboolean
xml_mesh_mtl_parse_material(m_mesh_t *m, xmlNode *node, GError **err)
{
    xmlNode *p = NULL;
    GError *tmp = NULL;
    m_mat_t *mat;
    unsigned char *prop;

    /* read properties */
    if (! (prop = xxmlGetProp(node, "name"))) {
	g_set_error(err, SCS_ERROR, SCS_ERROR_XML,
		"no name property on material tag");
	return FAIL;
    }
    
    mat = g_new0(m_mat_t, 1);
    mat->name = strdup((char *)prop);
    mat->lit = TRUE;
    mat->textures = g_ptr_array_new();
    xmlFree(prop);

    for (p = node->children; p; p = p->next) {
	if (xstrcmp(p->name, "color") == 0) {
    	    if (! xml_mesh_mtl_parse_color(m, mat, p, &tmp))
		goto fail;
	} else if (xstrcmp(p->name, "shininess") == 0) {
	    if (! xml_mesh_mtl_parse_shininess(m, mat, p, &tmp)) 
		goto fail;
	} else if (xstrcmp(p->name, "texture") == 0) {
	    if (! xml_mesh_mtl_parse_texture(m, mat, p, &tmp)) 
		goto fail;
	} else if (xstrcmp(p->name, "lit") == 0) {
	    if (! xml_mesh_mtl_parse_lit(m, mat, p, &tmp)) 
		goto fail;
	}
    }
    
    g_ptr_array_add(m->materials, mat);

    return OK;

fail:
    g_ptr_array_free(mat->textures, FALSE);
    g_free(mat->name);
    g_free(mat);
    
    g_propagate_error(err, tmp);
    return FAIL;
}
	    
static gboolean
xml_mesh_mtl_parse_matgroup(m_mesh_t *m, xmlNode *node, GError **err)
{
    xmlNode *p = NULL;
    GError *tmp = NULL;

    /* loop over elements and pass them off to the proper handlers */
    for (p = node; p; p = p->next) {
	if (p->type != XML_ELEMENT_NODE)
	    continue;
	    
	if (xstrcmp(p->name, "material") == 0) {
    	    if (! xml_mesh_mtl_parse_material(m, p, &tmp))
		goto fail;
	}
    }

    return OK;

fail:
    g_propagate_error(err, tmp);
    return FAIL;

}

static gboolean
xml_mesh_mtl_parse(m_mesh_t *m, xmlNode *root, GError **err)
{
    xmlNode *p = NULL;
    GError *tmp = NULL;

    if (g_thread_self() == server.thread)
	return OK;

    if (xstrcmp(root->name, "material-lib") != 0) {
	g_set_error(err, SCS_ERROR, SCS_ERROR_XML, 
		"root node of wrong type: %s", root->name);
	return FAIL;
    }

    /* loop over elements and pass them off to the proper handlers */
    for (p = root->children; p; p = p->next) {
	if (p->type != XML_ELEMENT_NODE)
	    continue;
	    
	if (xstrcmp(p->name, "material-group") == 0) {
    	    if (! xml_mesh_mtl_parse_matgroup(m, p->children, &tmp))
		goto fail;
	}
    }

    return OK;

fail:
    g_propagate_error(err, tmp);
    return FAIL;
}
	    
gboolean
xml_mesh_parse_matlib(m_mesh_t *m, xmlNode *node, GError **err) 
{
    GError *tmp = NULL;
    xmlNode *root;
    xmlDoc *doc;
    unsigned char *prop;
    FILE *f;
	
    /* figure out the filename */
    if (! (prop = xxmlGetProp(node, "name"))) {
	g_set_error(err, SCS_ERROR, SCS_ERROR_XML,
		"no material file specified with material-lib tag");
	return FAIL;
    }

    /* open the material file */
    f = opendatafile(FILE_MTL, (char *)prop, &tmp);
    if (! f) {
	g_propagate_error(err, tmp);
	return FAIL;
    }

    /* parse the xml into a tree */
    if (! (doc = xmlReadFd(fileno(f), "http://hellcoast.org", NULL,
		    XML_PARSE_RECOVER|XML_PARSE_NONET|XML_PARSE_NOBLANKS))) {
	g_set_error(err, SCS_ERROR, SCS_ERROR_XML,
		"error parsing: %s", prop);
	return FAIL;
    }

    /* get root node */
    root = xmlDocGetRootElement(doc);

    /* parse tree */
    if (! xml_mesh_mtl_parse(m, root, &tmp)) {
	g_propagate_error(err, tmp);
	return FAIL;
    }

    /* cleanup */
    xmlFreeDoc(doc);
//    xmlCleanupParser();

    print("loaded material: %s\n", prop);
    xmlFree(prop);

    return OK;

}
