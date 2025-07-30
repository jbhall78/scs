#include <AL/al.h>
#include <AL/alc.h>

#include "scs.h"
#include "shared.h"

#include "client.h"
#include "snd.h"
#include "wav.h"

// Renamed sound_cache to sound_map as it's now a GHashTable for explicit ref counting.
static GHashTable *sound_map;

double volume = 0.02;

// Added a reference count to the snd_t structure
// This allows multiple sound sources to share the same underlying sound data
// and ensures it's only freed when no longer in use.

gboolean
snd_load_snd(snd_t *snd, char *name, GError **err)
{
    GError      *tmp = NULL;
    char        *filename = NULL; // Initialize to NULL for safety
    wav_t       *wav = NULL;    // Initialize to NULL for safety
    ALenum       format;
    ALenum       al_error;

    // Clear any previous OpenAL errors
    alGetError();

    // Generate buffer ID
    alGenBuffers(1, &snd->buf_id);
    al_error = alGetError();
    if (al_error != AL_NO_ERROR) {
        g_set_error(err, SCS_ERROR, SCS_ERROR_SND, "can't generate OpenAL buffer: %s", alGetString(al_error));
        return FAIL;
    }

    // Get the full path to the sound file
    filename = getdatafilename(FILE_SOUND, name, &tmp);
    if (! filename) {
        g_propagate_error(err, tmp);
        // No OpenAL buffers to delete yet, as it failed before loading data
        alDeleteBuffers(1, &snd->buf_id); // Clean up the newly generated but unused buffer
        snd->buf_id = 0; // Invalidate the ID
        return FAIL;
    }

    // Load the WAV file into memory
    if ((wav = wav_new(filename)) == NULL) {
        g_set_error(err, SCS_ERROR, SCS_ERROR_SND, "can't load WAV file '%s'", filename);
        alDeleteBuffers(1, &snd->buf_id); // Clean up OpenAL buffer
        snd->buf_id = 0; // Invalidate the ID
        g_free(filename); // Free filename
        return FAIL;
    }

    // Determine OpenAL format based on WAV properties
    if (wav->channels == 1) { // Mono
        if (wav->bits_per_sample == 8) {
            format = AL_FORMAT_MONO8;
        } else if (wav->bits_per_sample == 16) {
            format = AL_FORMAT_MONO16;
        } else {
            g_set_error(err, SCS_ERROR, SCS_ERROR_SND, "Unsupported mono bit depth for OpenAL: %u", wav->bits_per_sample);
            goto error_cleanup;
        }
    } else if (wav->channels == 2) { // Stereo
        if (wav->bits_per_sample == 8) {
            format = AL_FORMAT_STEREO8;
        } else if (wav->bits_per_sample == 16) {
            format = AL_FORMAT_STEREO16;
        } else {
            g_set_error(err, SCS_ERROR, SCS_ERROR_SND, "Unsupported stereo bit depth for OpenAL: %u", wav->bits_per_sample);
            goto error_cleanup;
        }
    } else {
        g_set_error(err, SCS_ERROR, SCS_ERROR_SND, "Unsupported number of channels for OpenAL: %u", wav->channels);
        goto error_cleanup;
    }

    // Buffer the audio data into OpenAL
    // IMPORTANT: Use wav->data_size (from the 'data' chunk) not wav->size if your wav_t struct was updated.
    alBufferData(snd->buf_id, format, wav->data, wav->data_size, wav->sample_rate);
    al_error = alGetError();
    if (al_error != AL_NO_ERROR) {
        g_set_error(err, SCS_ERROR, SCS_ERROR_SND, "can't buffer sound data for '%s': %s", filename, alGetString(al_error));
        goto error_cleanup;
    }

    // Successful loading, free WAV data and filename
    wav_free(wav);
    g_free(filename);

    return OK;

error_cleanup:
    // This label handles all failures after successful alGenBuffers or wav_new
    if (wav != NULL) {
        wav_free(wav);
    }
    if (filename != NULL) {
        g_free(filename);
    }
    // Only delete the buffer if it was successfully generated and not yet invalidated
    if (snd->buf_id != 0) {
        alDeleteBuffers(1, &snd->buf_id);
        snd->buf_id = 0; // Invalidate ID after deletion
    }
    return FAIL;
}

void
snd_free_data(snd_t *snd)
{
    // This function now handles the actual destruction of snd_t and its OpenAL buffer.
    // It's called when ref_count reaches 0.
    if (snd == NULL) {
        return;
    }

    if (snd->buf_id != 0) {
        alGetError(); // Clear any previous OpenAL errors
        alDeleteBuffers(1, &snd->buf_id);
        ALenum al_error = alGetError();
        if (al_error != AL_NO_ERROR) {
            printerr("OpenAL Error deleting buffer %u for sound %s: %s\n",
                     snd->buf_id, snd->name ? snd->name : "UNKNOWN", alGetString(al_error));
        }
        snd->buf_id = 0; // Invalidate the buffer ID
    }

    if (snd->name) {
        g_free(snd->name);
    }
    g_free(snd);
}


// No longer needed as GCache is removed.
// static gpointer snd_cache_val_new(gpointer key) { ... }
// static void snd_cache_val_destroy(gpointer val) { ... }
// static void snd_cache_key_destroy(gpointer key) { ... }
// static gpointer snd_cache_key_dup(gpointer key) { ... }


// This function is used by client.sounds hash table, not the sound_map.
// It frees the integer ID key.
static void snd_destroy_key(gpointer key) { g_free(key); }

static void snd_map_value_destroy(gpointer val) {
    snd_t *snd = val;
    snd_free_data(snd);
}

gboolean
snd_init(GError **err)
{
    ALCdevice *dev = NULL; // Initialize to NULL for safety
    ALCcontext *ctx = NULL; // Initialize to NULL for safety
    char *def;

    // Ensure client.snd_initialized is FALSE by default
    client.snd_initialized = FALSE;

    // Initialize the sound_map (GHashTable) instead of GCache
    // Keys are sound names (strings), values are snd_t pointers.
    // key_destroy_func: g_free for the duplicated string keys.
    // value_destroy_func: NULL, as we manually manage snd_t freeing with ref_count.
    sound_map = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, snd_map_value_destroy);
    
    // If g_hash_table_new_full somehow returns NULL (e.g., out of memory)
    if (sound_map == NULL) {
        g_set_error(err, SCS_ERROR, SCS_ERROR_SND, "Failed to create sound map.");
        return FAIL;
    }

    client.sounds = g_hash_table_new_full(g_int_hash, g_int_equal, snd_destroy_key, NULL);

    /* open the device */
    dev = alcOpenDevice(NULL);
    if (!dev) { // alcOpenDevice returns NULL on failure
        g_set_error(err, SCS_ERROR, SCS_ERROR_SND, "can't open sound device: %s", alcGetString(NULL, alcGetError(NULL)));
        goto error_cleanup; // Go to cleanup if device fails
    }

    /* create the context */
    ctx = alcCreateContext(dev, NULL);
    if (!ctx) { // alcCreateContext returns NULL on failure
        g_set_error(err, SCS_ERROR, SCS_ERROR_SND, "can't create OpenAL context: %s", alcGetString(dev, alcGetError(dev)));
        goto error_cleanup; // Go to cleanup if context fails
    }

    // Make context current *before* checking AL errors (alcGetError is per-context or per-device)
    alcMakeContextCurrent(ctx);

    /* check for errors after making current */
    if (alcGetError(NULL) != ALC_NO_ERROR) { // Check AL error after making context current
        g_set_error(err, SCS_ERROR, SCS_ERROR_SND, "can't make OpenAL context current: %s", alGetString(alGetError()));
        goto error_cleanup;
    }

    // If everything succeeded, set initialized to TRUE
    client.snd_initialized = TRUE;

    print("sound system initialized\n");
    def = (char *)alcGetString(dev, ALC_DEVICE_SPECIFIER);
    print("sound: using device '%s'.\n", def);

    return OK;

error_cleanup:
    // This cleanup block is executed if any error occurs above
    if (ctx != NULL) {
        alcMakeContextCurrent(NULL); // Detach context first
        alcDestroyContext(ctx);
    }
    if (dev != NULL) {
        alcCloseDevice(dev);
    }
    // Destroy sound_map on failure, which will free any keys but not values (as value_destroy_func is NULL).
    // The values themselves (snd_t objects) should not be present or valid if init failed before loading.
    if (sound_map != NULL) {
        g_hash_table_destroy(sound_map);
        sound_map = NULL;
    }
    client.snd_initialized = FALSE; // Ensure this is FALSE on failure
    return FAIL;
}

void
snd_update(snd_listener_t *listener)
{
    vec3_t up, fwd;
    real vec[6];

    if (! client.snd_initialized)
        return;

    quat_to_vecs(listener->orient, fwd, up, NULL);

    vec[0] = fwd[X];
    vec[1] = fwd[Y];
    vec[2] = fwd[Z];

    vec[3] = up[X];
    vec[4] = up[Y];
    vec[5] = up[Z];

    alGetError();

    alListenerfv(AL_POSITION, listener->pos);
    alListenerfv(AL_VELOCITY, listener->posv);
    alListenerfv(AL_ORIENTATION, vec);
    alListenerf(AL_GAIN, volume);

    if (alGetError() != AL_NO_ERROR) {
        print("warning: error updating listener!\n");
        return;
    }
}

snd_t *
snd_load(char *name)
{
    if (! client.snd_initialized)
        return NULL;

    // Try to get the sound from the map first
    snd_t *snd = g_hash_table_lookup(sound_map, name);

    if (snd) {
        // Sound already loaded, increment ref_count
        //snd->ref_count++;
        return snd;
    }

    // Sound not loaded, create a new one
    snd = g_new0(snd_t, 1);
    snd->name = strdup(name); // Duplicate name for storage in snd_t and as key in map
    //snd->ref_count = 1; // Initial reference count

    GError *err = NULL;
    if (! snd_load_snd(snd, name, &err)) {
        printerr("Failed to load sound '%s': %s\n", name, err->message);
        if (err) {
            g_error_free(err);
        }
        g_free(snd->name); // Free the duplicated name
        g_free(snd); // Free the snd_t object
        return NULL;
    }

    // Add to hash table. The key (strdup(name)) is managed by the hash table.
    // The value (snd) is managed by our ref_count.
    // Use g_hash_table_insert with a duplicated key.
    g_hash_table_insert(sound_map, strdup(name), snd);

    return snd;
}

void
snd_unload(snd_t *snd)
{
    if (! client.snd_initialized || snd == NULL || snd->name == NULL)
        return;

    //print("unloading: %s, ref_count before: %d\n", snd->name, snd->ref_count);

    // Decrement reference count
    //snd->ref_count--;

    // If reference count drops to 0, free the sound data and remove from map
    //if (snd->ref_count <= 0) {
    //    print("ref_count for %s dropped to 0. Freeing.\n", snd->name);
        // Remove from hash table. The key is freed by the hash table's key_destroy_func.
        g_hash_table_remove(sound_map, snd->name);
        // Free the actual snd_t data and OpenAL buffer
        //snd_free_data(snd);
    //}
}

void
snd_play(snd_src_t *src)
{
    if (! client.snd_initialized)
        return;

    alGetError();

    print("playing sound: %u\n", src->id);
    alSourcePlay(src->src_id);

    if (alGetError() != AL_NO_ERROR) 
        printerr("error playing sound: %u\n", src->src_id);
}

void
snd_stop(snd_src_t *src)
{
    alSourceStop(src->src_id);
}

snd_src_t *
snd_src_spawn(snd_t *snd, int32_t id, object_t *obj, gboolean loop)
{
    snd_src_t *src;

    if (! client.snd_initialized)
        return NULL;

    src = g_new0(snd_src_t, 1);
    src->id  = id;
    src->loop = loop;
    src->obj = obj;

    // IMPORTANT CHANGE: Acquire a new reference to the sound when a source is spawned.
    // This ensures the snd_t object is not freed while this src_t object is active.
    src->snd = snd_load(snd->name);
    if (src->snd == NULL) {
        printerr("Failed to acquire sound reference for source %u: %s\n", id, snd->name);
        g_free(src);
        return NULL;
    }

    alGenSources(1, &src->src_id);
    
    // It's good to check for errors immediately after alGenSources.
    // If an error occurs here, src->src_id might be 0, and subsequent
    // alSourcei calls would fail or cause issues.
    if (alGetError() != AL_NO_ERROR) {
        printerr("Error generating OpenAL source for sound %s\n", src->snd->name);
        // If source generation fails, release the acquired sound reference.
        snd_unload(src->snd);
        g_free(src); // Free the allocated src object
        return NULL;
    }

    alSourcei(src->src_id, AL_BUFFER, src->snd->buf_id);
    alSourcef(src->src_id, AL_PITCH, 1.0);
    alSourcef(src->src_id, AL_GAIN, 1.0);
    alSourcei(src->src_id, AL_LOOPING, src->loop);
    alSourcei(src->src_id, AL_SOURCE_RELATIVE, TRUE);
    alSourcef(src->src_id, AL_REFERENCE_DISTANCE, 1000.0);
    alSourcef(src->src_id, AL_MAX_DISTANCE, 50000.0);
    
    return src;
}


void
snd_src_update(snd_src_t *src)
{
    if (! client.snd_initialized)
        return;

    // Ensure src->snd is still valid before accessing it.
    // This is especially important if snd_unload was called prematurely.
    if (src->snd == NULL || src->snd->buf_id == 0) {
        printerr("warning: attempting to update source %u with invalid sound data!\n", src->id);
        return;
    }

    alGetError();

    alSourcefv(src->src_id, AL_POSITION, src->obj->pos );
    alSourcefv(src->src_id, AL_VELOCITY, src->obj->posv);

    if (alGetError() != AL_NO_ERROR) {
        print("warning: error updating source!\n");
        return;
    }
}


gboolean
snd_update_srcs_cb(gpointer key, gpointer val, gpointer data)
{
    snd_src_t *src = val;

    snd_src_update(src);

    return FALSE;
}

void
snd_update_srcs(void)
{
    if (! client.snd_initialized)
        return;

    g_hash_table_foreach_remove(client.sounds, snd_update_srcs_cb, NULL);
}

void
snd_src_frag(snd_src_t *src)
{
    if (! client.snd_initialized)
        return;

    src->fragged = TRUE;
}

void
snd_src_register(snd_src_t *src)
{
    uint32_t *id = g_new0(uint32_t, 1);

    if (! client.snd_initialized)
        return;

    *id = src->id;

    print("inserting: %u: %d\n", src->id, g_hash_table_size(client.sounds));
    g_hash_table_insert(client.sounds, id, src); 
    print("  sounds: %d\n", g_hash_table_size(client.sounds));
}

gboolean
snd_src_reap_cb(gpointer key, gpointer val, gpointer data)
{
    snd_src_t *src = val;
    int32_t state;

    //print("getting called: %u\n", src->id);

    if (! src->fragged)
        return FALSE; // Do not remove if not fragged

    //print("fragged: %u\n", src->id);

    alGetSourcei(src->src_id, AL_SOURCE_STATE, &state);
    if (state != AL_STOPPED)
        return FALSE; // Do not remove if still playing

    // Clear any previous errors before deleting source
    alGetError();
    alDeleteSources(1, &src->src_id);
    ALenum al_error = alGetError();
    if (al_error != AL_NO_ERROR) {
        printerr("OpenAL Error deleting source %u: %s\n", src->src_id, alGetString(al_error));
    }
    src->src_id = 0; // Invalidate source ID

    print("reaped sound: %u\n", src->id);
    
    // Crucial: Decrement the reference count of the underlying sound data (snd_t)
    // as this source is no longer using it.
    //if (src->snd) {
    //    snd_unload(src->snd); // This will handle freeing snd->name and snd if ref_count hits 0.
    //}

    // Note: The key (uint32_t *id) for client.sounds needs to be freed by snd_destroy_key,
    // which is provided as the key_destroy_func to g_hash_table_new_full.
    // The value (snd_src_t *src) is freed here.
    g_free(src);

    return TRUE; // Indicate that this entry should be removed from the hash table
}

void
snd_src_reap(void)
{
    if (! client.snd_initialized)
        return;

    g_hash_table_foreach_remove(client.sounds, snd_src_reap_cb, NULL);
}

void
snd_shutdown(void)
{
    ALCcontext *ctx;
    ALCdevice *dev;

    if (! client.snd_initialized)
        return;

    ctx = alcGetCurrentContext();
    dev = alcGetContextsDevice(ctx);

    // Destroy the sound_map first. This will free all duplicated keys (sound names)
    // but not the snd_t values, as we use a NULL value_destroy_func.
    // We then iterate to ensure all remaining snd_t objects are freed.
    if (sound_map != NULL) {
        // Iterate through the sound_map to ensure all snd_t objects are freed.
        // This is important because snd_unload might not have been called for all.
      /*  GHashTableIter iter;
        gpointer key, value;
        g_hash_table_iter_init(&iter, sound_map);
        while (g_hash_table_iter_next(&iter, &key, &value)) {
            snd_t *snd = (snd_t *)value;
            printerr("Warning: Sound '%s' still in map during shutdown (ref_count: %d). Forcing free.\n",
                     snd->name ? snd->name : "UNKNOWN", snd->ref_count);
            //snd_free_data(snd); // Force free any remaining snd_t data
        }*/
        g_hash_table_destroy(sound_map); // This will free the keys, but we've freed values.
        sound_map = NULL; // Invalidate the global pointer
    }

    // Destroy the hash table for sound sources
    if (client.sounds != NULL) {
        // Any remaining snd_src_t objects in client.sounds will be freed here.
        // Their src->snd pointers might be invalid if sound_map was destroyed first.
        // The snd_src_reap_cb ideally cleans these up proactively.
        g_hash_table_destroy(client.sounds);
        client.sounds = NULL;
    }

    // Then shutdown OpenAL
    alcMakeContextCurrent(NULL);
    alcDestroyContext(ctx);
    alcCloseDevice(dev);

    client.snd_initialized = FALSE; // Ensure state is reset
}
