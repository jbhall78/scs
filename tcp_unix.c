#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#include <netdb.h>	/* getprotobyname */

#include <sys/socket.h>		/* tcp,bind */
#include <netinet/in.h>		/* tcp */
#include <netinet/tcp.h>	/* tcp */

#include <sys/types.h>          /* bind */
#include <sys/socket.h>

#define TRUE 1
#define FALSE 0

int
tcp_connect(char *hostname, char *service)
{
    struct protoent *pent = getprotobyname("tcp");
    int protocol = pent->p_proto;

    int ret, fd;
    struct addrinfo hints;
    struct addrinfo *result, *rp;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family   = AF_UNSPEC;    /* AF_INET,AF_INET6, AF_UNSPEC */
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags    = 0;
    hints.ai_protocol = protocol;

    ret = getaddrinfo(hostname, service, &hints, &result);
    if (ret != 0) {
        fprintf(stderr, "getaddrinfo: %s:%s: %s\n", hostname, service, gai_strerror(ret));
        exit(EXIT_FAILURE);
    }

    // loop through addresses and try to connect to each one
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);

        if (fd == -1)
            continue;

        if (connect(fd, rp->ai_addr, rp->ai_addrlen) != -1)
            break;

        close(fd);
    }

    if (rp == NULL) {               /* No address succeeded */
        fprintf(stderr, "Unable to connect to %s:%s: %s\n", hostname, service, strerror(errno));
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(result);           /* No longer needed */

    // set to nonblocking I/O
/*
    if ((ret = fcntl(fd, F_SETFL, O_NONBLOCK)) < 0) {
	fprintf(stderr, "fcntl: %s\n", strerror(errno)); 
    }
*/

    return fd;
}

