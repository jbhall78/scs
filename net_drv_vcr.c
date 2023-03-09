#include <stdio.h>
#include <string.h>

#include <glib.h>

#include "scs.h"
#include "shared.h"
#include "net.h"

#include "client.h"

gboolean
net_vcr_open(net_state_t *net, gboolean record, char *filename, GError **err)
{
    char name[BUFSIZ];
    int16_t i, len;
    net_vcr_hdr_t hdr;

    if (record) {
	snprintf(name, sizeof(name), "%s/demos/%s", sys_datadir(), "demo00.dem");
	len = strlen(name);
	for (i = 0; i <= 99; i++) {
	    name[len-6] = i / 10 + '0';
	    name[len-5] = i % 10 + '0';

	    /* remember: always open in binary, we run on win32 too! */
	    net->demo = fopen(name, "rb");
	    if (! net->demo)
		break;	/* file doesnt exist */
	    fclose(net->demo);
	}
	net->demo = fopen(name, "wb");
	if (i == 100 || ! net->demo) {
	    g_set_error(err, SCS_ERROR, SCS_ERROR_NET,
		    "can't open a new demo file\n");
	    return FALSE;
	}

	/* fill in header */
	strcpy(hdr.magic, "SCSd");	/* scs demo */
	hdr.version = SCS_NET_VER;
	strcpy(hdr.player, "hyena");
	strcpy(hdr.desc, "SCS Demo Recording");
	hdr.creation = GUINT32_TO_LE(time(NULL));
	hdr.start_ticks = GUINT32_TO_LE(scs.clock->ticks());

	if (fwrite(&hdr, sizeof(hdr), 1, net->demo) <= 0) {
	    g_set_error(err, SCS_ERROR, SCS_ERROR_NET,
		    "can't write header to demo file\n");
	    fclose(net->demo);
	    return FALSE;
	}
    } else {
	snprintf(name, sizeof(name), "%s/demos/%s", sys_datadir(), filename);

	/* remember: always open in binary, we run on win32 too! */
	net->demo = fopen(name, "rb");
	if (! net->demo) {
	    g_set_error(err, SCS_ERROR, SCS_ERROR_NET,
		    "can't open demo file: %s\n", name);
	    return FALSE;
	} 

	if (fread(&hdr, sizeof(hdr), 1, net->demo) <= 0) {
	    g_set_error(err, SCS_ERROR, SCS_ERROR_NET,
		    "can't read header from demo file: %s\n", name);
	    return FALSE;
	}

	net->tick_offset = GUINT32_FROM_LE(hdr.start_ticks) - scs.clock->ticks();
    }
    net->filename = strdup(name);
    print("VCR: %s %s\n", record ? "recording to" : "reading from",
	    net->filename);

    return TRUE;
}

void
net_vcr_close(net_state_t *net)
{
    if (net->filename) {
	print("VCR: closed: %s\n", net->filename);
	g_free(net->filename);
	net->filename = NULL;
    }
    if (net->demo)
	fclose(net->demo);
    net->demo = NULL;
}

gboolean
net_vcr_write(net_state_t *net, pkt_t *pkt, GError **err)
{
    pkt_vcr_hdr_t hdr;
    char *buf;
    uint32_t len;

    buf = net_pkt_pack(pkt, &len);
    assert(buf != NULL);

    hdr.ticks = GUINT32_TO_LE(scs.clock->ticks());
    hdr.len = GUINT16_TO_LE(len);

    if (fwrite(&hdr, sizeof(hdr), 1, net->demo) <= 0) {
	g_set_error(err, SCS_ERROR, SCS_ERROR_NET,
		"can't write header to demo file\n");
	return FAIL;
    }
    if (fwrite(buf, len, 1, net->demo) <= 0) {
	g_set_error(err, SCS_ERROR, SCS_ERROR_NET,
		"can't write data to demo file\n");
	return FAIL;
    }

    return OK;
}

pkt_t *
net_vcr_read(net_state_t *net)
{
    char buf[2048];
    pkt_t *pkt;
    pkt_vcr_hdr_t hdr;
    GError *tmp = NULL;

    if (fread(&hdr, sizeof(hdr), 1, net->demo) <= 0) {
	if (feof(net->demo)) {
		/* a lot of work to make demo looping work properly
		 * first, a lot of objects are not being free'd
		 * need to track these down or start using a garbage
		 * collector
		 * second, the demo is not closed / reopened, eventually
		 * we will run out of file descriptors and the engine will
		 * crash.
		 * finally, we need to set a console variable to enable
		 * and disable demo looping
		 */
#ifndef DEDICATED
		client.callbacks->unload(&tmp);
		client.callbacks->load(&tmp);
#endif
	}
	return NULL;
    }

    /* see if its time to return a packet */
    if (GUINT32_FROM_LE(hdr.ticks) - net->tick_offset < scs.clock->ticks()) {
	if (fread(buf, GUINT16_FROM_LE(hdr.len), 1, net->demo) <= 0)
	    return NULL;

	/* unpack the packet */
	pkt = net_pkt_unpack(buf, GUINT16_FROM_LE(hdr.len));
	pkt->to = NULL;

	/* fill in from address */
	pkt->from = g_new0(addr_t, 1);
	pkt->from->addr = strdup("local");

	return pkt;
    } 

    /* go back so we can read the header again later */
    fseek(net->demo, -sizeof(hdr), SEEK_CUR);

    return NULL;
}

/* network driver functions */
static void 
net_drv_vcr_init(net_state_t *net)
{
}

static void
net_drv_vcr_shutdown(net_state_t *net)
{
    net_vcr_close(net);
}

static conn_t *
net_drv_vcr_connect(net_state_t *net, char *host, uint16_t port)
{
    conn_t *conn;
    sock_t *sock;
    addr_t *addr;

    /* create socket */
    sock = g_new0(sock_t, 1);
    sock->state = STATE_CONNECTING;
    sock->drv_data = NULL;

    /* create address */
    addr = g_new0(addr_t, 1);
    addr->addr = strdup("local");
    addr->drv_data = NULL;

    /* create connection */
    conn = g_new0(conn_t, 1);
    conn->session_id = 0;
    conn->rx_reliable_seq = 0;
    conn->tx_reliable_seq = 0;
    conn->rx_unreliable_seq = 0;
    conn->tx_unreliable_seq = 0;
    conn->last_reliable_xmit = 0;
    conn->last_tx_ping = 0;
    conn->last_rx_ping = 0;
    conn->sock = sock;
    conn->addr = addr;
    conn->reliable_sendq = g_queue_new();

    assert(conn->reliable_sendq != NULL);

    net->connections = g_slist_prepend(net->connections, conn);
    net->sock = sock;

    net_vcr_open(net, FALSE, net->filename, NULL);

    return conn;
}

static void
net_drv_vcr_disconnect(net_state_t *net, conn_t *conn)
{

}

static sock_t *
net_drv_vcr_bind(net_state_t *net, char *address, GError **err)
{
    return NULL;
}

static void
net_drv_vcr_unbind(net_state_t *net)
{

}

static void
net_drv_vcr_send(net_state_t *net, conn_t *conn, pkt_t *pkt)
{

}

static pkt_t *
net_drv_vcr_recv(net_state_t *net, sock_t *sock)
{
    pkt_t *pkt;

    if ((pkt = net_vcr_read(net)) != NULL)
	return pkt;

    return NULL;
}

static gboolean
net_drv_vcr_addr_cmp(net_state_t *net, addr_t *addr1, addr_t *addr2)
{
    if (addr1 == NULL || addr2 == NULL)
	return FALSE;

    return TRUE;
}

static void
net_drv_vcr_addr_delete(net_state_t *net, addr_t **del)
{
    addr_t *addr;

    assert(del != NULL);
    addr = *del;

    g_free(addr);

    *del = NULL;
}

static addr_t *
net_drv_vcr_addr_dup(net_state_t *net, addr_t *addr)
{
    addr_t *new = g_new0(addr_t, 1);
    new->addr = strdup(addr->addr);
    new->drv_data = NULL;

    return new;
}

static sock_t *
net_drv_vcr_sock_dup(net_state_t *net, sock_t *sock)
{
    sock_t *new;

    assert(sock != NULL);

    new = g_new0(sock_t, 1);
    new->state = sock->state;
    new->drv_data = NULL;

    return new;
}

static void
net_drv_vcr_sock_delete(net_state_t *net, sock_t **del)
{
    sock_t *sock;

    sock = *del;

    assert(sock != NULL);

    g_free(sock);

    *del = NULL;
}

net_drv_t net_drv_vcr = {
    net_drv_vcr_init, 
    net_drv_vcr_shutdown,
    net_drv_vcr_connect,
    net_drv_vcr_disconnect,
    net_drv_vcr_bind,
    net_drv_vcr_unbind,
    net_drv_vcr_send,
    net_drv_vcr_recv,
    net_drv_vcr_addr_cmp,
    net_drv_vcr_addr_delete,
    net_drv_vcr_addr_dup,
    net_drv_vcr_sock_dup,
    net_drv_vcr_sock_delete,
}; 

