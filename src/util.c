#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <errno.h>

#include <glib.h>

#include "scs.h"
#include "shared.h"
#include "config.h"
#include "server.h"
#include "client.h"

static char *prefix[] = {
    NULL,
    "fonts", 
    "textures",
    "obj",
    "obj",
    "sounds",
    "conf",
};
static char *gamedir[] = {
    "data",
    "../data",
    "../../data",
    "/usr/share/scs",
    "/app/share/scs",
};

char *
mkstr(char *fmt, ...)
{
    /* Guess we need no more than 100 bytes. */
    int n, size = 100;
    char *p;
    va_list ap;
    
    if ((p = malloc (size)) == NULL)
	return NULL;

    for (;;) {
	/* Try to print in the allocated space. */
	va_start(ap, fmt);
	n = vsnprintf (p, size, fmt, ap);
	va_end(ap);

	/* If that worked, return the string. */
	if (n > -1 && n < size)
	    return p;

	/* Else try again with more space. */
	if (n > -1)		// glibc 2.1
	    size = n+1;		// precisely what is needed
	else			// glibc 2.0
	    size *= 2;		// twice the old size

	if ((p = realloc (p, size)) == NULL)
	    return NULL;
    }

    return NULL;
}

/* FIXME - XXX - TODO - fix this crap!@$!@$ */
void
_print(int err, const char *file, const char *func, int line, char *fmt, ...)
{
    /* Guess we need no more than 100 bytes. */
    int n, size = 100;
    char *p;
    char *p2;
    va_list ap;
    GThread *thread;

    if ((p = malloc (size)) == NULL)
	return;

    for (;;) {
	/* Try to print in the allocated space. */
	va_start(ap, fmt);
	n = vsnprintf (p, size, fmt, ap);
	va_end(ap);

	/* If that worked, return the string. */
	if (n > -1 && n < size)
	    break;

	/* Else try again with more space. */
	if (n > -1)		// glibc 2.1
	    size = n+1;		// precisely what is needed
	else			// glibc 2.0
	    size *= 2;		// twice the old size

	if ((p = realloc (p, size)) == NULL)
	    return;
    }

    /* I'm not sure if this is good design or not, but it works and it was
       a lot cleaner then adding cl_print and sv_print macros. */
    thread = g_thread_self();

    if (file != NULL && func != NULL) {
	if (err)
	    p2 = mkstr("ERROR: [%c][%s::%s:%d] %s",
		    is_client() ? 'c' : 's',
		    file, func, line, p); 
	else
	    p2 = mkstr("[%-18s] %s", func, p); 
    } else
	p2 = strdup(p);

    sys_print(err, p2);

#ifndef DEDICATED
    if (thread == client.thread && client.console.initialized) {
	con_add_text(p2);
	// this causes real problems
	// if something in cl_draw calls print()
	//
	// i think it was added so that lines printed to the
	// console appear immediately.
	//
/*	if (client.console.enabled) {
	    cl_draw(NULL);
	} */
    } else if (thread == server.thread && server.initialized)
#else
	sv_print(p2);
#endif

    free(p);
    free(p2);
}

FILE *
_opendatafile(const char *file, const char *func, int line, filetype_t t, char *name, GError **err)
{
    FILE *f;
    char buf[BUFSIZ];
    int i;

    for (i = 0; i < G_N_ELEMENTS(gamedir); i++) {
	g_snprintf(buf, sizeof(buf), "%s/%s/%s", gamedir[i], prefix[t], name);

	/* make sure to open in binary mode for windows */
	if ((f = fopen(buf, "rb"))) {
	    _print(0, file, func, line, "loading... \"%s\"\n", name);
	    return f;
	}
    }

    g_set_error(err, SCS_ERROR, SCS_ERROR_UTIL, "%s: %s", name, g_strerror(ENOENT));

    return NULL;
}

char *
_getdatafilename(const char *file, const char *func, int line, filetype_t t, char *name, GError **err)
{
    FILE *f;
    char buf[BUFSIZ];
    int i;

    for (i = 0; i < G_N_ELEMENTS(gamedir); i++) {
	g_snprintf(buf, sizeof(buf), "%s/%s/%s", gamedir[i], prefix[t], name);

	/* make sure to open in binary mode for windows */
	if ((f = fopen(buf, "rb"))) {
	    _print(0, file, func, line, "loading... \"%s\"\n", name);
	    fclose(f);
	    return strdup(buf);
	}
    }

    g_set_error(err, SCS_ERROR, SCS_ERROR_UTIL, "%s: %s", name, g_strerror(ENOENT));

    return NULL;
}

//deprecated old_file_t *
old_file_t *
old_file_new(char *name)
{
    old_file_t *file = g_new0(old_file_t, 1);
    file->name = strdup(name);
    return file;
}

//deprecated void
void
old_file_free(old_file_t *file)
{
    g_free(file->name);
    g_free(file);
}

/* retrieve a list of files in our data dir/pack of a particular type */
GList *
getdatafilelist(filetype_t t, char *subdir, gboolean recursive, GError **err)
{
    GError *tmp = NULL;
    char buf[BUFSIZ];
    int i;
    GList *list1 = NULL, *p1 = NULL;	// return from scandir
    GList *list2 = NULL, *p2 = NULL;  // new list we are building to return

    for (i = 0; i < G_N_ELEMENTS(gamedir); i++) {
	if (subdir != NULL) 
    	    g_snprintf(buf, sizeof(buf), "%s/%s/%s", gamedir[i], prefix[t], subdir);
	else
    	    g_snprintf(buf, sizeof(buf), "%s/%s", gamedir[i], prefix[t]);

	// read list of files in dir
	print("scanning: %s\n", buf);
	list1 = sys_scandir(buf, recursive, &tmp);

	if (list1 == NULL) {
	    print("could not scan dir: %s: %s\n", buf, tmp->message);
	    g_error_free(tmp);
	    tmp = NULL;
	    continue;
	}

	// merge list of files into hash
	for (p1 = list1; p1; p1 = p1->next) {
	    old_file_t *f1 = p1->data;

	    for (p2 = list2; p2; p2 = p2->next) {
		old_file_t *f2 = p2->data;

		if (strcmp(f1->name, f2->name) == 0)
		    goto next;
	    }
	    list2 = g_list_append(list2, f1);
next:
	    continue;
	}

	g_list_free(list1);
    }

    return list2;
}

FILE *
_opencfgfile(const char *file, const char *func, int line, char *name, GError **err)
{
    FILE *f;
    char *source_file;
    char dirname[BUFSIZ];
    char dirname2[BUFSIZ];
    char filename[BUFSIZ];
    char *cfgdir = sys_datadir();
    GError *tmp = NULL;

    if (! cfgdir)
	goto fail;

    g_snprintf(dirname, sizeof(dirname), "%s", cfgdir);
    g_snprintf(dirname2, sizeof(dirname2), "%s/conf", cfgdir);
    g_snprintf(filename, sizeof(filename), "%s/conf/%s", cfgdir, name);

    g_free(cfgdir);

    // try to open the file
    if ((f = fopen(filename, "rb+"))) {
	_print(0, file, func, line, "parsing... \"%s\"\n", name);
	return f;
    }

    // copy the file if it doesn't exist
    source_file = getdatafilename(FILE_CONF, name, &tmp);
    if (! source_file) {
	    goto fail;
    }

    sys_mkdir(dirname);
    sys_mkdir(dirname2);

    if (sys_copy_file(source_file, filename) != OK) {
    	g_set_error(err, SCS_ERROR, SCS_ERROR_UTIL, "cannot copy file: %s to %s", source_file, filename);
    	return NULL;
    }

    // try opening it again
    if ((f = fopen(filename, "rb+"))) {
	_print(0, file, func, line, "parsing... \"%s\"\n", name);
	return f;
    }

fail:
    g_set_error(err, SCS_ERROR, SCS_ERROR_UTIL, "%s: %s", filename, g_strerror(ENOENT));

    return NULL;
}

/*
 * XXX - need to check that fgets call succeeded & return length or error
 */
void
readline(char *buf, int size, FILE *f)
{
    do {
	fgets(buf, size, f);
    } while (buf[0] == '\n' || buf[0] == '#');
}

char *
join(int c, char **v, char *sep)
{
    char *str;
    uint32_t i, len;

    /* figure out how large of a string we need */
    for (i = 0, len = 1; i < c; i++)
	len += strlen(v[i]) + strlen(sep);

    str = g_malloc0(len);

    /* put the data in allocated string */
    for (i = 0; i < c; i++) {
	if (i > 0)
	    sprintf(str, "%s%s%s", str, sep, v[i]);
	else
	    sprintf(str, "%s", v[i]);
    }

    return str;
}

gboolean
fail(void)
{
    return FAIL;
}

gboolean
ok(void)
{
    return OK;
}
