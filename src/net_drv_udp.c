#include <gio/gio.h>
#include <gio/ginetaddress.h>

#ifdef SCS_ENGINE
#include <string.h>
#include "scs.h"
#include "shared.h"
#include "net.h"
#else
#include <glib.h>
#include <gnet.h>
#include "megalib.h"
#endif

typedef struct {
    GInetAddress *addr;
    GUdpSocket *sock;
} sock_data_t;

typedef struct {
    GInetAddress *addr;
} addr_data_t;

static void
net_drv_udp_init(net_state_t *net)
{
    return;
}

static void
net_drv_udp_shutdown(net_state_t *net)
{
    return;
}

static conn_t *
net_drv_udp_connect(net_state_t *net, char *host, uint16_t port)
{
    conn_t *conn;
    sock_t *sock;
    addr_t *addr;
    sock_data_t *sock_data;
    addr_data_t *addr_data;

    assert(host != NULL);
    assert(port != 0);
    
    /* create socket */
    sock_data = g_new0(sock_data_t, 1);
    sock_data->sock = gnet_udp_socket_new();
    sock_data->addr = gnet_udp_socket_get_local_inetaddr(sock_data->sock);
    sock = g_new0(sock_t, 1);
    sock->state = STATE_CONNECTING;
    sock->drv_data = sock_data;

    /* create address */
    addr_data = g_new0(addr_data_t, 1);
    addr_data->addr = gnet_inetaddr_new(host, port);
    assert(addr_data->addr != NULL); /* XXX - FIXME */
    addr = g_new0(addr_t, 1);
    addr->addr = gnet_inetaddr_get_canonical_name(addr_data->addr);
    addr->drv_data = addr_data;
    assert(addr_data->addr != NULL);

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
net_drv_udp_disconnect(net_state_t *net, conn_t *conn)
{
    sock_data_t *data = conn->sock->drv_data;

    gnet_udp_socket_delete(data->sock);
    gnet_inetaddr_delete(data->addr);
    g_free(data);

    g_free(conn); 
}

static sock_t *
net_drv_udp_bind(net_state_t *net, char *address, GError **err)
{
    uint32_t port = SCS_NET_PORT;
    sock_t *sock = g_new0(sock_t, 1);
    sock_data_t *data = g_new0(sock_data_t, 1);

    data->sock = gnet_udp_socket_new_with_port(port);
    sock->state = STATE_LISTEN;
    sock->drv_data = data;

    if (data->sock == NULL) {
#ifdef SCS_ENGINE
	g_set_error(err, SCS_ERROR, SCS_ERROR_NET,
		"UDP: unable to bind to port %d\n", port);

	g_free(data);
	g_free(sock);
#endif
	return NULL;
    }

    print("udp net driver listening on port: %d.\n", port);

    return sock;
}

static void
net_drv_udp_unbind(net_state_t *net)
{
    sock_t *sock;
    sock_data_t *data;

    assert(net->sock != NULL);
    sock = net->sock;
    assert(sock->drv_data != NULL);
    data = sock->drv_data;

    gnet_udp_socket_delete(data->sock);

}

static void
net_drv_udp_send(net_state_t *net, conn_t *conn, pkt_t *pkt)
{
    char *buf;
    uint32_t len; 
    sock_data_t *sock_data;
    addr_data_t *addr_data;

    assert(net != NULL);
    assert(conn != NULL);
    assert(conn->sock != NULL);
    assert(conn->addr != NULL);
    assert(pkt != NULL);

    addr_data = conn->addr->drv_data;
    sock_data = conn->sock->drv_data;

    assert(sock_data != NULL);
    assert(addr_data != NULL);

    buf = net_pkt_pack(pkt, &len); 
    assert(buf != NULL);

    if (gnet_udp_socket_send(sock_data->sock, buf, len, addr_data->addr))
	print("send error\n");

    // no longer nessecary a pointer to the data buffer is just returned
    //g_free(buf);
}

static pkt_t *
net_drv_udp_recv(net_state_t *net, sock_t *sock)
{
    sock_data_t *sock_data = sock->drv_data;
    char buf[2048];
    int32_t len;
    pkt_t *pkt;
    addr_data_t *addr_data;
    GInetAddress *addr;

    if (gnet_udp_socket_has_packet(sock_data->sock)) {
	addr_data = g_new0(addr_data_t, 1);

	len = gnet_udp_socket_receive(sock_data->sock, buf, sizeof(buf), &addr);
//	printf("received: %d\n", len);

	/* fill in the address field (note: gnet is weird about addresses */
	assert(addr != NULL);
	addr_data->addr = addr;

	pkt = net_pkt_unpack(buf, len);
	pkt->to = NULL;

	/* fill in from address */
	pkt->from = g_new0(addr_t, 1);
	pkt->from->addr = gnet_inetaddr_get_canonical_name(addr);
	pkt->from->drv_data = addr_data;

	return pkt;
    }

    return NULL;
}

static gboolean
net_drv_udp_addr_cmp(net_state_t *net, addr_t *addr1, addr_t *addr2)
{
    addr_data_t *data1 = addr1->drv_data;
    addr_data_t *data2 = addr2->drv_data;

    if (data1 == NULL || data1->addr == NULL)
	return FALSE;

    if (data2 == NULL || data2->addr == NULL)
	return FALSE;
    
    return gnet_inetaddr_equal(data1->addr, data2->addr);
}

static void
net_drv_udp_addr_delete(net_state_t *net, addr_t **dup)
{
    addr_t *addr;
    addr_data_t *data;

    assert(dup != NULL);
    addr = *dup;
    assert(addr->drv_data != NULL);
    data = addr->drv_data;

    if (data->addr)
	gnet_inetaddr_unref(data->addr);

    g_free(data);

    if (addr->addr)
	g_free(addr->addr);

    g_free(addr);
}

static addr_t *
net_drv_udp_addr_dup(net_state_t *net, addr_t *addr)
{
    addr_data_t *data, *new_data;
    addr_t *new;

    assert(addr != NULL);

    data = addr->drv_data;
   
    /* create our data object */
    new_data = g_new0(addr_data_t, 1);
    if (data->addr) {
	gnet_inetaddr_ref(data->addr);
	new_data->addr = data->addr;
    }

    /* create our generic address object */
    new = g_new0(addr_t, 1);
    new->drv_data = new_data;
    new->addr = strdup(addr->addr);

    return new;
}

static sock_t *
net_drv_udp_sock_dup(net_state_t *net, sock_t *sock)
{
    sock_data_t *data, *new_data;
    sock_t *new;

    assert(sock != NULL);

    data = sock->drv_data;

    /* create our data object */
    new_data = g_new0(sock_data_t, 1);
    if (data->addr) 
	new_data->addr = gnet_inetaddr_clone(data->addr);
    if (data->sock) {
	gnet_udp_socket_ref(data->sock);
	new_data->sock = data->sock;
    }

    new = g_new0(sock_t, 1);
    new->state = sock->state;
    new->drv_data = new_data;

    return new;
}

static void
net_drv_udp_sock_delete(net_state_t *net, sock_t **del)
{
    sock_t *sock;
    sock_data_t *data;

    assert(del != NULL);

    sock = *del;
    data = sock->drv_data;

    if (data->addr)
	gnet_inetaddr_delete(data->addr);
    if (data->sock) 
	gnet_udp_socket_unref(data->sock);

    g_free(data);
    g_free(sock);

    *del = NULL;
}

net_drv_t net_drv_udp = {
    net_drv_udp_init, 
    net_drv_udp_shutdown,
    net_drv_udp_connect,
    net_drv_udp_disconnect,
    net_drv_udp_bind,
    net_drv_udp_unbind,
    net_drv_udp_send,
    net_drv_udp_recv,
    net_drv_udp_addr_cmp,
    net_drv_udp_addr_delete,
    net_drv_udp_addr_dup,
    net_drv_udp_sock_dup,
    net_drv_udp_sock_delete,
}; 

