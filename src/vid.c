#include <string.h>
#include <math.h>

#include <glib.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include "scs.h"
#include "shared.h"
#include "client.h"
#include "sys_gl.h"

static gboolean
vid_init_sdl(GError **err)
{
    int flags;

#if 0
    flags = (SDL_INIT_VIDEO|SDL_INIT_NOPARACHUTE);
#else
    flags = (SDL_INIT_VIDEO);
#endif
    
#ifdef __linux__
    setenv("SDL_VIDEO_FULLSCREEN_HEAD", "0", 1);
#endif

    if (SDL_InitSubSystem(flags) < 0)
    	goto fail;

    if (! vid_set_mode())
    	goto fail;

    print("Vendor    : %s\n", glGetString(GL_VENDOR));
    print("Renderer  : %s\n", glGetString(GL_RENDERER));
    print("Version   : %s\n", glGetString(GL_VERSION));
#if 1
    print("Extensions: %s\n", glGetString(GL_EXTENSIONS));
#endif

    SDL_WM_SetCaption("[SCS] - Space Combat Simulator", NULL);
    SDL_EnableUNICODE(SDL_ENABLE);

    /* FIXME - move this to mouse init code */
    SDL_ShowCursor(SDL_DISABLE);

    return TRUE;

fail:
    g_set_error(err, SCS_ERROR, SCS_ERROR_VID, "%s", SDL_GetError());
    return FALSE;
}

/*
void
vid_perspective(float fov, float aspect, float zNear, float zFar)
{
    float y = tan((fov*M_PI)/360) * zNear;
    float x = y * aspect;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glFrustum(-x, x, -y, y, zNear, zFar);

    glGetv(GL_PROJECTION_MATRIX, client.proj_mat);

    glMatrixMode(GL_MODELVIEW);

} 
*/

void vid_perspective(float horizontal_fov, float aspect, float zNear, float zFar)
{
    // Calculate vertical FOV from desired horizontal FOV and aspect ratio
    float vertical_fov_rad = 2.0f * atan(tan(horizontal_fov * M_PI / 360.0f) / aspect);
    float vertical_fov_deg = vertical_fov_rad * 180.0f / M_PI;

    float y = tan((vertical_fov_deg * M_PI) / 360) * zNear;
    float x = y * aspect;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-x, x, -y, y, zNear, zFar);
    glGetv(GL_PROJECTION_MATRIX, client.proj_mat);
    glMatrixMode(GL_MODELVIEW);
}

static void
vid_init_gl(void)
{
    vid_perspective(client.fov,
	            (real)client.res[X] / (real)client.res[Y],
		    client.znear, client.zfar);

#if 1
    {
    	int32_t lights, size;

	    glGetIntegerv(GL_MAX_LIGHTS, &lights);
	    print("OpenGL: max lights: %d\n", lights);

	    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &size);
	    print("OpenGL: max texture size: %dx%d\n", size, size);
    }
#endif

    // check for framebuffer-object extension
    {
	    GLenum status;
	    status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	    assert(status == GL_FRAMEBUFFER_COMPLETE_EXT);
    }
}

// must be called before vid_init
void
vid_set(gboolean fullscreen, uint16_t resx, uint16_t resy)
{
    client.fullscreen		= fullscreen;
    client.res[X]		= resx;
    client.res[Y]		= resy;
    client.center[X]		= resx / 2;
    client.center[Y]		= resy / 2;
    client.ortho[WIDTH]		= resx; //800;
    client.ortho[HEIGHT]	= resy; //600;
    //client.fov			= 45.0;
    client.fov			= 100.0;
    client.znear		= 1.0; //3.0;
    client.zfar			= 256000.0;

    glViewport(0, 0, client.res[X], client.res[Y]);
}

gboolean
vid_set_mode(void)
{
    const SDL_VideoInfo *vinfo;
    int rgb_size[3];
    int bpp;
    int flags;

    vinfo = SDL_GetVideoInfo();
    bpp = vinfo->vfmt->BitsPerPixel;

    client.bpp = bpp;

    switch (bpp) {
        case 8:
            rgb_size[0] = 3;
            rgb_size[1] = 3;
            rgb_size[2] = 2;
            break;
        case 15:
        case 16:
            rgb_size[0] = 5;
            rgb_size[1] = 5;
            rgb_size[2] = 5;
            break;
        default:
            rgb_size[0] = 8;
            rgb_size[1] = 8;
            rgb_size[2] = 8;
            break;
    }

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE,     rgb_size[0]);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,   rgb_size[1]);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,    rgb_size[2]);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,   16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    /* XXX - We may want to move this out into its own function if we want to
     * allow for on the fly video mode switching. */
    flags = SDL_OPENGL;

    if (client.fullscreen)
        flags |= SDL_FULLSCREEN;
    else
	    flags |= SDL_RESIZABLE;

    client.surface = SDL_SetVideoMode(client.res[X], client.res[Y], bpp, flags);
    if (! client.surface)
	    return FAIL;

    print("Initialized %dx%d@%d%sdisplay.\n", client.res[X], client.res[Y],
	    bpp, client.fullscreen ? " fullscreen " : " ");

    return OK;
}

gboolean
vid_init(GError **err)
{
    GError *err2 = NULL;


    if (! vid_init_sdl(&err2)) {
    	g_propagate_error(err, err2);
	    return FALSE;
    }

    vid_init_gl();

    return TRUE;
}

gboolean
vid_shutdown(GError **err)
{
    SDL_QuitSubSystem(SDL_INIT_VIDEO);

    return OK;
}

void
vid_toggle_fullscreen(void)
{
    /* we have to refresh all textures when switching between fullscreen
       mode or else the textures will be erased from memory */
    tex_refresh_unload();

    // make sure we only go into fullscreen with a valid fullscreen mode
    if (client.fullscreen) {
	    vec2_cp(client.prev_res, client.res);
    } else {
	    vec2_cp(client.res, client.prev_res);
    	vec2_cp(client.fullscreen_res, client.res);
    }

    vid_set(client.fullscreen ^ 1, client.res[X], client.res[Y]);

    vid_set_mode();

    tex_refresh_load();
}

void
vid_resize(gboolean fullscreen, int32_t x, int32_t y)
{
    /* we have to refresh all textures when switching between fullscreen
       mode or else the textures will be erased from memory */
    tex_refresh_unload();

    vid_set(fullscreen, x, y);

    vid_set_mode();

    tex_refresh_load();
}

void
vid_screenshot(void)
{
    unsigned char *buf;
    char name[BUFSIZ];
    int32_t i, c, swap;
    uint32_t w, h, len;
    FILE *f;

    snprintf(name, sizeof(name), "%s/screenshots/%s",
	     sys_datadir(), "game00.tga");

    len = strlen(name);
    for (i = 0; i <= 99; i++) {
    	name[len-6] = i / 10 + '0';
    	name[len-5] = i % 10 + '0';

    	/* remember: always open in binary, we run in win32 too! */
    	f = fopen(name, "rb");
    	if (! f)
    	    break;  // file doesn't exist
    	fclose (f);
    }
    if (i == 100) {
    	print("couldn't create a file\n");
    	return;
    }

    w = client.res[WIDTH];
    h = client.res[HEIGHT];

    buf = g_malloc(w * h * 3 + 18);
    memset(buf, 0, 18);
    buf[2] = 2;	    // uncompressed type
    buf[12] = w & 255;
    buf[13] = w >> 8;
    buf[14] = h & 255;
    buf[15] = h >> 8;
    buf[16] = 24;	    // pixel size

    glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, buf + 18);

    // swap rgb to bgr
    c = 18 + w * h * 3;
    for (i = 18 ; i<c ; i+=3) {
    	swap = buf[i];
    	buf[i] = buf[i+2];
    	buf[i+2] = swap;
    }

    f = fopen(name, "wb");
    fwrite(buf, 1, c, f);
    fclose(f);

    g_free(buf);
    print("wrote %dx%d image \"%s\"\n", w, h, name);
}

