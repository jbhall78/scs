#include <glib.h>

#include <SDL2/SDL.h>

#include "scs.h"
#include "shared.h"
#include "client.h"

static cl_bind_map_t cl_bind_map[] = {
#include "cl_bind_map.h"
};

uint32_t
bind_id_from_str(char *str)
{
    uint32_t i;

    for (i = 0; i < G_N_ELEMENTS(cl_bind_map); i++) {
	if (g_strcasecmp(str, cl_bind_map[i].str) == 0) {
	    return cl_bind_map[i].id;
	}
    }

    return FAIL;
}

int8_t 
bind_mods_from_str(char *str)
{
    char **v;
    uint32_t i;
    int8_t mods = 0;

    v = g_strsplit(str, "|", -1);

    for (i = 0; v[i] != NULL; i++) {
	if (g_strcasecmp(v[i], "NONE") == 0)
	    ;
	else if (g_strcasecmp(v[i], "SHIFT") == 0)
	    mods |= BIND_MOD_SHIFT;
	else if (g_strcasecmp(v[i], "CTRL") == 0)
	    mods |= BIND_MOD_CTRL;
	else if (g_strcasecmp(v[i], "ALT") == 0)
	    mods |= BIND_MOD_ALT;
	else
	    ;	/* XXX - error */
    }

    g_strfreev(v);

    return mods;
}

bind_state_t 
bind_state_from_str(char *str)
{
    if (g_strcasecmp(str, "UP") == 0)
	return BIND_UP;
    else if (g_strcasecmp(str, "DOWN") == 0)
	return BIND_DOWN;
    else if (g_strcasecmp(str, "UPDOWN") == 0)
	return BIND_UPDOWN;
    else if (g_strcasecmp(str, "UPDATE") == 0)
	return BIND_UPDATE;
    else
	return BIND_NONE;
}

gboolean
CMD_bind(sh_state_t *st, int argc, char **argv, GError **err)
{
    uint32_t *id;
    binding_t *bind;

    if (argc != 5) {
	g_set_error(err, SCS_ERROR, SCS_ERROR_SH,
		"usage: %s <key> <modifiers> <state> <action>", argv[0]);
	return FAIL;
    }

    /* allocate memory */
    bind            = g_new0(binding_t, 1);
    id              = g_new(uint32_t, 1);

    /* fill in data */
    *id             = bind_id_from_str(argv[1]);
    bind->modifiers = bind_mods_from_str(argv[2]);
    bind->state     = bind_state_from_str(argv[3]);
    bind->action    = strdup(argv[4]);

    /* TODO - precompile bind->action to see if it looks valid */
    if (! *id || ! bind->state) {
	g_set_error(err, SCS_ERROR, SCS_ERROR_SH, "invalid binding!");
	g_free(bind->action);
	g_free(id);
	g_free(bind);
	
	return FAIL;
    }

    /* add or replace binding */
    g_hash_table_replace(client.bindings, id, bind);

    return OK;
}

static void
bind_destroy_key(gpointer key)
{
    g_free(key);
}

static void
bind_destroy_val(gpointer val)
{
    binding_t *bind = val;

    if (bind->action) 
	g_free(bind->action);

    g_free(val);
}

gboolean
bind_init(GError **err)
{
    GError *tmp = NULL;

    client.bindings = g_hash_table_new_full(g_int_hash, g_int_equal,
	    bind_destroy_key, bind_destroy_val);

    if (! sh_cmd_add(client.shell, "bind", CMD_bind, &tmp)) {
	g_propagate_error(err, tmp);
	return FAIL;
    }

    return OK;
}

static gboolean
bind_destroy_all(gpointer key, gpointer val, gpointer data)
{
    return OK;
}
    
gboolean
bind_shutdown(GError **err)
{
    g_hash_table_foreach_remove(client.bindings, bind_destroy_all, NULL);

    return OK;
}

#if 0
void
bind_do_update(gpointer key, gpointer val, gpointer data)
{
    int32_t *key_id = key;
    binding_t *bind = val;
    const uint8_t *keys;
    int modifiers;
    int sdl_modifiers;
    GError *err = NULL;

    keys = SDL_GetKeyboardState(NULL);
    sdl_modifiers = SDL_GetModState();

    if (*key_id > SDLK_LAST)
	return;

    if (! keys[*key_id]) 
	return;

    if (bind->state != BIND_UPDATE)
	return;
	    
    // check modifiers
    modifiers = 0;
    if (sdl_modifiers & KMOD_CTRL) 
	modifiers |= BIND_MOD_CTRL;
    if (sdl_modifiers & KMOD_ALT)
	modifiers |= BIND_MOD_ALT;
    if (sdl_modifiers & KMOD_SHIFT) 
	modifiers |= BIND_MOD_SHIFT;

    if (modifiers != bind->modifiers)
	return;

    
    if (! sh_exec(client.shell, bind->action, &err))
	print("binding error: %s\n", err->message);
}
#endif

void
bind_do_update(gpointer key, gpointer val, gpointer data)
{
    int32_t *key_id = key; // This is an SDL_Keycode
    binding_t *bind = val;
    const uint8_t *keys;
    int modifiers;
    int sdl_modifiers;
    GError *err = NULL;

    // Get the keyboard state array. It's indexed by SDL_Scancode.
    keys = SDL_GetKeyboardState(NULL);
    sdl_modifiers = SDL_GetModState();

    // --- CRITICAL CHANGE HERE ---
    // Convert the SDL_Keycode (*key_id) to an SDL_Scancode
    SDL_Scancode scancode = SDL_GetScancodeFromKey(*key_id);

    // If SDL_GetScancodeFromKey returns SDL_SCANCODE_UNKNOWN, it means
    // the SDL_Keycode provided doesn't map to a known scancode.
    // This can happen for things that aren't typical physical keys, or invalid values.
    if (scancode == SDL_SCANCODE_UNKNOWN) {
        // Optionally print a warning or handle this case
        // fprintf(stderr, "Warning: Invalid SDL_Keycode %d encountered in bind_do_update\n", *key_id);
        return; // Skip if the key is unknown
    }

    // Now, use the scancode to index the 'keys' array
    // This check is valid if 'scancode' is a proper index.
    if (! keys[scancode])
        return; // Key is not pressed

    // Check if the binding is in the update state
    if (bind->state != BIND_UPDATE)
        return;

    // check modifiers
    modifiers = 0;
    if (sdl_modifiers & KMOD_CTRL)
        modifiers |= BIND_MOD_CTRL;
    if (sdl_modifiers & KMOD_ALT)
        modifiers |= BIND_MOD_ALT;
    if (sdl_modifiers & KMOD_SHIFT)
        modifiers |= BIND_MOD_SHIFT;

    if (modifiers != bind->modifiers)
        return;

    // Execute the bound action
    if (! sh_exec(client.shell, bind->action, &err)) {
        print("binding error: %s\n", err->message);
        if (err) { // Always free GError if it was set
            g_error_free(err);
        }
    }
}

void
bind_update(void)
{
    g_hash_table_foreach(client.bindings, bind_do_update, NULL);
}

/**
 * Process a keyboard event against the default bindings table
 *
 * @param SDL's keyboard event structure
 */
void
bind_key(SDL_KeyboardEvent *k)
{
    binding_t *bind;
    int modifiers;
    GError *err = NULL;
    
    modifiers = 0;
    if (k->keysym.mod & KMOD_CTRL) 
	modifiers |= BIND_MOD_CTRL;
    if (k->keysym.mod & KMOD_ALT)
	modifiers |= BIND_MOD_ALT;
    if (k->keysym.mod & KMOD_SHIFT) 
	modifiers |= BIND_MOD_SHIFT;

    if (! (bind = g_hash_table_lookup(client.bindings, &k->keysym.sym)))
	return;

    if (bind->modifiers != modifiers)
	return;

    if (k->type == SDL_KEYUP &&
	    (bind->state != BIND_UP && bind->state != BIND_UPDOWN))
	return;
    if (k->type == SDL_KEYDOWN &&
	    (bind->state != BIND_DOWN && bind->state != BIND_UPDOWN))
	return;

    if (! sh_exec(client.shell, bind->action, &err))
	print("binding error: %s\n", err->message);
}

/**
 * Process a mouse event against the default bindings table
 *
 * @param SDL's mouse button event structure
 */
void
bind_mbutton(SDL_MouseButtonEvent *m)
{
    int32_t sdl_modifiers, modifiers;
    int32_t key;
    binding_t *bind;
    GError *err = NULL;

    key = SDLK_EX_MOUSE_FIRST + m->button;

    if (! (bind = g_hash_table_lookup(client.bindings, &key)))
	return;

    sdl_modifiers = SDL_GetModState();

    modifiers = 0;
    if (sdl_modifiers & KMOD_CTRL) 
	modifiers |= BIND_MOD_CTRL;
    if (sdl_modifiers & KMOD_ALT)
	modifiers |= BIND_MOD_ALT;
    if (sdl_modifiers & KMOD_SHIFT) 
	modifiers |= BIND_MOD_SHIFT;

    if (bind->modifiers != modifiers)
	return;

    if (m->type == SDL_MOUSEBUTTONUP &&
	    (bind->state != BIND_UP && bind->state != BIND_UPDOWN))
	return;
    if (m->type == SDL_MOUSEBUTTONDOWN &&
	    (bind->state != BIND_DOWN && bind->state != BIND_UPDOWN))
	return;

    if (! sh_exec(client.shell, bind->action, &err))
	print("binding error: %s\n", err->message);
}
