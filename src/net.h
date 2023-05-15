#ifndef _NET_H
#define _NET_H

#include <stdio.h>

#include <glib.h>

#include "mathlib.h"

#include "dbuf.h"
#include "clock.h"

#define SCS_NET_VER  0x01
#define SCS_NET_GAME "scs"
#define SCS_NET_PORT 31337

typedef enum {
    MSG_ACK            = 0x00,
    MSG_SHUTDOWN       = 0x01,
    MSG_PKT            = 0x02,
    MSG_ERROR          = 0x03,
} thread_msg_code_t;

typedef struct {
    thread_msg_code_t code;
    gpointer data;
} thread_msg_t;

/*
Handshake Documentation:

Similar to TCP 3 way handshake, however, we send acknowledgements in seperate packets for simplicity

        (c to s) 0x0101 {reliable} (PKT_CONN_REQ) [cl_sock_state = CONNECTING]
        (s to c) 0x0001 {sort of reliable*} (PKT_ACK) [conns[client]->sock_state = CONNECTING]
        (s to c) 0x0102 (PKT_CONN_ACCEPT)
        (c to s) 0x0105 (PKT_CONN_REGISTER) [cl_sock_state = ESTABLISHED]
        (s to c) 0x0106 (PKT_CONN_REGISTERED) [conns[client]->sock_state = CONNECTING]

* acknowledgements are resent for reliable packets received with a lower sequence number than the current sequence
        (I think I tried to make acknowledgements reliable at one point and it screwed things up)
*/

//typedef enum {	/* allow API users to create their own packet codes */
#define PKT_ACK                0x001
#define PKT_PING	       0x002
#define PKT_PONG	       0x003

#define PKT_CONN_REQ		0x101
#define PKT_CONN_ACCEPT		0x102
#define PKT_CONN_REJECT		0x103
#define PKT_CONN_DISCONNECT	0x104
#define PKT_CONN_REGISTER	0x105
#define PKT_CONN_REGISTERED	0x106
#define PKT_CONN_PING_TIMEOUT	0x107

#define PKT_CONSOLE_MSG		0x201

#define PKT_PLAYER_VELOCITIES	0x301
#define PKT_PLAYER_ATTACK	0x302
#define PKT_PLAYER_WARP		0x303

#define PKT_OBJ_SPAWN		0x401
#define PKT_OBJ_FRAG		0x402
#define PKT_OBJ_UPDATE		0x403

#define PKT_SND_SPAWN		0x501
#define PKT_SND_UPDATE		0x502
#define PKT_SND_PLAY		0x503
#define PKT_SND_STOP		0x504
#define PKT_SND_FRAG		0x505

//} pkt_type_t;

#if 0
typedef enum {
    PKT_STR    = 0x01,  /* 8bit char */
    PKT_INT    = 0x04,	/* 32bit integer */
    PKT_REAL   = 0x08,  /* 64bit float */
} pkt_chunk_type_t;
#endif 
#define PKT_STR sizeof(char)
#define PKT_INT sizeof(int32_t)
#define PKT_REAL sizeof(real)

typedef struct {
//    pkt_chunk_type_t type;
    uint8_t type;
    uint16_t len;
    void *data;
} pkt_chunk_t;

#define PKT_FLAG_RELIABLE 0x01

typedef enum {
    STATE_DISCONNECTED = 0x01,
    STATE_CONNECTING   = 0x02,
    STATE_ESTABLISHED  = 0x03,
    STATE_LISTEN       = 0x04,
} sock_state_t;

typedef struct {
    sock_state_t state;
    void *drv_data;
} sock_t;

typedef struct {
    char *addr;
    void *drv_data;
} addr_t;

typedef struct {
    uint32_t session_id;
    uint32_t tx_reliable_seq;
    uint32_t rx_reliable_seq;
    uint32_t tx_unreliable_seq;
    uint32_t rx_unreliable_seq;
    GQueue *reliable_sendq;
    uint32_t last_reliable_xmit;

    uint32_t last_tx_ping;
    uint32_t last_rx_ping;
    uint16_t latency;

    sock_t *sock;
    addr_t *addr;
} conn_t;




#define ETHERNET_MTU 1500
#define ETHERNET_OVERHEAD 42
#define IPV4_OVERHEAD 20 
#define IPV6_OVERHEAD 48
#define UDP_OVERHEAD 8

#define PKT_HEADER_SIZE (2 + 4 + 4 + 1 + 5) // 5 bytes padding for total of 16
#define MAX_PACKET_SIZE (ETHERNET_MTU - PKT_HEADER_SIZE - IPV6_OVERHEAD - UDP_OVERHEAD)
typedef struct {
    // header values
    uint16_t type;
    uint32_t session_id;
    uint32_t sequence;
    uint8_t reliable;

    // addresses
    addr_t *from;
    addr_t *to;

    // data
    dbuf_t *dbuf;	// data buffer which stores the packet contents
} pkt_t;

/* dont use dynamic memory here so we can save to file */
typedef struct {
    char magic[5];
    uint8_t version;
    char player[16];
    char desc[32];
    uint32_t creation;
    uint32_t start_ticks;
} net_vcr_hdr_t;

/* dont use dynamic memory here so we can save to file */
typedef struct {
    uint32_t ticks;
    uint16_t len;
} pkt_vcr_hdr_t;

#define OBJ_PHYS_NORM	0x00
#define OBJ_PHYS_LASER	0x01
#define OBJ_PHYS_MISSLE	0x02
#define OBJ_PHYS_NONE   0x03	/* position/velocity not affected by "gravity" */

#if 0
typedef struct {
    uint32_t id;
    uint8_t phys;
    uint32_t frame;
    vec3_t pos;
    quat_t orient;
    vec3_t posv, rotv;	/* velocities */
    m_mesh_t *mesh;
    gboolean notarget;
    uint32_t target_id;
} object_t;

typedef struct {
    int8_t id;
    conn_t *conn;
    object_t *obj;
    gboolean active;
    gboolean privledged;
    char *name;
    uint8_t team;
    int32_t score;
    time_t connect_time;
} player_t;
#endif

#define is_client__new() ((net->mode == NET_MODE_CLIENT) ? true : false)

typedef struct {
    struct net_drv_s *drv;
    void (*pkt_cb)(conn_t *, pkt_t *);
    GSList *connections;
    sock_t *sock;

    enum {
	NET_MODE_UNKNOWN = 0,
	NET_MODE_CLIENT = 1,
	NET_MODE_SERVER = 2
    } mode;

    /* demo recording stuff */
    gboolean recording;
    char *filename;
    FILE *demo;
    uint32_t tick_offset;
    clock_driver_t *clock;
} net_state_t;

typedef struct net_drv_s {
    void (*init)(net_state_t *);
    void (*shutdown)(net_state_t *);
    conn_t *(*connect)(net_state_t *, char *address, uint16_t port);
    void (*disconnect)(net_state_t *, conn_t *conn);
    sock_t *(*bind)(net_state_t *, char *address, GError **);
    void (*unbind)(net_state_t *);
    void (*send)(net_state_t *, conn_t *conn, pkt_t *);
    pkt_t *(*recv)(net_state_t *, sock_t *);
    gboolean (*addr_cmp)(net_state_t *, addr_t *, addr_t *);
    void (*addr_delete)(net_state_t *, addr_t **);
    addr_t *(*addr_dup)(net_state_t *, addr_t *);
    sock_t *(*sock_dup)(net_state_t *, sock_t *);
    void (*sock_delete)(net_state_t *, sock_t **);
} net_drv_t;

extern net_drv_t net_drv_udp;
extern net_drv_t net_drv_async;
extern net_drv_t net_drv_vcr;

void net_recv_pkts(net_state_t *net);
void net_send_pkts(net_state_t *net);

net_state_t *net_init(net_drv_t *drv, clock_driver_t *clock_driver, void (*pkt_cb)(conn_t *, pkt_t *));
void net_shutdown(net_state_t *net);
conn_t *net_connect(net_state_t *net, char *address, uint16_t port, GError **err);
void net_disconnect(net_state_t *net, conn_t *conn);
sock_t *net_bind(net_state_t *net, char *address, GError **err);
void net_unbind(net_state_t *net);
void net_send(net_state_t *net, conn_t *conn, pkt_t *pkt);
void net_send_all(net_state_t *net, pkt_t *pkt);
gboolean net_addr_cmp(net_state_t *net, addr_t *addr1, addr_t *addr2);
sock_t *net_sock_dup(net_state_t *net, sock_t *sock);
void net_sock_delete(net_state_t *net, sock_t **sock);
void net_conn_delete(net_state_t *net, conn_t **conn);

void *net_pkt_pack(pkt_t *pkt, uint32_t *len);
pkt_t *net_pkt_unpack(void *buf, uint32_t len);
void net_pkt_pack_int8(pkt_t *pkt, uint16_t count, int8_t *val);
void net_pkt_pack_int32(pkt_t *pkt, uint16_t count, int32_t *val);
void net_pkt_pack_uint32(pkt_t *pkt, uint16_t count, uint32_t *val);
void net_pkt_pack_real(pkt_t *pkt, uint16_t count, real *val);
void net_pkt_pack_str(pkt_t *pkt, uint16_t count, char *val);
//int32_t *net_pkt_unpack_int32(pkt_t *pkt, uint16_t *len);
void net_pkt_unpack_int8(pkt_t *pkt, uint16_t count, int8_t *val);
void net_pkt_unpack_int32(pkt_t *pkt, uint16_t count, int32_t *val);
void net_pkt_unpack_uint32(pkt_t *pkt, uint16_t count, uint32_t *val);
void net_pkt_unpack_str(pkt_t *pkt, uint16_t *len, char **val);
void net_pkt_unpack_real(pkt_t *pkt, uint16_t count, real *val);
pkt_t *net_pkt_new(uint16_t type, bool reliable);
pkt_t *net_pkt_dup(net_state_t *net, pkt_t *pkt);
void net_pkt_delete(pkt_t *pkt);
conn_t *net_conn_new(void);
void net_addr_delete(net_state_t *net, addr_t **addr);
addr_t *net_addr_dup(net_state_t *net, addr_t *addr);

/* net vcr functions */

gboolean net_vcr_open(net_state_t *, gboolean, char *, GError **);
void net_vcr_close(net_state_t *net);
gboolean net_vcr_write(net_state_t *net, pkt_t *pkt, GError **err);

#endif
