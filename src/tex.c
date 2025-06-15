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

static void
tex_putpixel(SDL_Surface *surface, uint16_t x, uint16_t y, uint32_t color)
{
    if (SDL_MUSTLOCK(surface)) {
        if (SDL_LockSurface(surface) < 0) {
            return;
        }
    }
    switch (surface->format->BytesPerPixel) {
        case 1: { /* Assuming 8-bpp */
                uint8_t *bufp;

                bufp = (uint8_t *)surface->pixels + y*surface->pitch + x;
                *bufp = color;
            }
            break;

        case 2: { /* Probably 15-bpp or 16-bpp */
                uint16_t *bufp;

                bufp = (uint16_t *)surface->pixels + y*surface->pitch/2 + x;
                *bufp = color;
            }
            break;

        case 3: { /* Slow 24-bpp mode, usually not used */
                uint8_t *bufp;

                bufp = (uint8_t *)surface->pixels + y*surface->pitch + x * 3;
                if (SDL_BYTEORDER == SDL_LIL_ENDIAN) {
                    bufp[0] = color;
                    bufp[1] = color >> 8;
                    bufp[2] = color >> 16;
                } else {
                    bufp[2] = color;
                    bufp[1] = color >> 8;
                    bufp[0] = color >> 16;
                }
            }
            break;

        case 4: { /* Probably 32-bpp */
                uint32_t *bufp;

                bufp = (uint32_t *)surface->pixels + y*surface->pitch/4 + x;
                *bufp = color;
            }
            break;
    }
    if (SDL_MUSTLOCK(surface)) {
        SDL_UnlockSurface(surface);
    }
    //SDL_UpdateRect(surface, x, y, 1, 1);
}

static uint32_t
tex_getpixel(SDL_Surface *surface, uint16_t x, uint16_t y)
{
    uint8_t bpp;
    uint8_t *p;
    uint32_t retval;

    if (SDL_MUSTLOCK(surface)) {
        if (SDL_LockSurface(surface) < 0) {
            return 0;
        }
    }

    bpp = surface->format->BytesPerPixel;
    p = (uint8_t *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
        case 1:
            retval = *p;
            break;

        case 2:
            retval = *(uint16_t *)p;
            break;

        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                retval = p[0] << 16 | p[1] << 8 | p[2];
            else
                retval = p[0] | p[1] << 8 | p[2] << 16;
            break;

        case 4:
            retval = *(uint32_t *)p;
            break;

        default:
            retval = 0;
            break;       /* shouldn't happen, but avoids warnings */
    }

    if (SDL_MUSTLOCK(surface)) {
        SDL_UnlockSurface(surface);
    }

    return retval;
}


/* Convert an SDL Surface to one suitable as on OpenGL texture */
static SDL_Surface *
tex_convert_surf(SDL_Surface *s, int want_alpha, GError **err)
{
    uint32_t rmask, gmask, bmask, amask;
    SDL_Surface *conv;
    uint8_t bpp = want_alpha ? 32 : 24;

    /* SDL interprets each pixel as a 24 or 32-bit number, so our
       masks must depend on the endianness (byte order) of the
       machine. */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000 >> (32 - bpp);
    gmask = 0x00ff0000 >> (32 - bpp);
    bmask = 0x0000ff00 >> (32 - bpp);
    amask = 0x000000ff >> (32 - bpp);
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = want_alpha ? 0xff000000 : 0;
#endif

    /* check if the surface happens to be in the right format */
    if (s->format->BitsPerPixel == bpp
	&& s->format->Rmask == rmask
	&& s->format->Gmask == gmask
	&& s->format->Bmask == bmask
	&& s->format->Amask == amask) {
	/* no conversion needed */
	return s;
    }

    /* SDL surfaces are created with lines padded to start at 32-bit boundaries
       which suits OpenGL well (as long as GL_UNPACK_ALIGNMENT remains
       unchanged from its initial value of 4) */
    if (! (conv = SDL_CreateRGBSurface(SDL_SWSURFACE, s->w, s->h, bpp,
				rmask, gmask, bmask, amask)))
	goto tex_convert_surf_fail;

    /*
    if (want_alpha) {
	SDL_SetAlpha(s, 0, 255);
    }
    */

    /*
     * Do the conversion. If the source surface has a colourkey, then it
     * will be used in the blit. We use the fact that newly created software
     * surfaces are zero-filled, so the pixels not blitted will remain
     * transparent.
     */
    if (SDL_BlitSurface(s, NULL, conv, NULL) < 0)
	goto tex_convert_surf_fail;

    SDL_FreeSurface(s);

    return conv;

tex_convert_surf_fail:
    SDL_FreeSurface(conv);
    g_set_error(err, SCS_ERROR, SCS_ERROR_TEX, "%s", SDL_GetError());
    return NULL;
}

static SDL_Surface *
tex_flip_surf(SDL_Surface *surface)
{
    SDL_Surface *new_surf;
    int x, y;
    Uint32 color;

    new_surf = SDL_CreateRGBSurface(surface->flags, surface->w, surface->h,
				    surface->format->BitsPerPixel,
				    surface->format->Rmask,
				    surface->format->Gmask,
				    surface->format->Bmask,
				    surface->format->Amask);
    assert(new_surf != NULL);

    SDL_LockSurface(surface);
    SDL_LockSurface(new_surf);
    for (x = 0; x < surface->w; x++) {
	for (y = 0; y < surface->h; y++) {
	    int dx = x;
	    int dy = surface->h - y - 1;
	    color = tex_getpixel(surface, x, y);
	    tex_putpixel(new_surf, dx, dy, color);
	}
    }
    SDL_UnlockSurface(surface);
    SDL_UnlockSurface(new_surf);

    return new_surf;
}

static SDL_Surface *
tex_load_sdl_surf(char *filename, GError **err)
{
    SDL_Surface *surf;
    FILE *f;
    GError *tmp = NULL;
    
    f = opendatafile(FILE_TEXTURE, filename, &tmp);
    if (! f) {
	g_propagate_error(err, tmp);
	return NULL;
    }

    surf = IMG_Load_RW(SDL_RWFromFP(f, TRUE), TRUE);
    if (surf) {
	return surf;
    } else {
	g_set_error(err, SCS_ERROR, SCS_ERROR_TEX, "%s", SDL_GetError());
	return NULL;
    }
}

void
tex_load_gl_surf(tex_t *tex)
{
    glGenTextures(1, &tex->id);
    glBindTexture(GL_TEXTURE_2D, tex->id);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    /* I wish this function would do something besides crash when you give
     * it data in a bad format =|
     */

#if 1
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
	    tex->size[WIDTH], tex->size[HEIGHT], 0,
     	    tex->format, GL_UNSIGNED_BYTE, tex->surf);
#else
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA,
	    tex->size[WIDTH], tex->size[HEIGHT],
	    tex->format, GL_UNSIGNED_BYTE, tex->surf);
#endif


    // set some defaults
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

}

static tex_t *
tex_load_surf(tex_t *tex, char *filename, GError **err)
{
    int32_t want_alpha;
    GError *tmp = NULL;
    SDL_Surface *s;
    int32_t size;

    /* load the texture surface from the file */
    if (! (s = tex_load_sdl_surf(filename, &tmp))) {
	goto tex_load_surf_fail;
    }

    /* convert the surface to the format OpenGL wants. */
    want_alpha = s->format->Amask;
    if (! (s = tex_convert_surf(s, want_alpha, &tmp)))
	goto tex_load_surf_fail;

    /* flip the surface the way OpenGL wants. */
    if (! (s = tex_flip_surf(s)))
	goto tex_load_surf_fail;

    tex->size[WIDTH]  = s->w;
    tex->size[HEIGHT] = s->h;
    tex->bpp          = want_alpha ? 32 : 24;
    tex->alpha        = want_alpha ? TRUE : FALSE;
    if (want_alpha)
	tex->format = GL_RGBA;
    else 
	tex->format = GL_RGB;

    /* save the pixel data and get rid of the SDL surface */
    size = tex->size[WIDTH] * tex->size[HEIGHT] * (tex->bpp / 8);
    tex->surf = g_malloc0(size);
    memcpy(tex->surf, s->pixels, size); 
    SDL_FreeSurface(s);

    // NOTE: this prints the bpp after the conversion
    print("loaded texture: %dx%d@%dbpp %s\n", tex->size[WIDTH], tex->size[HEIGHT], tex->bpp, (tex->format == GL_RGB) ? "RGB" : "RGBA");

    tex_load_gl_surf(tex);

    return tex;

tex_load_surf_fail:
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

    if (! tex_load_surf(tex, name, &err)) {
	tex_gen_fake_surf(tex);
	printerr("Couldnt load texture: %s: %s\n", name, err->message);
    }

    return tex;
}

static void
tex_cache_val_destroy(gpointer val)
{
    tex_t *tex = val;

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

    return OK;
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

    glDeleteTextures(1, &tex->id);
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

    tex_load_gl_surf(tex);
}

void
tex_refresh_load(void)
{
    g_cache_key_foreach(TextureCache, tex_refresh_load_func, NULL);
}
