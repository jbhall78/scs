#ifndef _SHD_H
#define _SHD_H

#include <stdio.h>
#include <glib.h>

#include "scs.h"

#if (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR >= 1))
#define deprecated	__attribute__((__deprecated__))
#else
#define deprecated
#endif

#ifdef __SUNPRO_C
# define FUNCTION __func__
#else
# define FUNCTION __FUNCTION__
#endif

#define print(...) \
		_print(0,__FILE__,FUNCTION,__LINE__,__VA_ARGS__)
#define printerr(...) \
		_print(1,__FILE__,FUNCTION,__LINE__,__VA_ARGS__)
#define opendatafile(t,f,e) \
		_opendatafile(__FILE__,FUNCTION,__LINE__,t,f,e)
#define getdatafilename(t,f,e) \
		_getdatafilename(__FILE__,FUNCTION,__LINE__,t,f,e)
#define opencfgfile(f,e) \
		_opencfgfile(__FILE__,FUNCTION,__LINE__,f,e)

gboolean fail(void);
gboolean ok(void);
#define CALL(x) (x == NULL) ? ok() : x


typedef struct old_file_s {
    char *name;
    uint64_t size;
} old_file_t;

extern GQuark SCS_ERROR;

int sv_launch(gboolean dedicated, game_type_t type);
gboolean sv_shutdown(GError **err);
int cl_launch(char *, gboolean, uint16_t, uint16_t);

void _print(int, const char *, const char *, int, char *, ...);
FILE *_opendatafile(const char *, const char *, int, filetype_t type, char *filename, GError **);
char *_getdatafilename(const char *, const char *, int, filetype_t type, char *filename, GError **);
FILE *_opencfgfile(const char *, const char *, int, char *filename, GError **);
GList *getdatafilelist(filetype_t t, char *subdir, gboolean recursive, GError **err);
void readline(char *buf, int size, FILE *f);
char *join(int, char **, char *);
char *mkstr(char *fmt, ...);


deprecated old_file_t *old_file_new(char *filename);
deprecated void old_file_free(old_file_t *file);

void sys_print(int, char *);
void sys_init(void);
void sys_shutdown(void);
char *sys_datadir(void);
GList *sys_scandir(char *dir, gboolean recursive, GError **err);

#ifndef DEDICATED
#define is_client() g_thread_self() == client.thread
#else
#define is_client() FALSE
#endif


#endif
