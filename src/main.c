#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <glib.h>
#include <gio/gio.h>
#include <libxml/parser.h>

#include "mem.h"
#include "scs.h"
#include "shared.h"
#include "client.h"
#include "server.h"

GQuark SCS_ERROR;

scs_t scs;

/* stuff client and server threads depend on being initialized */
static void
init(void)
{
    /* initialize system library */
    sys_init();

    /* initialize SDL library */
    SDL_Init(0);

    SDL_StartTextInput();

    /* initialize glib */
    //g_thread_init(NULL);
    SCS_ERROR = g_quark_from_static_string("SCS");

    /* initialize libxml */
    LIBXML_TEST_VERSION

    xmlInitParser();
}

static void
usage(char *prog)
{
    printerr("%s [-d] [-f] [-s server] [-(resx)x(resy)]\n", prog);
    exit(1);
}

int
main(int argc, char **argv)
{
    int i;
    gboolean dedicated = FALSE, fullscreen = FALSE;
    uint16_t resx = 1920, resy = 1080;
    char *host = NULL;

    mem_init();

    init();

    /* read command line options */
    for (i = 1; i < argc; i++) {
	if (strcmp(argv[i], "--dedicated") == 0 || strcmp(argv[i], "-d") == 0)
	    dedicated = TRUE;
	else if (strcmp(argv[i], "--fullscreen") == 0 ||
		 strcmp(argv[i], "-f") == 0)
	    fullscreen = TRUE;
	else if (strcmp(argv[i], "--server") == 0 ||
		 strcmp(argv[i], "-s") == 0) {
	    if (i+1 < argc) 
		host = strdup(argv[++i]);
	    else
		usage(argv[0]);
	} else if (strcmp(argv[i], "-2560x1024") == 0) {
	    resx = 2560;
	    resy = 1024;
	} else if (strcmp(argv[i], "-1920x1080") == 0) {
	    resx = 1920;
	    resy = 1080;
	} else if (strcmp(argv[i], "-1600x1200") == 0) {
	    resx = 1600;
	    resy = 1200;
	} else if (strcmp(argv[i], "-1280x1024") == 0) {
	    resx = 1280;
	    resy = 1024;
	} else if (strcmp(argv[i], "-1152x864") == 0) {
	    resx = 1152;
	    resy = 864;
	} else if (strcmp(argv[i], "-1024x768") == 0) {
	    resx = 1024;
	    resy = 768;
	} else if (strcmp(argv[i], "-800x600") == 0) {
	    resx = 800;
	    resy = 600;
	} else if (strcmp(argv[i], "-640x480") == 0) {
	    resx = 640;
	    resy = 480;
	} else
	    usage(argv[0]);
    }

    if (dedicated)
	return sv_launch(dedicated, GAME_SERVER);
    else
	return cl_launch(host, fullscreen, resx, resy);
}
