/*
 * Name: net_drv.c
 * Description: Networking Driver API
 *              Handles transmitting & receiving reliable and unreliable packets over UDP
 *              Also handles recording packet stream to a log file (if NET_VCR is defined)
 *
 * Copyright (c) 2012 Jason Hall <jbhall78@gmail.com>
 */
#ifdef SCS_ENGINE
#include <glib.h>
#include <string.h>

#include "mem.h"
#include "scs.h"
#include "shared.h"
#include "net.h"
#else
#include "megalib.h"
#endif

/* 0 = no debugging
 * 1 = connection info, latency info, dropped packets, error conditions
 * 2 = send/recv packets info
 * 3 = queueing/processing info
 * 4 = stress test (drop random packets)
 */
#define NET_DEBUG 1

net_state_t *
net_init(net_drv_t *drv, clock_driver_t *clock_driver, void (*pkt_cb)(conn_t *, pkt_t *))
{
    net_state_t *net = g_new0(net_state_t, 1);

    net->drv = drv;
    net->pkt_cb = pkt_cb;
    net->clock = clock_driver;

    printf("initializing network driver\n");
    if (NET_DEBUG >= 4)
	printf("NOTE: (random packet dropping enabled)\n");
    net->drv->init(net);

    return net;
}

void
net_shutdown(net_state_t *net)
{
    GSList *l;
    conn_t *conn;

    /* get rid of any connections */
    for (l = net->connections; l; l = l->next) {
	conn = l->data;

	/* XXX - why is this null? */
	if (conn)
	    net_conn_delete(net, &conn);
    }
    g_slist_free(net->connections);

    net->drv->shutdown(net); 

    g_free(net);
}

conn_t *
net_connect(net_state_t *net, char *address, uint16_t port, GError **err)
{
    conn_t *conn;
    pkt_t *pkt;
    int32_t version = SCS_NET_VER;

#ifdef NET_VCR
    GError *tmp = NULL;
    if (net->recording) {
	if (! net_vcr_open(net, TRUE, NULL, &tmp)) {
	    g_propagate_error(err, tmp);
	    return NULL;
	}
    }
#endif

    conn = net->drv->connect(net, address, port);

    net->mode = NET_MODE_CLIENT;

    /* send the connection request */
    pkt = net_pkt_new(PKT_CONN_REQ, TRUE);
    net_pkt_pack_int32(pkt, 1, &version);
    net_pkt_pack_str(pkt, strlen(SCS_NET_GAME)+1, SCS_NET_GAME);

    net_send(net, conn, pkt);

    return conn;
}

void
net_disconnect(net_state_t *net, conn_t *conn)
{
    pkt_t *pkt = net_pkt_new(PKT_CONN_DISCONNECT, FALSE);
    net_send(net, conn, pkt);

    net->drv->disconnect(net, conn);

#ifdef NET_VCR
    if (net->recording)
	net_vcr_close(net);
#endif
}

sock_t *
net_bind(net_state_t *net, char *address, GError **err)
{
    GError *tmp = NULL;

    net->sock = net->drv->bind(net, address, &tmp);

    if (! net->sock)
	g_propagate_error(err, tmp);

    net->mode = NET_MODE_SERVER;

    return net->sock;
}

void
net_unbind(net_state_t *net)
{
    net->drv->unbind(net);
}

void
net_send(net_state_t *net, conn_t *conn, pkt_t *pkt)
{
    pkt->session_id = conn->session_id;

    if (pkt->reliable) {
	if (pkt->type == PKT_CONN_REQ) {
	    // generate a random sequence number for the initial connection request
	    uint32_t seq = g_random_int();
	    pkt->sequence = seq;
	    seq++;
	    conn->tx_reliable_seq = seq;
	    conn->rx_reliable_seq = seq;
	    conn->tx_unreliable_seq = seq;
	    conn->rx_unreliable_seq = seq;
	} else if (pkt->type != PKT_CONN_ACCEPT)
	    pkt->sequence = conn->tx_reliable_seq++;

#if (NET_DEBUG >= 3)
	printf("[%c]     queued   reliable packet: [type:0x%04x] [seq:0x%08x] [sid:0x%08x]\n",
		is_client__new() ? 'c' : 's',
		pkt->type, pkt->sequence, pkt->session_id);
#endif
	g_queue_push_tail(conn->reliable_sendq, pkt);
    } else {
	if (pkt->type != PKT_ACK)
	    pkt->sequence = conn->tx_unreliable_seq++;

	net->drv->send(net, conn, pkt);
#if (NET_DEBUG >= 2)
	if (pkt->type != PKT_OBJ_UPDATE && pkt->type != PKT_PING && pkt->type != PKT_PONG) {
	    printf("[%c]    sending unreliable packet: [type:0x%04x] [seq:0x%08x] [sid:0x%08x]\n", 
		    is_client__new() ? 'c' : 's', 
		    pkt->type, pkt->sequence, pkt->session_id);
	}
#endif
	/* free unreliable packets immediately */
	net_pkt_delete(pkt);
    }
}

void
net_send_all(net_state_t *net, pkt_t *pkt)
{
    GSList *l;
    conn_t *conn;
    pkt_t *dup;

    for (l = net->connections; l; l = l->next) {
	conn = l->data;

	if (conn->sock->state != STATE_ESTABLISHED && conn->sock->state != STATE_CONNECTING) {
	    printf("skipping: [0x%08x]\n", conn->session_id);
	    continue;
	}

	dup = net_pkt_dup(net, pkt);
	net_send(net, conn, dup);
    }

    net_pkt_delete(pkt);
}

void
net_conn_delete(net_state_t *net, conn_t **del)
{
    pkt_t *pkt;
    conn_t *conn;

    assert(del != NULL);
   
    conn = *del;

    /* clean out packets in send queue */
    while (! g_queue_is_empty(conn->reliable_sendq)) {
	pkt = g_queue_pop_head(conn->reliable_sendq);
	net_pkt_delete(pkt);
    }
    g_queue_free(conn->reliable_sendq);

    net_sock_delete(net, &conn->sock);
    net_addr_delete(net, &conn->addr);

    /* remove from the connections list */
    net->connections = g_slist_remove(net->connections, conn);

    g_free(conn);

    *del = NULL;
}

addr_t *
net_addr_dup(net_state_t *net, addr_t *addr)
{
    return net->drv->addr_dup(net, addr);
}

gboolean
net_addr_cmp(net_state_t *net, addr_t *addr1, addr_t *addr2)
{
    if (addr1 == NULL || addr2 == NULL)
	return FALSE;
    else
	return net->drv->addr_cmp(net, addr1, addr2);
}

sock_t *
net_sock_dup(net_state_t *net, sock_t *sock)
{
    return net->drv->sock_dup(net, sock);
}

void
net_sock_delete(net_state_t *net, sock_t **sock)
{
    net->drv->sock_delete(net, sock);
}

void
net_addr_delete(net_state_t *net, addr_t **addr)
{
    net->drv->addr_delete(net, addr);
}

static conn_t *
net_find_conn(net_state_t *net, pkt_t *pkt)
{
    GSList *l;
    conn_t *conn;

    for (l = net->connections; l; l = l->next) {
	conn = l->data;

#if 0
	if (pkt->session_id == 0) {
	    if (net_addr_cmp(net, pkt->from, conn->addr)) {
		return conn;
	    }
	} else {
#endif
	    if (net_addr_cmp(net, pkt->from, conn->addr) &&
		    pkt->session_id == conn->session_id) {
		return conn;
	    }
#if 0
	}
#endif
    }

    return NULL;
}

static void
net_proc_ack(net_state_t *net, conn_t *conn, pkt_t *pkt)
{
    pkt_t *sent;

    /* make sure we have a connection */
    if (conn == NULL && net->sock->state == STATE_CONNECTING)
	conn = net->connections->data;
    else if (conn == NULL) {
	printf("ack received with no associated connection\n");
	return;
    }

    /* make sure we get an ack from the same host */
    if (! net_addr_cmp(net, conn->addr, pkt->from)) {
	printf("ack received from an invalid address\n");
	return;
    }

    sent = g_queue_peek_head(conn->reliable_sendq);
    if (! sent) {
	printf("duplicate (probably) ack received.. dropping\n");
	return;
    }

    if (sent->sequence == pkt->sequence) {
	/* remove the packet we sent from the sendq,
	   open transmitting reliable packets back up again and
	   free the original sent packet */
	g_queue_pop_head(conn->reliable_sendq);
	conn->last_reliable_xmit = 0;

	/* free the packet here because we way retransmit */
	net_pkt_delete(sent);
    } else  
	printf("out of sequence ack received, ignoring.\n");	

    return;
}

static void
net_cl_handle_accept_packet(net_state_t *net, pkt_t *pkt)
{
    conn_t *conn;
    pkt_t *ack, *reg;

    if (net->sock->state != STATE_CONNECTING) {
	printf("connection accept on a non-connecting socket\n");
	return;
    }

    /* this should be the only connection we have */
    conn = net->connections->data;

    conn->sock->state = STATE_ESTABLISHED;
    conn->session_id = pkt->session_id;
    /* use the server provided sequence number now */
    conn->tx_reliable_seq = pkt->sequence;
    conn->rx_reliable_seq = pkt->sequence;
    conn->tx_unreliable_seq = pkt->sequence;
    conn->rx_unreliable_seq = pkt->sequence;
    
    /* send acknowledgement */
    ack = net_pkt_new(PKT_ACK, FALSE);
    ack->sequence = pkt->sequence;
    net_send(net, conn, ack);
    
    /* send our registration packet */
    reg = net_pkt_new(PKT_CONN_REGISTER, TRUE);
    net_pkt_pack_str(reg, strlen("player")+1, "player");
    net_send(net, conn, reg);
}

void
net_sv_handle_request_packet(net_state_t *net, pkt_t *pkt)
{
    uint16_t len;
    int32_t version;
    char *game;
    conn_t *conn;
    pkt_t *accept, *ack;
    uint32_t seq;
    GSList *l;

    assert(net != NULL);
    assert(net->sock != NULL);

    if (net->sock->state != STATE_LISTEN) {
	printf("connection request on a non-listening socket\n");
	return;
    }

    net_pkt_unpack_int32(pkt, 1, &version);
    net_pkt_unpack_str(pkt, &len, &game);

    /* XXX - send reject packets */
    if (version != SCS_NET_VER) {
	printf("unrecognized version %d from: %s\n", version, pkt->from->addr);
	goto end;
    }
    if (strcmp(game, SCS_NET_GAME) != 0) {
	printf("unknown game %s from: %s\n", game, pkt->from->addr);
	goto end;
    }

    /* ensure only one connecting connection per host */
    for (l = net->connections; l; l = l->next) {
	conn = l->data;

	if (net_addr_cmp(net, conn->addr, pkt->from) &&
		conn->sock->state == STATE_CONNECTING) {
	    printf("duplicate connection request from host\n");
	    goto end;
	}
    }

    print("connection request: [version: %d] [game: %s] [peer: %s]\n", version, game, pkt->from->addr);

    /* create a new connection */
    seq = g_random_int();

    conn = g_new0(conn_t, 1);
    conn->sock = net_sock_dup(net, net->sock);
    conn->sock->state = STATE_CONNECTING;
    conn->addr = net_addr_dup(net, pkt->from);
    conn->session_id = g_random_int();

    conn->tx_reliable_seq = seq;
    conn->rx_reliable_seq = seq;
    conn->tx_unreliable_seq = seq;
    conn->rx_unreliable_seq = seq;
    conn->last_reliable_xmit = 0;
    conn->last_tx_ping = 0;
    conn->last_rx_ping = 0;
    conn->reliable_sendq = g_queue_new();

    /* register the connection -
       next we can run the packet through the normal handlers */
    net->connections = g_slist_prepend(net->connections, conn);
    
#if (NET_DEBUG >= 1)
    printf("connection request: %d, %s from %s accepted. session: [0x%08x]\n",
	    version, game, pkt->from->addr, conn->session_id);
#endif

    /* send acknowledgement of connection request */
    ack = net_pkt_new(PKT_ACK, FALSE);
    ack->sequence = pkt->sequence;	/* use the clients sequence */
    net_send(net, conn, ack);

    /* send connection accept */
    accept = net_pkt_new(PKT_CONN_ACCEPT, TRUE);
    accept->sequence = seq;		/* set the initial sequence */
    net_send(net, conn, accept);

end:
    mem_free(game);
}

// process packets before a conn_t structure is created for the link
// these are packets in stage 1 & 2 of the 3 way handshake
static void
net_handle_pre_connection_packets(net_state_t *net, pkt_t *pkt)
{
    if (net->mode == NET_MODE_SERVER) {
	// stage 1
	switch (pkt->type) {
	    case PKT_CONN_REQ:
		net_sv_handle_request_packet(net, pkt);
		break;
	}
    } else {
	// stage 2
	switch (pkt->type) {
	    case PKT_CONN_ACCEPT:
		net_cl_handle_accept_packet(net, pkt);
		break;
	}
    }
}

// handle packets in stage 3 of the 3 way handshake
// a conn_t object has been created at this point
static void
net_finalize_connection(net_state_t *net, conn_t *conn, pkt_t *pkt)
{
    switch (pkt->type) {
	case PKT_CONN_REGISTER:
	    conn->sock->state = STATE_ESTABLISHED;	/* remove equiv action from sv_pkt.c */

	    // send this packet to the upper levels so authentication can be done
	    // the user of the code can shut down the connection if authentication fails
#if (NET_DEBUG >= 3)
	printf("[%c] processing %s packet: [type:0x%04x] [seq:0x%08x] [sid:0x%08x]\n",
		is_client__new() ? 'c' : 's',
		pkt->reliable ? "  reliable" : "unreliable",
		pkt->type, pkt->sequence, pkt->session_id);
#endif
	    net->pkt_cb(conn, pkt);
	    break;
    }
}

// automatically handle ping & pong packets to detect connection failure transparently
//
// BUG: resolution needs to be increased to nanoseconds
static void
net_handle_ping_packets(net_state_t *net, conn_t *conn, pkt_t *pkt)
{
    int32_t tm;
    pkt_t *pong;

    if (pkt->type == PKT_PING) {
	/* reply with a pong packet */
	net_pkt_unpack_int32(pkt, 1, &tm);
	pong = net_pkt_new(PKT_PONG, FALSE);
	net_pkt_pack_int32(pong, 1, &tm);
	
	net_send(net, conn, pong);
    } else if (pkt->type == PKT_PONG) {
	/* update the connections ping time */
	net_pkt_unpack_int32(pkt, 1, &tm);
	conn->last_rx_ping = net->clock->ticks();
	conn->last_tx_ping = 0;
	conn->latency = conn->last_rx_ping - tm;
#if (NET_DEBUG >= 1)
	{
	    static int x = 0;

	    /* rate limit the messages */
	    if (++x == 5) {
		printf("[%c] latency update: [sid:0x%08x] - %ums\n",
			is_client__new() ? 'c' : 's',
			conn->session_id, conn->latency);
		x = 0;
	    }
	}
#endif
    }
}

void
net_recv_pkts(net_state_t *net)
{
    pkt_t *pkt;
    conn_t *conn;
    sock_t *sock = net->sock;

    while ((pkt = net->drv->recv(net, sock)) != NULL) {
	/* record the packet if we are recording */
#ifdef NET_VCR
	if (net->recording) 
	    net_vcr_write(net, pkt, NULL);
#endif

#if (NET_DEBUG >= 4)
	// drop a few packets for debugging 
	if (! (g_random_int() % 100))
	    continue;
#endif
#if (NET_DEBUG >= 2)
	if (pkt->type != PKT_OBJ_UPDATE && pkt->type != PKT_PING && pkt->type != PKT_PONG) {
	    printf("[%c]   received %s packet: [type:0x%04x] [seq:0x%08x] [sid:0x%08x]\n",
		    is_client__new() ? 'c' : 's',
		    pkt->reliable ? "  reliable" : "unreliable",
		    pkt->type, pkt->sequence, pkt->session_id);
	}
#endif
	conn = net_find_conn(net, pkt);

	/* process acks even if we dont have a connect yet */
	if (pkt->type == PKT_ACK) {
	    net_proc_ack(net, conn, pkt);
	    goto next_packet;
	}

	/* handle pre connection setup */
	if (! conn) {
	    net_handle_pre_connection_packets(net, pkt);
	    goto next_packet;
	}

	/* drop out of sequence packets */
	if (pkt->reliable) {
	    if (pkt->sequence > conn->rx_reliable_seq) {
		// we did not receive packets leading up to this one
		// so drop the packet and wait for a retransmit
		//
		// later we may want to save it in a queue, so we don't
		// have to wait for the retransmitted packet
#if (NET_DEBUG >= 1)
		printf("dropping reliable packet: got %u, wanted %u\n",
			pkt->sequence, conn->rx_reliable_seq);
#endif
		goto next_packet;
	    } else if (pkt->sequence <= conn->rx_reliable_seq) {
		// send or resend acknowledgement
		pkt_t *ack = net_pkt_new(PKT_ACK, FALSE);
#if (NET_DEBUG >= 2)
		printf("[%c]    sending   acknowledgement: [type:0x%04x] [seq:0x%08x] [sid:0x%08x]\n",
			is_client__new() ? 'c' : 's',
			pkt->type, pkt->sequence, pkt->session_id);
#endif
		ack->sequence = pkt->sequence;
		net_send(net, conn, ack);

		// if the sequence is less than what we expect, we are retransmitting the ack
		// otherwise we are doing the initial acknowledgement and should increment the
		// sequence
		if (pkt->sequence == conn->rx_reliable_seq)
		    conn->rx_reliable_seq++;
		else { 
		    // we have already processed the packet, so don't process it again
		    goto next_packet;
		}
	    }
#if 0 /* old code, works mostly */
	    if (pkt->sequence != conn->rx_reliable_seq) {
#if (NET_DEBUG >= 1)
		printf("dropping reliable packet: got %u, wanted %u\n",
			pkt->sequence, conn->rx_reliable_seq);
#endif
		continue;
	    } else {
		pkt_t *ack = net_pkt_new(net, PKT_ACK, FALSE);
#if (NET_DEBUG >= 2)
		printf("[%c]  sending   acknowledgement: [type:0x%04x] [seq:0x%08x] [sid:0x%08x]\n",
			is_client__new() ? 'c' : 's',
			pkt->type, pkt->sequence, pkt->session_id);
#endif
		ack->sequence = pkt->sequence;
		net_send(net, conn, ack);
		conn->rx_reliable_seq++;
	    }
#endif
	} else {
	    /* JH - what happens if the packet when the sequence wraps
	             is dropped? */
	    if (pkt->sequence >= conn->rx_unreliable_seq) {
		if (pkt->sequence > conn->rx_unreliable_seq)
#if (NET_DEBUG >= 1)
		    printf("[%c]   received unreliable packet with sequence: [0x%08x] expected: [0x%08x] (ok)\n",
			    is_client__new() ? 'c' : 's',
			    pkt->sequence, conn->rx_unreliable_seq);
#endif
		// make this packet the most recent and process it
	    	conn->rx_unreliable_seq = pkt->sequence + 1;
    	    } else {
#if (NET_DEBUG >= 1)
		printf("[%c]   received unreliable packet with sequence: [0x%08x] expected: [0x%08x] delta %u (dropping)\n", pkt->sequence, 
			is_client__new() ? 'c' : 's',
			conn->rx_unreliable_seq, 
			conn->rx_unreliable_seq - pkt->sequence);
#endif
		goto next_packet;
	    }
	}

	if (conn->sock->state != STATE_ESTABLISHED) {
	    net_finalize_connection(net, conn, pkt);
	    goto next_packet;
	}

	/* dont let ping/pong packets hit the upper levels either */
	if (pkt->type == PKT_PING || pkt->type == PKT_PONG) {
	    net_handle_ping_packets(net, conn, pkt);
	    goto next_packet;
	}

#if (NET_DEBUG >= 3)
	printf("[%c] processing %s packet: [type:0x%04x] [seq:0x%08x] [sid:0x%08x]\n",
		is_client__new() ? 'c' : 's',
		pkt->reliable ? "  reliable" : "unreliable",
		pkt->type, pkt->sequence, pkt->session_id);
#endif

	/* send the packet to the net event handlers */
	net->pkt_cb(conn, pkt);

next_packet:
	net_pkt_delete(pkt);
    }
}

void
net_send_pkts(net_state_t *net)
{
    pkt_t *pkt;
    conn_t *conn;
    GSList *l; 
    uint32_t ticks;

    for (l = net->connections; l; l = l->next) {
	conn = l->data;
	ticks = net->clock->ticks();

	/* send a ping if needed */
	if (conn->sock->state == STATE_ESTABLISHED) {
	    /* first check how long they have been idle */
	    if (conn->last_rx_ping != 0 && ticks - conn->last_rx_ping > 30000) {
		/* disconnect after 10 seconds */
		pkt = net_pkt_new(PKT_CONN_PING_TIMEOUT, FALSE);
		pkt->session_id = conn->session_id;

		/* send the packet to upper layers so they can
		   do the cleanup.
		
                   NOTE: this can mess with the connections list so 
                         break here and we'll process the rest of the
                         packets next time
		 */
		net->pkt_cb(conn, pkt);
		break;
	    }
	    /* idle time hasnt been hit, lets see if we need 
	       to send a ping */
	    if (ticks - conn->last_rx_ping > 2000 &&
		ticks - conn->last_tx_ping > 300) {
		pkt = net_pkt_new(PKT_PING, FALSE);
		net_pkt_pack_uint32(pkt, 1, &ticks);
		
		net_send(net, conn, pkt);
		conn->last_tx_ping = ticks;
	    }
	}

	/* process reliable packets */
	if (! g_queue_is_empty(conn->reliable_sendq)) {
	    // this needs to be converted into a loop but i am going to wait until
	    // i switch over to my own data types before i do this.
	    // it might be difficult to traverse a g_queue structure without
	    // modifying it
	    // making it a loop will allow us to attempt to send multiple
	    // reliable packets per frame (which could considerably improve performance)


    	    /* transmit initial packet */
	    if (conn->last_reliable_xmit > 0) {
		/* we have already sent the packet once, wait for our retry
		   time */
		if (ticks - conn->last_reliable_xmit > 300) {
		    pkt = g_queue_peek_head(conn->reliable_sendq);
#if (NET_DEBUG >= 1)
		    printf("[%c]    sending   reliable packet: [type:0x%04x] [seq:0x%08x] [sid:0x%08x] (retransmit)\n",
			    is_client__new() ? 'c' : 's',
			    pkt->type, pkt->sequence, pkt->session_id);
#endif
		    net->drv->send(net, conn, pkt);
		    conn->last_reliable_xmit = ticks;
		}
	    } else {
		pkt = g_queue_peek_head(conn->reliable_sendq);
#if (NET_DEBUG >= 4)
		if (g_random_int() % 2)
		    continue;
#endif
		net->drv->send(net, conn, pkt);
		conn->last_reliable_xmit = ticks;
#if (NET_DEBUG >= 2)
       		if (pkt->type != PKT_OBJ_UPDATE) {
		    printf("[%c]    sending   reliable packet: [type:0x%04x] [seq:0x%08x] [sid:0x%08x]\n",
			    is_client__new() ? 'c' : 's',
			    pkt->type, pkt->sequence, pkt->session_id);
		}
#endif
	    }
	}
    }

    /* don't delete the reliable packet now, wait for ack */
}
