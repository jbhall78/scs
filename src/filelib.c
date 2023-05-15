#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>	/* errno,strerror */
#include <assert.h>

#ifdef UNIX
#include <sys/types.h>	/* open */
#include <sys/stat.h>	/* open */
#include <fcntl.h>	/* open */
#include <unistd.h>	/* write */
#endif


#include "mem.h"
#include "error.h"
#include "filelib.h"
#include "strlib.h"

#define FALSE 0
#define TRUE 1

extern file_driver_t file_posix_driver;
extern file_driver_t file_win32_driver;
extern file_driver_t file_ansi_driver;

#ifdef UNIX
static file_driver_t *drv = &file_posix_driver;
#else
# ifdef WIN32
static file_driver_t *drv = &file_win32_driver;
# else
static file_driver_t *drv = &file_ansi_driver;
# endif
#endif

void
file_set_path(file_t *file, char *path)
{
    if (file->path != NULL)
	mem_free(file->path);
    
    file->path = dup(path);
}

void
file_set_default_mode(file_t *file, int mode)
{
    file->default_mode = mode;
}

/*
void
file_set_stat_buf(file_t *file, struct stat *st)
{
    memcpy(&file->st, st, sizeof(struct stat));
}
*/

file_t *
file_new(char *name)
{
    file_t *file;
    file = mem_new0(file_t);
    file->name = dup(name);
    file->default_mode = 0644;

#if 0	/* do this on demand so we don't suffer a performance hit from find(1) style usage */
    /* also allows for file_new("/foo/bar/baz") to work correctly */
    // set directory for non relative paths
    dir = dir_get_current();
    file_set_dirname(dir);
    mem_free(dir);
#endif

    return file;
}

void
file_free(file_t *file)
{
    mem_free(file->name);
    mem_free(file);
}

//////////////////////////////////////////////////////////////////////////////

int
file_ansi_exists(char *filename)
{
    FILE *f;

    f = fopen(filename, "r");
    if (f == NULL)
	return FALSE;

    fclose(f);
    return TRUE;
}

int
file_ansi_open(file_t *file, int flags, err_t **err)
{
    FILE *f;
    char buf[4];

    memset(buf, 0, sizeof(buf));
    if (flags & FILE_READ)
	buf[len(buf)] = 'r';

    if (flags & FILE_WRITE) {
	if (flags & FILE_APPEND)
	    buf[len(buf)] = 'a';
	else
	    buf[len(buf)] = 'w';
    }

    // make sure to open in binary mode under windows
    buf[len(buf)] = 'b';

    f = fopen(file->name, buf);
    if (f == NULL) {
	error_set(err, strerror(errno));
	return FALSE;
    }

    file->handle = f;

    return TRUE;
}

int
file_ansi_read(file_t *file, void *buf, int len, err_t **err)
{
    FILE *f = file->handle;
    int ret;

    assert(file != NULL);
    assert(buf != NULL);
    assert(len > 0);

    if ((ret = fread(buf, 1, len, f)) != len) {
	if (ferror(f)) {
	    error_set(err, strerror(errno));
	}
    }

    return ret;
}

int
file_ansi_write(file_t *file, void *buf, int len, err_t **err)
{
    FILE *f = file->handle;
    int ret;

    assert(file != NULL);
    assert(buf != NULL);
    assert(len > 0);

    if ((ret = fwrite(buf, 1, len, f)) != len) {
	if (ferror(f)) {
	    error_set(err, strerror(errno));
	}
    }

    return ret;

}

void
file_ansi_close(file_t *file)
{
    FILE *f = file->handle;
    fclose(f);
}

file_driver_t file_ansi_driver = {
    .open  = file_ansi_open,
    .read  = file_ansi_read,
    .write = file_ansi_write,
    .close = file_ansi_close,
};

//////////////////////////////////////////////////////////////////////////////

#ifdef UNIX

int
file_posix_open(file_t *file, int flags, err_t **err)
{
    int opts = 0;
    int fd, *fdp;

    if (flags & FILE_READ && flags & FILE_WRITE)
	opts = O_RDWR;
    else if (flags & FILE_READ)
	opts = O_RDONLY;
    else if (flags & FILE_WRITE) {
	opts = O_WRONLY;
	if (flags & FILE_CREATE)
	    opts |= O_CREAT;
    }

    if (flags & FILE_CREATE)
	fd = open(file->name, opts, file->default_mode);
    else
	fd = open(file->name, opts);

    if (fd < 0) {
	error_set(err, strerror(errno));
	return FALSE;
    }


    fdp = mem_new0(int);
    file->handle = fdp;

    return TRUE;
}

int
file_posix_read(file_t *file, void *buf, int len, err_t **err)
{
    int *fdp = file->handle;
    int ret;

    assert(file != NULL);
    assert(buf != NULL);
    assert(len > 0);

    if ((ret = read(*fdp, buf, len)) < 0) {
	error_set(err, strerror(errno));
	return ret;
    }

    return ret;
}

int
file_posix_write(file_t *file, void *buf, int len, err_t **err)
{
    int *fdp = file->handle;
    int ret;

    assert(file != NULL);
    assert(buf != NULL);
    assert(len > 0);

    if ((ret = write(*fdp, buf, len)) < 0) {
	error_set(err, strerror(errno));
	return ret;
    }

    return ret;
}

void
file_posix_close(file_t *file)
{
    int *fdp = file->handle;
    close(*fdp);
    mem_free(fdp);
}

file_driver_t file_posix_driver = {
    .open  = file_posix_open,
    .read  = file_posix_read,
    .write = file_posix_write,
    .close = file_posix_close,
};

#endif

//////////////////////////////////////////////////////////////////////////////

#ifdef WIN32
#include <windows.h>

int
file_win32_open(file_t *file, int flags, err_t **err)
{
    HANDLE fh;
    DWORD access = 0;
    DWORD create = 0;
    
    if (flags & FILE_APPEND) {
	access = FILE_APPEND_DATA;
    } else {
	if (flags & FILE_READ)
	    access |= GENERIC_READ;
	if (flags & FILE_WRITE)
	    access |= GENERIC_WRITE;
    }

    if (flags & FILE_CREATE)
	create = OPEN_ALWAYS;
    else
	create = OPEN_EXISTING;

/*
HANDLE WINAPI CreateFile(
  _In_      LPCTSTR lpFileName,
  _In_      DWORD dwDesiredAccess,
  _In_      DWORD dwShareMode,
  _In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
  _In_      DWORD dwCreationDisposition,
  _In_      DWORD dwFlagsAndAttributes,
  _In_opt_  HANDLE hTemplateFile
*/

    fh = CreateFile(file->name, access, FILE_SHARE_READ, NULL, create, FILE_ATTRIBUTE_NORMAL, NULL);
    if (fh == INVALID_HANDLE_VALUE) {
	char *msg = error_win32_strerror();
	error_set(err, "cannot open file %s: %s", file->name, msg);
	free(msg);
	return FALSE;
    }

    file->handle = fh;

    return TRUE;
}

int
file_win32_read(file_t *file, void *buf, int len, err_t **err)
{
    HANDLE fh = file->handle;

    assert(file != NULL);
    assert(buf != NULL);
    assert(len > 0);

    if (ReadFile(fh, buf, len, NULL, NULL) != TRUE) {
	char *msg = error_win32_strerror();
	error_set(err, "cannot read from file: %s: %s", file->name, msg);
	free(msg);
	return -1;
    }

    return len;
}

void
file_win32_write(file_t *file, void *buf, int len, err_t **err)
{
    HANDLE fh = file->handle;

    assert(file != NULL);
    assert(buf != NULL);
    assert(len > 0);

    if (WriteFile(fh, buf, len, NULL, NULL) != TRUE) {
	char *msg = error_win32_strerror();
	error_set(err, "cannot write to file: %s: %s", file->name, msg);
	free(msg);
	return -1;
    }

    return len;
}

void
file_win32_close(file_t *file)
{
    HANDLE fh = file->handle;
    CloseHandle(fh);
}

file_driver_t file_win32_driver = {
    .open  = file_win32_open,
    .read  = file_win32_read,
    .write = file_win32_write,
    .close = file_win32_close,
};

#endif

//////////////////////////////////////////////////////////////////////////////

int
file_open(file_t *file, int flags, err_t **err)
{
    return drv->open(file, flags, err);
}

int
file_read(file_t *file, void *buf, int len, err_t **err)
{
    return drv->read(file, buf, len, err);
}

int
file_write(file_t *file, void *buf, int len, err_t **err)
{
    return drv->write(file, buf, len, err);
}

void
file_close(file_t *file)
{
    drv->close(file);
}

int
file_read_full(char *name, char **data, int *len, err_t **err)
{
    int ret;
    string_t *str;
    char buf[BUFSIZ];
    file_t *file;
    err_t *tmp = NULL;

    file = file_new(name);
    if (file_open(file, FILE_READ, &tmp) != TRUE) {
	error_propagate(err, tmp);
	return FALSE;
    }

    str = str_new(NULL, 0);
    while ((ret = file_read(file, buf, BUFSIZ, &tmp)) > 0) {
	str_append_str(str, buf, ret);
    }
    if (tmp != NULL) {
	error_propagate(err, tmp);
	return FALSE;
    }

    file_close(file);
    file_free(file);

    if (data != NULL)
	*data = dup(str->txt);
    if (len != NULL)
	*len = str->len;

    return TRUE;
}

int
file_write_full(char *name, int mode, char *buf, int len, err_t **err)
{
    file_t *file;
    err_t *tmp = NULL;

    file = file_new(name);
    file_set_default_mode(file, mode);
    if (file_open(file, FILE_CREATE|FILE_WRITE, &tmp) != TRUE) {
	error_propagate(err, tmp);
	return FALSE;
    }

    if (len != 0) {
	if (file_write(file, buf, len, &tmp) < 0) {
	    error_propagate(err, tmp);
	    return FALSE;
	}
    }

    file_close(file);
    file_free(file);

    return TRUE;
}


#ifdef FILE_MAIN
int
main(int argc, char **argv)
{
    char *name1 = "hello1.txt";
    char *name2 = "hello2.txt";
    char *name3 = "nonexistant.txt";
    char *txt = "Hello World!\n";
    file_t *file;
    char buf[BUFSIZ];
    int len;
    char *str;
    err_t *tmp = NULL;

    file = file_new(name1);
    if (file_open(file, FILE_CREATE|FILE_WRITE, &tmp) != TRUE) {
	fprintf(stderr, "could not create/open file for writing: %s: %s\n", name1, tmp->msg);
	exit(1);
    }

    file_write(file, txt, strlen(txt), NULL);
    file_close(file);
    file_free(file);

    file = file_new(name1);
    if (file_open(file, FILE_READ, &tmp) != TRUE) {
	fprintf(stderr, "could not open file for reading: %s: %s\n", name1, tmp->msg);
	exit(1);
    }

    file_read(file, buf, BUFSIZ, NULL);
    printf("%s\n", buf);
    file_close(file);
    file_free(file);

    if (file_write_full(name2, 0644, txt, strlen(txt), &tmp) != TRUE) {
	fprintf(stderr, "could not create/open file for writing: %s: %s\n", name2, tmp->msg);
	exit(1);
    }

    if (file_read_full(name2, &str, &len, &tmp) != TRUE) {
	fprintf(stderr, "could not open file for reading: %s: %s\n", name2, tmp->msg);
	exit(1);
    }
    printf("[%d] %s\n", len, str);
    mem_free(str);

    file = file_new(name3);
    if (file_open(file, FILE_READ, &tmp) != TRUE) {
	fprintf(stderr, "could not open file for reading: %s: %s\n", name3, tmp->msg);
    }
    file_free(file);

    return 0;
}
#endif
