#include <string.h>

#include <glib.h>
#include <SDL_opengl.h>

#include "scs.h"
#include "shared.h"
#include "client.h"
#include "ui.h"
#include "sys_gl.h"

static font_t *BigFont;
static font_t *MediumFont;
static font_t *SmallFont;

// kinda cheezy but I want a central location to change fonts at =)
font_t *
ui_font_get(char *desc)
{
    struct {
	char *name;
	font_t *fnt;
    } fnttbl[] = {
	{ "console",            SmallFont       },
	{ "menu_title",		BigFont		},
	{ "menu_item",		MediumFont	},
	{ "menu_version",	SmallFont	},
	{ "computer",		SmallFont	},
	{ "input",              SmallFont       },
	{ "big",		BigFont		},
	{ "medium",		MediumFont	},
	{ "small",		SmallFont	},
    };
    uint8_t i;

    for (i = 0; i < G_N_ELEMENTS(fnttbl); i++) {
	if (strcmp(fnttbl[i].name, desc) == 0)
	    return fnttbl[i].fnt;
    }

    return SmallFont;
}

static gboolean
ui_font_read_textures(font_t *fnt, FILE *f, GError **err)
{
    int i;
    char buf[BUFSIZ];
    tex_t *tex;

    for (i = 0; i < fnt->ntextures; i++) {
	readline(buf, sizeof(buf), f);
	buf[strlen(buf) - 1] = 0;

	tex = tex_load(buf);
    	g_ptr_array_add(fnt->textures, tex);

	fnt->texsize[WIDTH]  = tex->size[WIDTH];
	fnt->texsize[HEIGHT] = tex->size[HEIGHT];
    
	if (fnt->filter_method == 0) {
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
    }


    return OK;
}

static gboolean
ui_font_read_hdr(font_t *fnt, FILE *f, GError **err)
{
    char buf[BUFSIZ];

    /* read font header in */
    readline(buf, sizeof(buf), f);
    sscanf(buf, "%hu %hu %hu %hu ",
	    &fnt->ntextures,
	    &(fnt->chrsize[WIDTH]), &(fnt->chrsize[HEIGHT]),
	    &fnt->filter_method);

    readline(buf, sizeof(buf), f);
    sscanf(buf, "%hu %hu ", &(fnt->range[LO]), &(fnt->range[HI]));
    
    readline(buf, sizeof(buf), f);
    fnt->name = strdup(buf);
    fnt->hratio = (real)fnt->chrsize[HEIGHT] / (real)fnt->chrsize[WIDTH];

    fnt->textures = g_ptr_array_sized_new(fnt->ntextures);

    return OK;
}

static gboolean
ui_font_compute_glyphs(font_t *fnt, GError **err)
{
    int32_t i, x, y, lines, tex;
    int32_vec2_t chrs;
    vec2_t chrsize, scale;
    real left, right, top, bot;

    chrs[X] = fnt->texsize[WIDTH] / fnt->chrsize[WIDTH];
    chrs[Y] = fnt->texsize[HEIGHT] / fnt->chrsize[HEIGHT];
    chrsize[WIDTH] = 1.0 / fnt->chrsize[WIDTH];
    chrsize[HEIGHT] = 1.0 / fnt->chrsize[HEIGHT];

    for (i = 0, lines = 0; i < fnt->range[HI] - fnt->range[LO]; i++) {
	if (i % chrs[X] == 0)
	    lines++;
	
    	if (fnt->chrsize[HEIGHT] * lines >= fnt->texsize[HEIGHT])
	    break;
    }

    scale[X] = 1.0 / fnt->texsize[WIDTH];
    scale[Y] = 1.0 / fnt->texsize[HEIGHT];

    tex = 0;
    for (i = 0, x = 0, y = 0; i < fnt->range[HI] - fnt->range[LO]; i++) {
	left  = fnt->chrsize[WIDTH] * x; 
	right = fnt->chrsize[WIDTH] * x + fnt->chrsize[WIDTH];
	top   = fnt->texsize[HEIGHT] - fnt->chrsize[HEIGHT] * y;
       	bot   = fnt->texsize[HEIGHT] - fnt->chrsize[HEIGHT] * y - fnt->chrsize[HEIGHT];

	fnt->glyphs[i][LEFT]  = left  * scale[X];
	fnt->glyphs[i][RIGHT] = right * scale[X];
	fnt->glyphs[i][TOP]   = top   * scale[Y];
	fnt->glyphs[i][BOT]   = bot   * scale[Y];
	fnt->texids[i] = tex;

	if ((i+1) % chrs[X] == 0) {
	    x = 0;
	    y++;
	    if (y % lines == 0) {
		tex++;
		y = 0;
	    }
	} else
	    x++;
    }

    return OK;
}

font_t *
ui_font_load(char *filename)
{
    FILE *f;
    char buf[BUFSIZ];
    font_t *fnt;

    fnt = g_malloc0(sizeof(font_t));

    /* open the font definition file and read info */
    g_snprintf(buf, sizeof(buf), "%s.fnt", filename);
    f = opendatafile(FILE_FONT, buf, NULL);
    assert(f != NULL);

    ui_font_read_hdr(fnt, f, NULL);

    ui_font_read_textures(fnt, f, NULL);

    fclose(f);

    ui_font_compute_glyphs(fnt, NULL);

#if 0 /* dump font info */
    {
	int x, y;
	for (x = 0; x < fnt->range[HI] - fnt->range[LO]; x++) {
	    printf("[%c] = [ ", x + fnt->range[LO]);
	    for (y = 0; y < 4;y++) {
		printf("%f ", fnt->glyphs[x][y]);
	    }
	    printf("%d ]\n", fnt->texids[x]);
	}
    }
#endif

    return fnt;
}

static void
ui_font_render_text(font_t *fnt, int size, char *string, int len)
{
    int32_t bound = -1;
    uint32_t index;
    uint16_t texid;
    int32_t i;
    real left, right, top, bot;
    tex_t *tex;

    for (i = 0; i < len; i++) {
	if (string[i] > fnt->range[HI] || string[i] < fnt->range[LO]) {
//	    printerr("char out of bounds: %d\n", string[i]);
	    continue;
	}
	index = string[i] - fnt->range[LO];
	texid = fnt->texids[index];

	// bind texture 
	if (texid != bound) {
	    tex = g_ptr_array_index(fnt->textures, texid);

	    glBindTexture(GL_TEXTURE_2D, tex->id);
	    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	    bound = texid;
	}

	left  = fnt->glyphs[index][LEFT];
	right = fnt->glyphs[index][RIGHT];
	top   = fnt->glyphs[index][TOP];
	bot   = fnt->glyphs[index][BOT];

	// find tex coords
	glBegin(GL_QUADS);
	    glTexCoord2(left,  bot);
	    glVertex2(i * size, 0);
	    
	    glTexCoord2(right, bot);
	    glVertex2((i + 1) * size, 0);

	    glTexCoord2(right, top);
	    glVertex2((i + 1) * size, fnt->hratio * size); 

	    glTexCoord2(left,  top);
	    glVertex2(i * size, fnt->hratio * size);
	glEnd();
    }
}

void
ui_font_draw_text(font_t *fnt,
	int16_t x, int16_t y, int16_t size, vec3_t color, char *fmt, ...)
{
    va_list ap;
    char text[BUFSIZ];

    memset(text, 0, sizeof(text));

    va_start(ap, fmt);
    vsnprintf(text, sizeof(text), fmt, ap);
    va_end(ap);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);

    glPushMatrix();
    glTranslate(x, y, 0);
    glColor3v(color);

    ui_font_render_text(fnt, size, text, strlen(text));
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
}

uint32_t
ui_font_get_height(font_t *fnt, uint32_t size)
{
    return fnt->hratio * size;
}

uint32_t
ui_font_get_strwidth(font_t *fnt, uint32_t size, int strlen)
{
    uint32_t width;
    // not sure the reason for the 1.01 
    //return 1.01 * size * strlen(str);

    width = size * strlen;

    return width;
}

gboolean
ui_font_init(GError **err)
{
    BigFont    = ui_font_load("big");
    MediumFont = ui_font_load("medium");
    SmallFont  = ui_font_load("small");

    return OK;
}
