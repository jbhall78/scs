#ifdef SCS_ENGINE
#include <string.h>
#include "scs.h"
#include "shared.h"
#include "net.h"
#else
#include <glib.h>
#include <gio/gio.h>
#include "megalib.h"
#endif

void net_drv_udp_addr_delete(net_state_t *net, addr_t **dup);

typedef struct {
    GSocket *sock;
    GInetAddress *addr; // For local bound address, if needed
} sock_data_t;

typedef struct {
    GInetAddress *addr;
    uint16_t port; // Storing port for remote address comparison and sending
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
    addr_t *addr_generic; // Renamed to avoid confusion with addr_data_t's 'addr'
    sock_data_t *sock_data;
    addr_data_t *addr_data_specific;
    GError *error = NULL;
    GSocketAddress *remote_socket_address = NULL;
    GList *resolved_inet_addresses = NULL; // List of GInetAddress

    assert(host != NULL);
    assert(port != 0);

    /* create socket */
    sock_data = g_new0(sock_data_t, 1);
    sock_data->sock = g_socket_new(G_SOCKET_FAMILY_IPV4, G_SOCKET_TYPE_DATAGRAM, G_SOCKET_PROTOCOL_UDP, &error);
    if (error) {
        g_warning("Failed to create UDP socket: %s", error->message);
        g_error_free(error);
        g_free(sock_data);
        return NULL;
    }

    g_socket_set_blocking(sock_data->sock, FALSE);

    sock = g_new0(sock_t, 1);
    sock->state = STATE_CONNECTING;
    sock->drv_data = sock_data;

    /* create address for the remote host */
    addr_data_specific = g_new0(addr_data_t, 1);

    // Use g_resolver_lookup_by_name to get a list of GInetAddress objects
    GResolver *resolver = g_resolver_get_default();
    resolved_inet_addresses = g_resolver_lookup_by_name(resolver, host, NULL, &error);
    g_object_unref(resolver); // Done with the resolver object

    if (error) {
        print("Failed to resolve host '%s': %s", host, error->message);
        g_error_free(error);
        g_object_unref(sock_data->sock);
        g_free(sock_data);
        g_free(addr_data_specific);
        g_free(sock);
        return NULL;
    }

    if (!resolved_inet_addresses) {
        print("No addresses found for host '%s'", host);
        g_object_unref(sock_data->sock);
        g_free(sock_data);
        g_free(addr_data_specific);
        g_free(sock);
        return NULL;
    }

    // Pick a suitable IPv4 address from the resolved list
    GInetAddress *inet_address_to_use = NULL;
    for (GList *l = resolved_inet_addresses; l != NULL; l = g_list_next(l)) {
        GInetAddress *current_inet_addr = G_INET_ADDRESS(l->data);
        if (g_inet_address_get_family(current_inet_addr) == G_SOCKET_FAMILY_IPV4) {
            inet_address_to_use = g_object_ref(current_inet_addr); // Take a ref
            break;
        }
    }
    g_list_free_full(resolved_inet_addresses, g_object_unref); // Free list nodes and their contained GInetAddress objects

    if (!inet_address_to_use) {
        print("No IPv4 address found for host '%s'", host);
        g_object_unref(sock_data->sock);
        g_free(sock_data);
        g_free(addr_data_specific);
        g_free(sock);
        return NULL;
    }

    // Now, create the GInetSocketAddress using the chosen GInetAddress and the port
    //remote_socket_address = g_inet_socket_address_new(inet_address_to_use, port);
    //assert(remote_socket_address != NULL);

    addr_data_specific->addr = inet_address_to_use; // addr_data_specific now owns this ref
    addr_data_specific->port = port; // Store the port

    addr_generic = g_new0(addr_t, 1);
    addr_generic->addr = g_inet_address_to_string(addr_data_specific->addr);
    addr_generic->drv_data = addr_data_specific;
    addr_generic->port = addr_data_specific->port; // Ensure addr_t also has the port for `addr_cmp` etc.
    assert(addr_data_specific->addr != NULL);

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
    conn->addr = addr_generic;
    conn->reliable_sendq = g_queue_new();

    assert(conn->reliable_sendq != NULL);

    net->connections = g_slist_prepend(net->connections, conn);
    net->sock = sock;

    //g_object_unref(remote_socket_address); // Free the socket_address after use

    return conn;
}

// ... (rest of your functions: disconnect, bind, send, recv, cmp, delete, dup)

// net_drv_udp_bind function (should be fine as last provided)
static sock_t *
net_drv_udp_bind(net_state_t *net, char *address, GError **err)
{
    uint32_t port = SCS_NET_PORT;
    sock_t *sock = g_new0(sock_t, 1);
    sock_data_t *data = g_new0(sock_data_t, 1);
    GError *internal_error = NULL;

    data->sock = g_socket_new(G_SOCKET_FAMILY_IPV4, G_SOCKET_TYPE_DATAGRAM, G_SOCKET_PROTOCOL_UDP, &internal_error);
    if (internal_error) {
        g_set_error(err, G_IO_ERROR, G_IO_ERROR_FAILED,
                    "UDP: Failed to create socket: %s", internal_error->message);
        g_error_free(internal_error);
        g_free(data);
        g_free(sock);
        return NULL;
    }
    g_socket_set_blocking(data->sock, FALSE);

    GInetAddress *inet_address = NULL;
    if (address && strlen(address) > 0) {
        inet_address = g_inet_address_new_from_string(address);
        if (!inet_address) {
            g_set_error(err, G_IO_ERROR, G_IO_ERROR_FAILED,
                        "UDP: Invalid bind address '%s'. Must be an IP literal or empty for 'any'.", address);
            g_object_unref(data->sock);
            g_free(data);
            g_free(sock);
            return NULL;
        }
    } else {
        inet_address = g_inet_address_new_from_string("0.0.0.0"); // Bind to all IPv4 interfaces
    }
    assert(inet_address != NULL);

    GSocketAddress *socket_address = g_inet_socket_address_new(inet_address, port);
    g_object_unref(inet_address);

    if (!g_socket_bind(data->sock, socket_address, TRUE, &internal_error)) {
        g_set_error(err, G_IO_ERROR, G_IO_ERROR_ADDRESS_IN_USE,
                    "UDP: Unable to bind to port %d: %s", port, internal_error->message);
        g_error_free(internal_error);
        g_object_unref(socket_address);
        g_object_unref(data->sock);
        g_free(data);
        g_free(sock);
        return NULL;
    }
    g_object_unref(socket_address);

    GSocketAddress *local_bound_address = g_socket_get_local_address(data->sock, NULL);
    if (local_bound_address && G_IS_INET_SOCKET_ADDRESS(local_bound_address)) {
        data->addr = g_object_ref(g_inet_socket_address_get_address(G_INET_SOCKET_ADDRESS(local_bound_address)));
        g_object_unref(local_bound_address);
    }

    sock->state = STATE_LISTEN;
    sock->drv_data = data;

    print("udp net driver listening on port: %d.\n", port);

    return sock;
}

static void
net_drv_udp_disconnect(net_state_t *net, conn_t *conn)
{
    sock_data_t *sock_data = conn->sock->drv_data;
    // addr_data_t *addr_data = conn->addr->drv_data; // Unused, addr_delete will handle it

    if (sock_data->sock) {
        g_object_unref(sock_data->sock);
    }
    g_free(sock_data); // Free the drv_data for the socket

    // net_drv_udp_addr_delete will handle addr_data and conn->addr
    net_drv_udp_addr_delete(net, &conn->addr);

    // Free the sock_t and conn_t objects themselves
    g_free(conn->sock); // Free the generic sock_t
    g_queue_free(conn->reliable_sendq); // Free the queue
    g_free(conn);
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

    if (data->sock) {
        g_object_unref(data->sock); // Unref the GSocket to close it
    }
    if (data->addr) {
        g_object_unref(data->addr); // Unref the GInetAddress if stored
    }
    g_free(data);
    g_free(sock); // Free the sock_t structure itself
    net->sock = NULL; // Clear the net's socket pointer
}

static void
net_drv_udp_send(net_state_t *net, conn_t *conn, pkt_t *pkt)
{
    char *buf;
    uint32_t len;
    sock_data_t *sock_data;
    addr_data_t *addr_data;
    GError *error = NULL;

    assert(net != NULL);
    assert(conn != NULL);
    assert(conn->sock != NULL);
    assert(conn->addr != NULL);
    assert(pkt != NULL);

    addr_data = conn->addr->drv_data;
    sock_data = conn->sock->drv_data;

    assert(sock_data != NULL);
    assert(addr_data != NULL);
    assert(addr_data->addr != NULL);

    //print("DEBUG: Sending to IP: %s\n", g_inet_address_to_string(addr_data->addr));
    //print("DEBUG: Sending to Port: %hu\n", addr_data->port);
    //print("DEBUG: Family of sending IP: %d (IPv4=%d, IPv6=%d)\n",
    //      g_inet_address_get_family(addr_data->addr), G_SOCKET_FAMILY_IPV4, G_SOCKET_FAMILY_IPV6);

    buf = net_pkt_pack(pkt, &len);
    assert(buf != NULL);

    GSocketAddress *dest_addr = g_inet_socket_address_new(addr_data->addr, addr_data->port);
    assert(dest_addr != NULL); // Should be valid if addr_data->addr and port are valid

    gssize bytes_sent = g_socket_send_to(sock_data->sock, dest_addr, buf, len, NULL, &error);
    if (bytes_sent == -1) {
        print("send error: %s", error->message);
        g_error_free(error);
    }

    g_object_unref(dest_addr);
}

static pkt_t *
net_drv_udp_recv(net_state_t *net, sock_t *sock)
{
    sock_data_t *sock_data = sock->drv_data;
    char buf[2048];
    gssize len;
    pkt_t *pkt;
    GSocketAddress *sender_address = NULL;
    GError *error = NULL;

    // Ensure the socket is valid before attempting to receive
    if (sock_data == NULL || sock_data->sock == NULL) {
        g_warning("net_drv_udp_recv called with invalid socket data.");
        return NULL;
    }

    len = g_socket_receive_from(sock_data->sock, &sender_address, buf, sizeof(buf), NULL, &error);

    if (len > 0) {
        addr_data_t *addr_data = g_new0(addr_data_t, 1); // This needs to be freed if pkt creation fails
        pkt = net_pkt_unpack(buf, len);

        if (pkt == NULL) { // Handle unpack failure
            g_free(addr_data); // Free the allocated addr_data
            if (sender_address) g_object_unref(sender_address);
            g_warning("Failed to unpack received packet.");
            return NULL;
        }
        pkt->to = NULL; // Assuming this is intentional

        if (sender_address && G_IS_INET_SOCKET_ADDRESS(sender_address)) {
            addr_data->addr = g_object_ref(g_inet_socket_address_get_address(G_INET_SOCKET_ADDRESS(sender_address)));
            addr_data->port = g_inet_socket_address_get_port(G_INET_SOCKET_ADDRESS(sender_address));

            /* fill in from address */
            pkt->from = g_new0(addr_t, 1);
            pkt->from->addr = g_inet_address_to_string(addr_data->addr);
            pkt->from->drv_data = addr_data; // addr_data ownership transferred to pkt->from
            pkt->from->port = addr_data->port;
        } else {
            g_warning("Received packet from non-INET address or NULL sender address. Ignoring.");
            // Free allocated resources for this case
            g_free(addr_data); // This addr_data was specifically for the 'from' address
            if (pkt) g_free(pkt); // pkt was unpacked, so free it
            if (sender_address) g_object_unref(sender_address);
            return NULL;
        }

        if (sender_address) {
            g_object_unref(sender_address); // Always unref sender_address when done
        }
        return pkt;
    } else if (len == -1) {
        if (error != NULL) { // Check error before accessing its members
            if (g_error_matches(error, G_IO_ERROR, G_IO_ERROR_WOULD_BLOCK) ||
                g_error_matches(error, G_IO_ERROR, G_IO_ERROR_AGAIN)) {
                // No packet available, this is expected in non-blocking mode
            } else {
                g_warning("Receive error: %s", error->message);
            }
            g_error_free(error); // Always free error
        }
    }
    return NULL;
}

#if 0
static pkt_t *
net_drv_udp_recv(net_state_t *net, sock_t *sock)
{
    sock_data_t *sock_data = sock->drv_data;
    char buf[2048];
    gssize len;
    pkt_t *pkt;
    GSocketAddress *sender_address = NULL;
    GError *error = NULL;

    len = g_socket_receive_from(sock_data->sock, &sender_address, buf, sizeof(buf), NULL, &error);

    if (len > 0) {
        addr_data_t *addr_data = g_new0(addr_data_t, 1);
        pkt = net_pkt_unpack(buf, len);
        pkt->to = NULL;

        if (sender_address && G_IS_INET_SOCKET_ADDRESS(sender_address)) {
            addr_data->addr = g_object_ref(g_inet_socket_address_get_address(G_INET_SOCKET_ADDRESS(sender_address)));
            addr_data->port = g_inet_socket_address_get_port(G_INET_SOCKET_ADDRESS(sender_address));

            /* fill in from address */
            pkt->from = g_new0(addr_t, 1);
            pkt->from->addr = g_inet_address_to_string(addr_data->addr);
            pkt->from->drv_data = addr_data;
            pkt->from->port = addr_data->port; // Ensure pkt->from also has the port
        } else {
            g_warning("Received packet from non-INET address or NULL sender address.");
            g_free(addr_data);
            if (pkt) g_free(pkt);
            pkt = NULL;
        }

        if (sender_address) {
            g_object_unref(sender_address);
        }
        return pkt;
    } else if (len == -1) {
        if (g_error_matches(error, G_IO_ERROR, G_IO_ERROR_WOULD_BLOCK) ||
            g_error_matches(error, G_IO_ERROR, G_IO_ERROR_AGAIN)) {
            // No packet available
        } else {
            g_warning("Receive error: %s", error->message);
        }
        g_error_free(error);
    }
    return NULL;
}
#endif

static gboolean
net_drv_udp_addr_cmp(net_state_t *net, addr_t *addr1, addr_t *addr2)
{
    addr_data_t *data1 = addr1->drv_data;
    addr_data_t *data2 = addr2->drv_data;

    if (data1 == NULL || data1->addr == NULL)
        return FALSE;

    if (data2 == NULL || data2->addr == NULL)
        return FALSE;

    // Compare GInetAddress and port
    if (!g_inet_address_equal(data1->addr, data2->addr)) {
        return FALSE;
    }
    return data1->port == data2->port;
}

void
net_drv_udp_addr_delete(net_state_t *net, addr_t **dup)
{
    addr_t *addr;
    addr_data_t *data;

    assert(dup != NULL);
    addr = *dup;
    if (addr == NULL) return;

    assert(addr->drv_data != NULL);
    data = addr->drv_data;

    if (data->addr)
        g_object_unref(data->addr);

    g_free(data);

    if (addr->addr)
        g_free(addr->addr);

    g_free(addr);
    *dup = NULL;
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
        new_data->addr = g_object_ref(data->addr);
    }
    new_data->port = data->port;

    /* create our generic address object */
    new = g_new0(addr_t, 1);
    new->drv_data = new_data;
    new->addr = g_strdup(addr->addr);
    new->port = addr->port; // Copy port to generic addr_t as well

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
        new_data->addr = g_object_ref(data->addr);
    if (data->sock) {
        new_data->sock = g_object_ref(data->sock);
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
    if (sock == NULL) return;

    data = sock->drv_data;
    if (data == NULL) {
        g_free(sock);
        *del = NULL;
        return;
    }

    if (data->addr)
        g_object_unref(data->addr);
    if (data->sock)
        g_object_unref(data->sock);

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