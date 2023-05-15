#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include <assert.h>

#ifdef UNIX
#include <unistd.h>	/* pathconf */
#include <sys/types.h>	/* opendir,stat,mkdir */
#include <sys/stat.h>	/* stat,mkdir */
#include <dirent.h>	/* opendir */
#include <errno.h>	/* errno,strerror */
#endif

#include "mem.h"
#include "mathlib.h"
#include "varray.h"
#include "strlib.h"
#include "filelib.h"
#include "dirlib.h"

#define TRUE 1
#define FALSE 0

extern dir_driver_t dir_win32_driver;
extern dir_driver_t dir_posix_driver;

#ifdef WIN32
static dir_driver_t *drv = &dir_win32_driver;
#else
static dir_driver_t *drv = &dir_posix_driver;
#endif


void
dir_set_path(directory_t *dir, char *path)
{
    if (dir->path != NULL)
	mem_free(dir->path);

    dir->path = dup(path);
}

/* read contents of directory into array */
#if 0
varray_t *
dir_read(char *name, int flags)
{
    DIR *dir;
    varray_t *va;
    struct dirent *entry, *result;
    int ret, len;

    // correctly allocate the struct dirent.
    // the size of the d_name field hasn't
    // been defined until recently.
    // see man readdir_r(3) for more info.
    len = offsetof(struct dirent, d_name) + pathconf(name, _PC_NAME_MAX) + 1;
    entry = malloc(len);

    if ((dir = opendir(name)) == NULL) {
	fprintf(stderr, "could not open directory: %s: %s\n", name, strerror(errno));
	return NULL;
    }

    va = varray_new(NULL, 0, &mem_free);

    while ((ret = readdir_r(dir, entry, &result)) == 0) {
	if (result == NULL)	// end of directory entries
	    break;

    	varray_append(va, dup(result->d_name));
    }

    if (va->len > 0)
	return va;

    varray_free(va);

    return NULL;
}
#endif

void
dir_func(
	directory_t *dir, boolean recursive, int flags,
	void (*dir_cb)(directory_t *dir, file_t *file, void *data1, void *data2),
	void *data1, void *data2)
{
    int i;
    directory_t *d;
    file_t *f;

    if (dir_cb == NULL)
	return;

    // subdirs
    for (i = 0; i < dir->subdirs->len; i++) {
	d = dir->subdirs->data[i];
	if (recursive) {
	    dir_func(d, recursive, flags, dir_cb, data1, data2);
	}
	//dir_cb(d, NULL, data1, data2);
    }

    // files
    for (i = 0; i < dir->files->len; i++) {
	f = dir->files->data[i];
	dir_cb(NULL, f, data1, data2);
    }

    // ourself
    dir_cb(dir, NULL, data1, data2);
}

static char *
_dir_build_path(varray_t *lineage, char *filename)
{
    string_t *str;
    char *s;
    int i;

    str = str_new(NULL, 0);
    for (i = 0; i < lineage->len; i++) {
	char *dir = lineage->data[i];
	if (i != 0)
	    str_append_char(str, DIR_PSEP);
	str_append_str(str, dir, len(dir));
    }
    if (filename != NULL) {
	str_append_char(str, DIR_PSEP);
	str_append_str(str, filename, len(filename));
    }

    s = dup(str->txt);
    str_free(str);

    return s;
}

/* free the current directory (to be called recursively) */
void
_dir_free(directory_t *dir)
{
    mem_free(dir->name);
    varray_free(dir->files);
    varray_free(dir->subdirs);
    mem_free(dir);
}

directory_t *
dir_new(char *name)
{
    directory_t *dir;

    dir = mem_new0(directory_t);
    dir->name = dup(name);
    dir->files = varray_new(NULL, 0, &file_free);
    dir->subdirs = varray_new(NULL, 0, &_dir_free);

    return dir;
}

static void
_dir_scan(varray_t *lineage, directory_t *dir, boolean recursive)
{
    char *path;
    directory_t *out_dir, *tmp_dir;
    file_t *out_file;
    file_type_t type;
    int i;
    err_t *tmp = NULL;

    varray_push(lineage, dir->name);
    path = _dir_build_path(lineage, NULL);

    if (dir_open(dir, &tmp) != TRUE) {
	fprintf(stderr, "could not open directory: %s: %s\n", dir->name, tmp->msg);
	error_free(tmp);
	varray_pop(lineage);
	mem_free(path);
	return;
    }

    // build files & subdirs arrays
    while (dir_read(dir, &type, &out_file, &out_dir, &tmp) == TRUE) {
	if (type == FILETYPE_DIR) {
	    printf("appending: %s\n", out_dir->name);
	    varray_append(dir->subdirs, out_dir);
	} else {
	    varray_append(dir->files, out_file);
	}
    }

    dir_close(dir);

    mem_free(path);

    // recurse into subdirs
    if (recursive) {
	for (i = 0; i < dir->subdirs->len; i++) {
	    _dir_scan(lineage, (directory_t *)dir->subdirs->data[i], recursive);
	}
    }

    varray_pop(lineage);

    return;
}

void
dir_scan(directory_t *dir, boolean recursive)
{
    varray_t *va = varray_new(NULL, 0, NULL);
    _dir_scan(va, dir, recursive);
}

/* equivilant of mkdir -p */
int
mkdir_p(char *dir, mode_t mode)
{
    char **parts;
    int i, n;
    string_t *str;
    struct stat st;
    int ret;

    parts = split(dir, "/", 0, &n);
    if (! (n > 0))
	return -1;	// BUG: set errno with something correct

    str = str_new(NULL, 0);
    for (i = 0; i < n; i++) {
	str_append_char(str, '/');
	str_append_str(str, parts[i], len(parts[i]));

	// check to see if directory exists
	if ((ret = stat(str->txt, &st)) != 0) {
	    // if it does not exist, create it
	    if ((ret = mkdir(str->txt, mode)) != 0) {
		return ret;
	    }
	}
    }

    return 0;
}

char *
dir_get_current(void)
{
    string_t *str;
    int size = 0;
    char *s;

    str = str_new(NULL, 0);
    while (1) {
	size += 1000;
	str_alloc(str, size);
	if (getcwd(str->txt, size) == NULL) {
	    fprintf(stderr, "error getting current working directory: %s\n", strerror(errno));
	}
	if (errno != ERANGE) {
	    break;
	}
    }
    s = dup(str->txt);
    str_free(str);
    return s;
}

//////////////////////////////////////////////////////////////////////////////

int
dir_posix_open(directory_t *dir, err_t **err)
{
    DIR *d;

    if ((d = opendir(dir->name)) == NULL) {
	error_set(err, strerror(errno));
	return FALSE;
    }

    // correctly allocate the struct dirent.
    // the size of the d_name field hasn't
    // been defined until recently.
    // see man readdir_r(3) for more info.
    int path_max = pathconf(dir->name, _PC_NAME_MAX);
    int len = offsetof(struct dirent, d_name) + path_max + 1;

    dir->handle = d;
    dir->_statpath_len = len(dir->name) + path_max + 2;
    dir->_statpath = mem_alloc(dir->_statpath_len);
    dir->data = mem_alloc(len);

    return TRUE;
}

int
dir_posix_read(directory_t *dir, file_type_t *type, file_t **out_file, directory_t **out_dir, err_t **err)
{
    DIR *d = dir->handle;
    struct dirent *entry = dir->data;
    struct dirent *result;
    struct stat st;
    directory_t *new_dir;
    file_t *new_file;

next:
    if (readdir_r(d, entry, &result) != 0) {
	error_set(err, strerror(errno));
	return FALSE;
    }

    if (result == NULL)	// end of directory entries
	return FALSE;

    // filter these out here so we don't have to worry about them in the higher layers
    if (eq(result->d_name, ".") || eq(result->d_name, ".."))
	goto next;

    snprintf(dir->_statpath, dir->_statpath_len, "%s" DIR_PSEP_S "%s", dir->name, result->d_name);

    if (stat(dir->_statpath, &st) < 0) {
	error_set(err, strerror(errno));
	return FALSE;
    }

    if (S_ISDIR(st.st_mode)) {
	new_dir = dir_new(dir->_statpath);
	new_dir->parent = dir;
	dir_set_path(new_dir, dir->name);
	if (out_dir != NULL)
	    *out_dir = new_dir;
	*type = FILETYPE_DIR;
    } else {
	new_file = file_new(dir->_statpath);
	new_file->parent = dir;
	file_set_path(new_file, dir->name);
	if (out_file != NULL)
	    *out_file = new_file;
	*type = FILETYPE_FILE;
    }

    return TRUE;
}

void
dir_posix_close(directory_t *dir)
{
    DIR *d = dir->handle;

    closedir(d);

    mem_free(dir->_statpath);
    mem_free(dir->data);
}

dir_driver_t dir_posix_driver = {
    .open  = dir_posix_open,
    .read  = dir_posix_read,
    .close = dir_posix_close,
};

//////////////////////////////////////////////////////////////////////////////

int
dir_open(directory_t *dir, err_t **err)
{
    return drv->open(dir, err);
}

int
dir_read(directory_t *dir, file_type_t *type, file_t **out_file, directory_t **out_dir, err_t **err)
{
    return drv->read(dir, type, out_file, out_dir, err);
}

void
dir_close(directory_t *dir)
{
    drv->close(dir);
}

#ifdef DIR_MAIN

void
dir_print(directory_t *dir, file_t *file, void *data1, void *data2)
{
    if (dir != NULL) {
	printf("directory: [%s] -> [%s]\n", dir->path, dir->name);
    }
    if (file != NULL) {
	printf("file: [%s] -> [%s]\n", file->path, file->name);
    }
}

int
main(int argc, char **argv)
{
#if 0
    varray_t *va;
    int i;

    va = dir_read(".", 0);

    for (i = 0; i < va->len; i++) {
	printf(">> '%s'\n", (char *)va->data[i]);
    }
#endif
    directory_t *dir;

    dir = dir_new(".");

    dir_scan(dir, TRUE);

    dir_func(dir, TRUE, 0, dir_print, NULL, NULL);

    return 0;
}
#endif
