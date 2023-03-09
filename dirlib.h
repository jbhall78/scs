#ifndef DIRLIB_H
#define DIRLIB_H

#include "filelib.h"
#include "mathlib.h"
#include "varray.h"

#ifdef WIN32
#define DIR_PSEP        '\\'
#define DIR_PSEP_S      "\\"
#define DIR_OTHER_PSEP  '/'
#else
#define DIR_PSEP        '/'
#define DIR_PSEP_S      "/"
#define DIR_OTHER_PSEP  '\\'
#endif

typedef struct directory_s directory_t;

struct directory_s {
    struct directory_s *parent;
    varray_t *files;
    varray_t *subdirs;
 //   struct stat st;   /* ../foo/bar/baz
    char *name;		/* name of directory including relative path portions (eg. bar/baz) */
    char *path;		/* relative path of directory which contains name, eg. ../foo/ */

    char *_statpath;	/* <path>/<dirname> relative path buffer used during read (eg. ../foo/bar/baz) */
			/* so we don't want to have to malloc/free this on each read */
    int _statpath_len;

    void *handle;
    void *data;
};

typedef struct {
    int (*open)(directory_t *dir, err_t **err);
    /* I'm not really happy with this API, another option is to only return file_t's with
     * and add file_type to the file_t structure, then add a conversion function to convert
     * file_t to dir_t or add files/subdirs to the file_t structure too.
     *
     * [out] type: type of object returned
     * [out] file: file_t object returned (allocated by read())
     * [out] dir:  dir_t object returned (allocated by read())
     */
    int (*read)(directory_t *dir, file_type_t *type, file_t **out_file, directory_t **out_dir, err_t **err);
    void (*close)(directory_t *dir);
} dir_driver_t;


#define DIR_OMIT_RELATIVES	(1 << 0)	/* . & .. */
#define DIR_OMIT_HIDDEN		(1 << 1)	/* .* */
#define DIR_OMIT_FILES		(1 << 2)
#define DIR_OMIT_DIRS		(1 << 3)
#define DIR_OMIT_LINKS		(1 << 4)	/* only run callback on unique files */
#define DIR_EXTENDED_INFO	(1 << 5)	/* struct stat on unix */

void dir_set_path(directory_t *dir, char *path);
//void dir_set_stat_buf(directory_t *dir, struct stat *st);
  void
  dir_func(
          directory_t *dir, boolean recursive, int flags,
          void (*dir_cb)(directory_t *dir, file_t *file, void *data1, void *data2),
          void *data1, void *data2);
void dir_scan(directory_t *dir, boolean recursive);
//varray_t *dir_read(char *dirname, int flags);
int mkdir_p(char *dir, mode_t mode);
char *dir_get_current(void);


directory_t * dir_new(char *name);
int dir_open(directory_t *dir, err_t **err);
int dir_read(directory_t *dir, file_type_t *type, file_t **out_file, directory_t **out_dir, err_t **err);
void dir_close(directory_t *dir);

#endif
