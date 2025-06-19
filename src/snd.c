#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

#include "scs.h"
#include "shared.h"

#include "client.h"
#include "snd.h"
#include "wav.h"

static GCache *sound_cache;

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
        // No OpenAL buffers to delete yet
        return FAIL;
    }

    // Load the WAV file into memory
    if ((wav = wav_new(filename)) == NULL) {
        g_set_error(err, SCS_ERROR, SCS_ERROR_SND, "can't load WAV file '%s'", filename);
        alDeleteBuffers(1, &snd->buf_id); // Clean up OpenAL buffer
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
    // Only delete the buffer if it was successfully generated
    if (snd->buf_id != 0) { // Check if a buffer was actually generated
        alDeleteBuffers(1, &snd->buf_id);
        snd->buf_id = 0; // Invalidate ID
    }
    return FAIL;
}

#if 0
gboolean
snd_load_snd(snd_t *snd, char *name, GError **err)
{
    GError      *tmp = NULL;
    char        *filename;
    wav_t       *wav;
    ALenum	format;

    alGetError();

    alGenBuffers(1, &snd->buf_id);

    if (alGetError() != AL_NO_ERROR) {
	    g_set_error(err, SCS_ERROR, SCS_ERROR_SND, "can't generate buffer");
	    return FAIL;
    }

    filename = getdatafilename(FILE_SOUND, name, &tmp);
    if (! filename) {
	    g_propagate_error(err, tmp);
	    return FAIL;
    }

    if ((wav = wav_new(filename)) == NULL) {
	    g_set_error(err, SCS_ERROR, SCS_ERROR_SND, "can't load sound: %s", filename);
	    return FAIL;
    }

    format = 0x1100;
    if (wav->bits_per_sample == 16)
	    format++;

    if (wav->channels == 2)
	    format += 2;

    alBufferData(snd->buf_id, AL_FORMAT_MONO16, wav->data, wav->size, wav->sample_rate);

    wav_free(wav);

    if (alGetError() != AL_NO_ERROR) {
	    g_set_error(err, SCS_ERROR, SCS_ERROR_SND, "can't buffer sound: %s", filename);
	    return FAIL;
    }

    return OK;
}
#endif

void
snd_unload_snd(snd_t *snd)
{
	alDeleteBuffers(1, &snd->buf_id);
}

static gpointer
snd_cache_val_new(gpointer key)
{
    char *name = key;
    snd_t *snd;
    GError *err = NULL;

    snd = g_new0(snd_t, 1);
    snd->name = strdup(name);

    if (! snd_load_snd(snd, name, &err)) {
	    printerr("Couldnt load sound: %s: %s\n", name, err->message);
	    g_free(snd->name);
	    g_free(snd);
	    return NULL;
    }

    return snd;
}

static void
snd_cache_val_destroy(gpointer val)
{
    snd_t *snd = val;

    snd_unload_snd(snd);

    g_free(snd->name);
    g_free(snd);
}

static void snd_cache_key_destroy(gpointer key) { g_free(key); }
static gpointer snd_cache_key_dup(gpointer key) { return strdup(key); }
static void snd_destroy_key(gpointer key) { g_free(key); }

gboolean
snd_init(GError **err)
{
    ALCdevice *dev;
    ALCcontext *ctx;
    char *def;

#if 0
print("SOUND DISABLED IN SOURCE!!!\n");
client.snd_initialized = FALSE;
return OK;
#endif

    sound_cache = g_cache_new(snd_cache_val_new,
	             snd_cache_val_destroy,
		     snd_cache_key_dup,
		     snd_cache_key_destroy,
		     g_str_hash,
		     g_direct_hash,
		     g_str_equal);

    client.sounds = g_hash_table_new_full(g_int_hash, g_int_equal,
	    snd_destroy_key, NULL);

    /* open the device */    
    dev = alcOpenDevice(NULL);

    /* create the context */
    ctx = alcCreateContext(dev, NULL);
    alcMakeContextCurrent(ctx);

    /* check for errors */
    if (alcGetError(dev) != ALC_NO_ERROR) {
#if 1
	    g_set_error(err, SCS_ERROR, SCS_ERROR_SND, "can't open sound device");
	    return FAIL;
#else
	    client.snd_initialized = FALSE;

	    return OK;
#endif
    }
        
    alGetError();

    print("sound system initialized\n");
    def = (char *)alcGetString(dev, ALC_DEVICE_SPECIFIER);
    print("sound: using device '%s'.\n", def);

    client.snd_initialized = TRUE;

    return OK;
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
    alListenerf(AL_GAIN, 1.0);

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

    return g_cache_insert(sound_cache, name);
}

void
snd_unload(snd_t *snd)
{
    if (! client.snd_initialized)
	    return;

    print("removing: %s\n", snd->name);
    g_cache_remove(sound_cache, snd->name);
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
    src->snd = snd;
    src->loop = loop;
    src->obj = obj;

    alGenSources(1, &src->src_id);
    
#if 1
    if (alGetError() != AL_NO_ERROR)
	return NULL;
#endif
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

    assert(src->snd != NULL);

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

    print("getting called: %u\n", src->id);

    if (! src->fragged)
	return FALSE;

    print("fragged: %u\n", src->id);

    alGetSourcei(src->src_id, AL_SOURCE_STATE, &state);
    if (state != AL_STOPPED)
	return FALSE;

    alDeleteSources(1, &src->src_id);

    print("reaped sound: %u\n", src->id);
    
    g_free(src);

    return TRUE;
}

void
snd_src_reap(void)
{
    if (! client.snd_initialized)
	    return;

//printf("wtf: %d\n", g_hash_table_size(client.sounds));

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

    alcMakeContextCurrent(NULL);

    alcDestroyContext(ctx);
    alcCloseDevice(dev);
}
