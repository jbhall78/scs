#include "scs.h"
#include "shared.h"
#include "net.h"
#include "client.h"
#include "server.h"

static GAsyncQueue *client_queue;
static GAsyncQueue *server_queue;

static void
net_drv_async_init(net_state_t *net)
{
    if (is_client())
	client_queue = g_async_queue_new();
    else
	server_queue = g_async_queue_new();

    print("async net driver initialized.\n");

    return;
}

static void
net_drv_async_shutdown(net_state_t *net)
{
    if (is_client())
	g_async_queue_unref(client_queue);
    else
	g_async_queue_unref(server_queue);

    return;
}

static conn_t *
net_drv_async_connect(net_state_t *net, char *host, uint16_t port)
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

    /* heh, we don't actually do a connect here */

    return conn;
}

static void
net_drv_async_disconnect(net_state_t *net, conn_t *conn)
{
}

static sock_t *
net_drv_async_bind(net_state_t *net, char *address, GError **err) 
{ 
    sock_t *sock = g_new0(sock_t, 1);

    sock->state = STATE_LISTEN;
    sock->drv_data = NULL;

    return sock;
}

static void
net_drv_async_unbind(net_state_t *net)
{
}

static void
net_drv_async_send(net_state_t *net, conn_t *conn, pkt_t *pkt)
{
    pkt_t *dup = net_pkt_dup(net, pkt);

    if (pkt->type == 0x0105) {
	int a = 1 + 1;
    }

    assert(dup->dbuf->pos == PKT_HEADER_SIZE);
/*
x/128xb (char *)(pkt->dbuf->data + 16)

x/128xb (char *)pkt->dbuf->data
p/x pkt->type
*/

    if (is_client())
	g_async_queue_push(server_queue, dup);
    else
	g_async_queue_push(client_queue, dup);
}

static pkt_t *
net_drv_async_recv(net_state_t *net, sock_t *sock)
{
    pkt_t *pkt;

    if (is_client())
	pkt = g_async_queue_try_pop(client_queue);
    else
	pkt = g_async_queue_try_pop(server_queue);

    if (pkt) {
	pkt->from = g_new0(addr_t, 1);
	pkt->from->addr = strdup("local");
	pkt->from->drv_data = NULL;
    }

    return pkt;
}

static gboolean
net_drv_async_addr_cmp(net_state_t *net, addr_t *addr1, addr_t *addr2)
{
    if (addr1 == NULL || addr2 == NULL)
	return FALSE;

    return TRUE;
}

static void
net_drv_async_addr_delete(net_state_t *net, addr_t **del)
{
    addr_t *addr;

    assert(del != NULL);
    addr = *del;

    g_free(addr);

    *del = NULL;
}

static addr_t *
net_drv_async_addr_dup(net_state_t *net, addr_t *addr)
{
    addr_t *new = g_new0(addr_t, 1);
    new->addr = strdup(addr->addr);
    new->drv_data = NULL;

    return new;
}

static sock_t *
net_drv_async_sock_dup(net_state_t *net, sock_t *sock)
{
    sock_t *new;

    assert(sock != NULL);

    new = g_new0(sock_t, 1);
    new->state = sock->state;
    new->drv_data = NULL;

    return new;
}

static void
net_drv_async_sock_delete(net_state_t *net, sock_t **del)
{
    sock_t *sock;

    assert(del != NULL);

    sock = *del;

    g_free(sock);

    *del = NULL;
}

net_drv_t net_drv_async = {
    net_drv_async_init, 
    net_drv_async_shutdown,
    net_drv_async_connect,
    net_drv_async_disconnect,
    net_drv_async_bind,
    net_drv_async_unbind,
    net_drv_async_send,
    net_drv_async_recv,
    net_drv_async_addr_cmp,
    net_drv_async_addr_delete,
    net_drv_async_addr_dup,
    net_drv_async_sock_dup,
    net_drv_async_sock_delete,
}; 

