#ifndef FILELIB_H
#define FILELIB_H

#include "mathlib.h"
#include "error.h"

typedef enum {
    FILETYPE_FILE,
    FILETYPE_DIR,
    FILETYPE_SYMLINK
} file_type_t;

typedef struct {
    struct directory_s *parent;
    char *name;		// eg. foo	for certain operations, /somewhere/foo is valid too
    char *path;		// eg. /somewhere

    // unix stuff
    int default_mode;	// permissions assigned to file at create time

    // FILE pointer, fd, or file HANDLE
    void *handle;

    // additional data
    void *data;
} file_t;

typedef struct {
    int (*open)(file_t *file, int flags, err_t **err);
    int (*read)(file_t *file, void *buf, int len, err_t **err);
    int (*write)(file_t *file, void *buf, int len, err_t **err);
    void (*close)(file_t *file);
    int (*exists)(char *filename);
    // TODO: seek
} file_driver_t;

// file flags
#define FILE_READ	(1 << 0)
#define FILE_WRITE	(1 << 1)
#define FILE_CREATE	(1 << 2)
#define FILE_APPEND     (1 << 3)

// 
file_t *file_new(char *filename);
void file_free(file_t *file);

// functions which manipulate file properties
void file_set_path(file_t *file, char *path);
void file_set_default_mode(file_t *file, int mode);
//void file_set_stat_buf(file_t *file, struct stat *st);

// convenience functions
int file_read_full(char *filename, char **data, int *len, err_t **err);
int file_write_full(char *filename, int mode, char *buf, int len, err_t **err);

// driver functions
int file_open(file_t *file, int flags, err_t **err);
int file_read(file_t *file, void *buf, int len, err_t **err);
int file_write(file_t *file, void *buf, int len, err_t **err);
void file_close(file_t *file);

// library initialization
void file_init(void);

#endif
