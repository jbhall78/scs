#include <string.h>

#include <glib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>

#include "scs.h"
#include "shared.h"
#include "client.h"
#include "sys_gl.h"

static GCache *TextureCache;

// --- tex_putpixel and tex_getpixel remain removed ---

/* Convert an SDL Surface to one suitable as an OpenGL texture */
static SDL_Surface *
tex_convert_surf(SDL_Surface *s, GError **err)
{
    SDL_Surface *conv = NULL;
    Uint32 desired_format;

    // --- CRITICAL CORRECTION HERE ---
    // Correctly check for transparency in SDL 2.0:
    // 1. Does the surface format natively have an alpha channel?
    // 2. Has a color key been explicitly set on this surface?
    if ((s->format->Amask != 0) || SDL_HasColorKey(s)) {
        desired_format = SDL_PIXELFORMAT_RGBA32; // Use RGBA for transparent surfaces
    } else {
        desired_format = SDL_PIXELFORMAT_RGB24; // Use RGB for opaque surfaces
    }

    /* Convert the surface to the desired format.
       SDL_ConvertSurfaceFormat handles endianness, byte order, and applies
       color keys into the alpha channel if SDL_SetColorKey was called on 's'. */
    conv = SDL_ConvertSurfaceFormat(s, desired_format, 0); // 0 for default flags
    if (!conv) {
        g_set_error(err, SCS_ERROR, SCS_ERROR_TEX, "Failed to convert surface format: %s", SDL_GetError());
        return NULL;
    }

    SDL_FreeSurface(s); // Free the original surface after conversion

    return conv;
}

// Optimized SDL 2.0 way to flip a surface vertically (in-place or to a new surface)
static SDL_Surface *
tex_flip_surf(SDL_Surface *surface)
{
    SDL_Surface *new_surf = NULL;
    int row_bytes = surface->w * surface->format->BytesPerPixel;
    int y;
    uint8_t *src_row, *dst_row;

    // Create a new surface with the same format. SDL_SWSURFACE indicates software surface.
    new_surf = SDL_CreateRGBSurfaceWithFormat(SDL_SWSURFACE, surface->w, surface->h,
                                               surface->format->BitsPerPixel,
                                               surface->format->format);
    if (!new_surf) {
        fprintf(stderr, "Failed to create new surface for flipping: %s\n", SDL_GetError());
        return NULL;
    }

    // Ensure surfaces are locked for direct pixel access
    if (SDL_MUSTLOCK(surface)) { SDL_LockSurface(surface); }
    if (SDL_MUSTLOCK(new_surf)) { SDL_LockSurface(new_surf); }

    // Copy rows from bottom of source to top of destination, effectively flipping
    for (y = 0; y < surface->h; y++) {
        src_row = (uint8_t *)surface->pixels + (surface->h - 1 - y) * surface->pitch; // Read from bottom up
        dst_row = (uint8_t *)new_surf->pixels + y * new_surf->pitch;                   // Write from top down
        memcpy(dst_row, src_row, row_bytes);
    }

    // Unlock surfaces
    if (SDL_MUSTLOCK(new_surf)) { SDL_UnlockSurface(new_surf); }
    if (SDL_MUSTLOCK(surface)) { SDL_UnlockSurface(surface); }

    SDL_FreeSurface(surface); // Free the original surface as it's no longer needed

    return new_surf;
}


static SDL_Surface *
tex_load_sdl_surf(char *filename, GError **err)
{
    SDL_Surface *surf;
    FILE *f;
    GError *tmp = NULL;

    f = opendatafile(FILE_TEXTURE, filename, &tmp);
    if (!f) {
        g_propagate_error(err, tmp);
        return NULL;
    }

    surf = IMG_Load_RW(SDL_RWFromFP(f, TRUE), TRUE); // SDL_RWFromFP takes ownership of FILE* f
    if (surf) {
        return surf;
    } else {
        g_set_error(err, SCS_ERROR, SCS_ERROR_TEX, "Failed to load image '%s': %s", filename, IMG_GetError());
        return NULL;
    }
}

void
tex_load_gl_surf(tex_t *tex)
{
    glGenTextures(1, &tex->id);
    glBindTexture(GL_TEXTURE_2D, tex->id);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4); // SDL surfaces are typically 4-byte aligned for speed

    GLint internalFormat;
    GLenum format;

    if (tex->format == GL_RGBA) {
        internalFormat = GL_RGBA;
        format = GL_RGBA; // Assuming the converted SDL surface is now RGBA byte order
    } else {
        internalFormat = GL_RGB;
        format = GL_RGB;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat,
        tex->size[WIDTH], tex->size[HEIGHT], 0,
        format, GL_UNSIGNED_BYTE, tex->surf);

    // Set some defaults for texture filtering and wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Use mipmaps for minification for better quality when scaled down
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D); // Generate mipmaps after uploading data

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

static tex_t *
tex_load_surf(tex_t *tex, char *filename, GError **err)
{
    GError *tmp = NULL;
    SDL_Surface *s = NULL;
    int32_t size;

    /* Load the texture surface from the file */
    s = tex_load_sdl_surf(filename, &tmp);
    if (!s) {
        goto tex_load_surf_fail;
    }

    // --- Font Color Key Handling ---
    // If this is a font texture (using a simple filename heuristic),
    // apply a color key for transparency. This MUST happen before conversion.
    if (strstr(filename, "font") != NULL) {
        Uint32 colorkey = SDL_MapRGB(s->format, 255, 0, 255); // Example: pure magenta background
        if (SDL_SetColorKey(s, SDL_TRUE, colorkey) < 0) {
            fprintf(stderr, "Warning: Failed to set color key for font '%s': %s\n", filename, SDL_GetError());
            // This is a warning; the process can continue, but transparency might be off.
        }
    }

    /* Convert the surface to the format OpenGL wants (RGB24 or RGBA32).
       This step will incorporate the color key into the alpha channel. */
    if (!(s = tex_convert_surf(s, &tmp))) {
        goto tex_load_surf_fail;
    }

    /* Flip the surface vertically to match OpenGL's texture coordinate system (origin bottom-left). */
    if (!(s = tex_flip_surf(s))) {
        goto tex_load_surf_fail;
    }

    // Determine final properties after conversion and flipping
    tex->size[WIDTH]  = s->w;
    tex->size[HEIGHT] = s->h;
    // Check if the *converted and flipped* surface has an alpha mask
    tex->alpha        = (s->format->Amask != 0) ? TRUE : FALSE;
    tex->bpp          = s->format->BytesPerPixel * 8; // Get actual BPP of converted surface

    if (tex->alpha) {
        tex->format = GL_RGBA;
    } else {
        tex->format = GL_RGB;
    }

    /* Save the pixel data and get rid of the SDL surface */
    size = tex->size[WIDTH] * tex->size[HEIGHT] * (tex->bpp / 8);
    tex->surf = g_malloc0(size);
    memcpy(tex->surf, s->pixels, size);
    SDL_FreeSurface(s); // Free the final processed SDL surface

    print("loaded texture: %dx%d@%dbpp %s\n", tex->size[WIDTH], tex->size[HEIGHT], tex->bpp, (tex->format == GL_RGB) ? "RGB" : "RGBA");

    tex_load_gl_surf(tex); // Upload to OpenGL

    return tex;

tex_load_surf_fail:
    // Ensure any partially loaded surface is freed on failure
    if (s) SDL_FreeSurface(s);
    g_propagate_error(err, tmp);
    return NULL;
}

void
tex_gen_fake_surf(tex_t *tex)
{
    int32_t size;

    tex->size[WIDTH] = 16;
    tex->size[HEIGHT] = 16;
    tex->alpha = FALSE;
    tex->bpp = 24;

    size = tex->size[WIDTH] * tex->size[HEIGHT] * (tex->bpp / 8);
    tex->surf = g_malloc0(size);
    memset(tex->surf, 1, size);
}

static gpointer
tex_cache_val_new(gpointer key)
{
    char *name = key;
    tex_t *tex;
    GError *err = NULL;

    tex = g_malloc0(sizeof(tex_t));
    tex->name = strdup(name);

    if (!tex_load_surf(tex, name, &err)) {
        tex_gen_fake_surf(tex);
        printerr("Couldn't load texture: %s: %s\n", name, err->message);
        if (err) g_error_free(err); // Free the error
    }

    return tex;
}

static void
tex_cache_val_destroy(gpointer val)
{
    tex_t *tex = val;

    if (tex->id != 0) { // Only delete if a GL texture was actually generated
        glDeleteTextures(1, &tex->id);
    }
    g_free(tex->surf);
    g_free(tex->name);
    g_free(tex);
}

static void tex_cache_key_destroy(gpointer key) { g_free(key); }
static gpointer tex_cache_key_dup(gpointer key) { return strdup(key); }

/* API Functions */
gboolean
tex_init(GError **err)
{
    TextureCache = g_cache_new(tex_cache_val_new,
                   tex_cache_val_destroy,
                   tex_cache_key_dup,
                   tex_cache_key_destroy,
                   g_str_hash,
                   g_direct_hash,
                   g_str_equal);

    print("texture system initialized\n");

    return TRUE; // Changed from OK to TRUE for standard boolean return.
}

tex_t *
tex_load(char *name)
{
    return g_cache_insert(TextureCache, name);
}

void
tex_unload(tex_t *tex)
{
    g_cache_remove(TextureCache, tex->name);
}

void
tex_refresh_unload_func(gpointer key, gpointer value, gpointer data)
{
    tex_t *tex = key;

    if (tex->id != 0) { // Only delete if it's a valid ID
        glDeleteTextures(1, &tex->id);
        tex->id = 0; // Reset ID after deletion
    }
}

void
tex_refresh_unload(void)
{
    g_cache_key_foreach(TextureCache, tex_refresh_unload_func, NULL);
}

void
tex_refresh_load_func(gpointer key, gpointer value, gpointer data)
{
    tex_t *tex = key;

    // Re-upload to GL. `tex->surf` is assumed to still hold the pixel data.
    tex_load_gl_surf(tex);
}

void
tex_refresh_load(void)
{
    g_cache_key_foreach(TextureCache, tex_refresh_load_func, NULL);
}
