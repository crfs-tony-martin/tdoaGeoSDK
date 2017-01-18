/*-----------------------------------------------------------------------------
 * (c) CRFS Limited, 2011. All rights reserved.
 *
 * This software is the property of CRFS Limited and may not be copied or
 * reproduced otherwise than on to a single hard disk for backup or
 * archival purposes. The source code is confidential information and must
 * not be disclosed to third parties or used without the express written
 * permission of CRFS Limited.
 *
 * Filename : tcp.c
 * Author : Stewart Hyde
 *
 * Purpose: TCP connections
 *
 * Description:
 *---------------------------------------------------------------------------*/
#include "tcp.h"

#include <string.h>
#include <fcntl.h>

#ifdef _WIN32
	#include <winsock2.h>
#else
    #include <netdb.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <sys/socket.h>
#endif
#include <errno.h>

#include "crfs_debug.h"

#ifdef _WIN32
/* local variables */
static int32_t tcp_initialized=0;

/**
	Startup WSA TCP  (for win32)
	\return 0 on success, -1 on failure
*/
static int32_t
tcp_initialize(void)
{
	WSADATA wsaDat;
	int32_t error;

	if( (error = WSAStartup(MAKEWORD(2, 2), &wsaDat)) != 0) {
		print_error("WSAStartup failed: %d", error);
		tcp_initialized = 0;
		return -1;
	}
	tcp_initialized = 1;
	return 0;
}
#endif

/**
	Disconnect socket
	\return 0 on success, -1 on failure
*/
int32_t
tcp_disconnect(int32_t sock)
{
#ifdef _WIN32
	if (closesocket(sock) == SOCKET_ERROR) {
		print_error("Winsock failed (%d) to close socket %d",
					 WSAGetLastError(), sock);
		return -1;
	}
#else
	if (close(sock) < 0) {
		print_sys_error("failed to close socket %d", sock);
		return -1;
	}
#endif
	return 0;
}


/**
	Resolve hostname, establish tcp connection and return socket.
    \return socket if OK, else -1
    \todo review WIN section: tidy up error returns
*/
int32_t
tcp_connect(const char *ip_addr_or_host_name, int32_t port, int32_t noblock,
			char *ipaddr, size_t ipaddr_len)
{

#ifdef _WIN32  /* Windows */
	SOCKET Socket;
	SOCKADDR_IN SockAddr;
	struct hostent *host;


	if (!tcp_initialized) {
		if (tcp_initialize() < 0)
			return -4;
	}

	/* Create our socket */
	Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (Socket == INVALID_SOCKET) {
		print_wsa_error("Winsock failed (%d) to create socket",
					 WSAGetLastError());
		return -1;
	}

	/* Resolve IP address for hostname */
	if((host = gethostbyname(ip_addr_or_host_name)) == NULL) {
		print_wsa_error("Winsock failed (%d) to resolve hostname %s",
					 WSAGetLastError(), ip_addr_or_host_name);
		return -2;
	}

	/* Setup our socket address structure */
	SockAddr.sin_port = htons(port);
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);

	/* Attempt to connect to server */
	if (connect(Socket, (struct sockaddr*)(&SockAddr), sizeof(SockAddr)) == SOCKET_ERROR) {
		print_wsa_error("Winsock failed (%d) to establish connection with %s : %d",
					 WSAGetLastError(), ip_addr_or_host_name, port);
		return -3;
	}

	if (noblock) {
		u_long mode = 1;

		if (ioctlsocket(Socket, FIONBIO, &mode) == SOCKET_ERROR) {
			print_wsa_error("Winsock failed (%d) to set FIONBIO ioctl of socket %d",
						 WSAGetLastError(), Socket);
			return -5;
		}
	}

    /** \todo assign resolved_ip_addr in WIN section */
	print_info("TCP connection established, socket:%d, ipaddr:%s\n", Socket,
		inet_ntoa(SockAddr.sin_addr));

	return Socket;
#else  /* not windows */

	int32_t connect_result;
    int sd;
    struct sockaddr_in pin;
    struct hostent *hp;

    /* go find out about the desired host machine */
    if ( (hp = gethostbyname(ip_addr_or_host_name)) == NULL) {
        print_error("Failed to get host by name for %s", ip_addr_or_host_name);
        return -1;
    }

    /* fill in the socket structure with host information */
    memset(&pin, 0, sizeof(pin));
    pin.sin_family = AF_INET;
    pin.sin_addr.s_addr = ((struct in_addr *)(hp->h_addr))->s_addr;
    pin.sin_port = htons(port);

    /* grab an Internet domain socket */
    if ( (sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        print_error("TCP Failed to get TCP socket");
        return -1;
    }

    if (noblock)
        fcntl(sd, F_SETFL, O_NONBLOCK);

    /* connect to PORT on HOST */
    connect_result = connect(sd, (struct sockaddr *)&pin, sizeof(pin));
    if ((connect_result == -1) && !(noblock && (errno==EINPROGRESS))) {
        char str[INET_ADDRSTRLEN];
        print_sys_error("Failed to establish TCP connection[nb=%d] to %s (%s)",
                        noblock,
                        inet_ntop(AF_INET, &pin.sin_addr, str, sizeof(str)),
                        ip_addr_or_host_name);
        return -1;
    }

    inet_ntop(AF_INET, &pin.sin_addr, ipaddr, ipaddr_len);
    print_info("TCP Connection[nb=%d] Established: socket %d, ipaddr %s (%s)",
               noblock, sd, ipaddr, ip_addr_or_host_name);

    return sd;
#endif
}
