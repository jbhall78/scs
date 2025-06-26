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
usage(const char *progname) {
    fprintf(stderr, "Usage: %s [options]\n", progname);
    fprintf(stderr, "  --dedicated | -d       Run as a dedicated server\n");
    fprintf(stderr, "  --windowed  | -w       Run in windowed mode\n");
    fprintf(stderr, "  --server <host> | -s <host> Connect to a game server\n");
    fprintf(stderr, "  -<width>x<height>      Set screen resolution (e.g., -1920x1080, -800x600)\n");
    exit(1); // Exit after showing usage
}

int
main(int argc, char **argv)
{
    int i;
    gboolean dedicated = FALSE, windowed = FALSE;
    uint16_t resx = 0, resy = 0;
    char *host = NULL;

    mem_init();

    init();

    /* read command line options */
    for (i = 1; i < argc; i++) {
        // Corrected strcmp checks for dedicated and fullscreen
        if (strcmp(argv[i], "--dedicated") == 0 || strcmp(argv[i], "-d") == 0) {
            dedicated = TRUE;
        } else if (strcmp(argv[i], "--windowed") == 0 ||
                   strcmp(argv[i], "-w") == 0) {
            windowed = TRUE;
        } else if (strcmp(argv[i], "--server") == 0 ||
                   strcmp(argv[i], "-s") == 0) {
            if (i + 1 < argc) {
                host = strdup(argv[++i]); // Increment i to consume the host argument
            } else {
                fprintf(stderr, "Error: --server requires a host argument.\n");
                usage(argv[0]);
            }
        }
        // --- NEW RESOLUTION PARSING LOGIC ---
        else if (sscanf(argv[i], "-%hux%hu", &resx, &resy) == 2) {
            // sscanf returns the number of items successfully matched and assigned.
            // If it returns 2, both width and height were parsed.
            if (resx == 0 || resy == 0) {
                fprintf(stderr, "Error: Invalid resolution value: %s. Width and height must be positive integers.\n", argv[i]);
                usage(argv[0]);
            }
        }
        // --- END NEW RESOLUTION PARSING LOGIC ---
        else {
            // If the argument doesn't match any known option
            fprintf(stderr, "Error: Unrecognized option '%s'.\n", argv[i]);
            usage(argv[0]);
        }
    }

    if (dedicated)
	    return sv_launch(dedicated, GAME_SERVER);
    else
	    return cl_launch(host, windowed ^ 1, resx, resy);
}
