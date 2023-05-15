#include <glib.h>
#include <errno.h>
#include <string.h>

#include "scs.h"
#include "shared.h"
#include "shell.h"

gboolean
CMD_echo(sh_state_t *st, int argc, char **argv, GError **err)
{
    char *str;

    str = join(argc-1, argv+1, " ");

    print("%s\n", str);

    g_free(str);

    return OK;
}

tok_t *
sh_cmd_add(sh_state_t *st, char *name, cmd_callback_t cb, GError **err)
{
    tok_t *tok;

    if (g_hash_table_lookup(st->tokens, name) != NULL) {
	g_set_error(err, SCS_ERROR, SCS_ERROR_SH, "token exists");
	return NULL;
    }

    tok = g_new0(tok_t, 1);

    tok->src = SH_C;
    tok->type = TOK_CMD;
    tok->name = strdup(name);
    tok->privledged = FALSE;
    tok->cb = cb;

    g_hash_table_insert(st->tokens, strdup(name), tok);

    return tok;
}

gboolean
sh_var_exists(sh_state_t *st, char *name)
{
    tok_t *tok;

    if ((tok = g_hash_table_lookup(st->tokens, name)) == NULL ||
	    tok->type != TOK_VAR)
	return FAIL;

    return OK;
}

tok_t *
sh_var_add(sh_state_t *st, char *name, GError **err)
{
    tok_t *tok;

    if (g_hash_table_lookup(st->tokens, name) != NULL) {
	g_set_error(err, SCS_ERROR, SCS_ERROR_SH, "token exists");
	return NULL;
    }

    tok = g_new0(tok_t, 1);

    tok->src = SH_C;
    tok->type = TOK_VAR;
    tok->name = strdup(name);
    tok->privledged = FALSE;
    tok->var_type = VAR_NONE;

    g_hash_table_insert(st->tokens, strdup(name), tok);

    return tok;
}

tok_t *
sh_var_set_num(sh_state_t *st, char *name, real val, GError **err)
{
    tok_t *tok;

    tok = g_hash_table_lookup(st->tokens, name);
    if (! tok) {
	g_set_error(err, SCS_ERROR, SCS_ERROR_SH, "can't find token");
	return NULL;
    }
    if (tok->type != TOK_VAR) {
	g_set_error(err, SCS_ERROR, SCS_ERROR_SH, "%s is not a variable", name);
	return NULL;
    }

    if (tok->str != NULL)
	g_free(tok->str);
    tok->num = val;
    tok->var_type = VAR_NUM;

    return tok;
}

char *
sh_var_get_str(sh_state_t *st, char *name)
{
    tok_t *tok;

    tok = g_hash_table_lookup(st->tokens, name);
    if (! tok || tok->type != TOK_VAR)
	return NULL;

    return strdup(tok->str);
}

real
sh_var_get_num(sh_state_t *st, char *name)
{
    tok_t *tok;

    tok = g_hash_table_lookup(st->tokens, name);
    if (! tok || tok->type != TOK_VAR)
	return 0;

    return tok->num;
}


tok_t *
sh_var_set_str(sh_state_t *st, char *name, char *val, GError **err)
{
    tok_t *tok;

    tok = g_hash_table_lookup(st->tokens, name);
    if (! tok) {
	g_set_error(err, SCS_ERROR, SCS_ERROR_SH, "can't find token");
	return NULL;
    }

    if (tok->str != NULL)
	g_free(tok->str);
    tok->str = strdup(val);
    tok->var_type = VAR_STR;

    return tok;
}

gboolean
CMD_show(sh_state_t *st, int argc, char **argv, GError **err)
{
    tok_t *tok;

    if (argc != 2) {
	g_set_error(err, SCS_ERROR, SCS_ERROR_SH,
		"usage: %s <variable>\n", argv[0]);
	return FAIL;
    }

    tok = g_hash_table_lookup(st->tokens, argv[1]);
    if (! tok) {
	g_set_error(err, SCS_ERROR, SCS_ERROR_SH,
		"%s does not exist\n", argv[1]);
	return FAIL;
    }
    if (tok->type != TOK_VAR) {
	g_set_error(err, SCS_ERROR, SCS_ERROR_SH,
		"%s is not a variable\n", argv[1]);
	return FAIL;
    }

    if (tok->var_type == VAR_STR)
	print("%s", tok->str);
    else if (tok->var_type == VAR_NUM)
	print("%f", tok->num);
    else {
	g_set_error(err, SCS_ERROR, SCS_ERROR_SH,
		"%s is invalid\n", argv[1]);
	return FAIL;
    }
	
    return OK;
}
gboolean
CMD_set(sh_state_t *st, int argc, char **argv, GError **err)
{
    GError *tmp = NULL;
    real val;
    tok_t *tok;
    uint16_t i, len;

    if (argc != 3) { 
	g_set_error(err, SCS_ERROR, SCS_ERROR_SH,
		"usage: %s <variable> <value>\n", argv[0]);
	return FAIL;
    }

    tok = g_hash_table_lookup(st->tokens, argv[1]);
    if (! tok) {
	if (! (tok = sh_var_add(st, argv[1], &tmp)))
	    goto fail;
    }

    /* figure out if this is a string or a number */
    len = strlen(argv[2]);
    for (i = 0; i < len; i++) {
	if (! g_ascii_isdigit(argv[2][i]) && argv[2][i] != '.')
	   break; 
    }	

    if (i == len) {
	/* we exhausted the search so we are a number */
	val = g_ascii_strtod(argv[2], NULL);
	tok = sh_var_set_num(st, argv[1], val, &tmp);
    } else
	tok = sh_var_set_str(st, argv[1], argv[2], &tmp);

    if (! tok)
       goto fail;
	
    return OK;

fail:
    g_propagate_error(err, tmp);
    return FAIL;
}

gboolean
sh_exec(sh_state_t *st, char *str, GError **err)
{
    GError *tmp = NULL;
    int argc;
    char **argv;
    tok_t *tok;

    /* split our string up and obey quotes */
    if (! g_shell_parse_argv(str, &argc, &argv, &tmp)) {
	g_propagate_error(err, tmp);
	return FAIL;
    }

    /* make sure we have a command line */
    if (argc <= 0) {
	g_set_error(err, SCS_ERROR, SCS_ERROR_SH, "invalid cmdline");
	return FAIL;
    }

    /* find the specified command */
    if ((tok = g_hash_table_lookup(st->tokens, argv[0])) == NULL) {
	g_set_error(err, SCS_ERROR, SCS_ERROR_SH, "bad command or variable: %s", argv[0]);
	g_strfreev(argv);
	return FAIL;
    }

    /* exec the command or set the variable */
    if (tok->type == TOK_CMD) {
	if (tok->cb != NULL) {
	    if (! tok->cb(st, argc, argv, &tmp)) {
		g_strfreev(argv);
		g_propagate_error(err, tmp);
		return FAIL;
	    }
	}
    } else if (tok->type == TOK_VAR) {
	/* TODO: add variable setting */
    }

    g_strfreev(argv);

    return OK;
}

gboolean
sh_exec_file(sh_state_t *st, FILE *f, GError **err)
{
    GError *tmp = NULL;
    char buf[BUFSIZ];
    uint32_t line = 0;

    fseek(f, 0, SEEK_SET);	/* rewind */

    while (fgets(buf, sizeof(buf), f) != NULL) {
	line++;

	if (! sh_exec(st, buf, &tmp)) {
	    g_set_error(err, SCS_ERROR, SCS_ERROR_SH,
		    "parse error on line %d: %s", line, tmp->message);
	    g_error_free(tmp);
	    return FAIL;
	}
    }

    return OK;
}

static void
sh_destroy_key(gpointer key)
{
    g_free(key);
}

static void
sh_destroy_val(gpointer val)
{
    tok_t *tok = val;

    if (tok->name)
	g_free(tok->name);
    if (tok->desc)
	g_free(tok->desc);
    if (tok->help)
	g_free(tok->help);

    if (tok->var_type == VAR_STR && tok->str)
	g_free(tok->str);

    g_free(tok);
}

sh_state_t *
sh_new(void)
{
    sh_state_t *st = g_new0(sh_state_t, 1);

    st->tokens = g_hash_table_new_full(g_str_hash, g_str_equal,
	    sh_destroy_key, sh_destroy_val);

    /* add default commands */
    sh_cmd_add(st, "echo", CMD_echo, NULL);
    sh_cmd_add(st, "set", CMD_set, NULL);
    sh_cmd_add(st, "show", CMD_show, NULL);

    /* add default variables */

    return st;
}

static gboolean
sh_destroy_all(gpointer key, gpointer val, gpointer data)
{
    return OK;
}

void
sh_destroy(sh_state_t *st)
{
    g_hash_table_foreach_remove(st->tokens, sh_destroy_all, NULL);

    g_hash_table_destroy(st->tokens);

    g_free(st);
}
