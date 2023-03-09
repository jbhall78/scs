#ifndef _SHELL_H
#define _SHELL_H

#include <stdio.h>
#include "mathlib.h"

typedef enum {
    SH_C,
    SH_LUA,
    SH_USER,
} sh_src_t;

typedef enum {
    TOK_CMD,
    TOK_VAR,
} tok_type_t;

typedef enum {
    VAR_NONE,
    VAR_NUM,
    VAR_STR,
} var_type_t;

typedef struct {
    GHashTable *tokens;
} sh_state_t;

typedef gboolean (*cmd_callback_t)(sh_state_t *st, int c, char **v, GError **e); 

typedef struct {
    /* shared stuff */
    sh_src_t src;
    tok_type_t type;
    char *name;
    char *desc;
    char *help;
    gboolean privledged;

    /* command stuff */
    cmd_callback_t cb;

    /* variable stuff */
    gboolean archive;
    gboolean monitor;

    /* XXX - might could do a union */
    var_type_t var_type;
    char *str;
    real num;
} tok_t;

sh_state_t *sh_new(void);
void sh_destroy(sh_state_t *);
gboolean sh_exec(sh_state_t *st, char *str, GError **err);
gboolean sh_exec_file(sh_state_t *st, FILE *f, GError **err);
tok_t *sh_cmd_add(sh_state_t *, char *, cmd_callback_t, GError **);
gboolean sh_var_exists(sh_state_t *, char *);
tok_t *sh_var_add(sh_state_t *st, char *name, GError **err);
tok_t *sh_var_set_str(sh_state_t *st, char *name, char *val, GError **err);
tok_t *sh_var_set_num(sh_state_t *st, char *name, real val, GError **err);
char *sh_var_get_str(sh_state_t *st, char *name);
real sh_var_get_num(sh_state_t *st, char *name);

#endif
