#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <glib.h>
#include <gnet.h>
#include <libxml/parser.h>

#include "scs.h"
#include "shared.h"
#include "server.h"

GQuark SCS_ERROR;

scs_t scs;

/* stuff client and server threads depend on being initialized */
static void
init(void)
{
    /* initialize system library */
    sys_init();

    /* initialize glib */
    g_thread_init(NULL);
    SCS_ERROR = g_quark_from_static_string("SCS");

    /* initialize gnet */
    gnet_init();
    gnet_ipv6_set_policy(GIPV6_POLICY_IPV4_ONLY);

    /* initialize libxml */
    LIBXML_TEST_VERSION

    xmlInitParser();
}

int
main(int argc, char **argv)
{
    init();

    return sv_launch(TRUE, GAME_SERVER);
}
