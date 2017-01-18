/*-----------------------------------------------------------------------------
 * (c) CRFS Limited, 2011. All rights reserved.
 *
 * This software is the property of CRFS Limited and may not be copied or
 * reproduced otherwise than on to a single hard disk for backup or
 * archival purposes. The source code is confidential information and must
 * not be disclosed to third parties or used without the express written
 * permission of CRFS Limited.
 *
 * Filename : ncp_client.h
 * Author : Stewart Hyde
 *
 * Purpose:  Connect to NCP server
 *
 * Description: Provides functions to establish and maintain an NCP connection
 *    to a NCP server.
 *
 *---------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
/**
    \defgroup   NCP_client NCP Client Library

    \brief      Provides functions for creating & destroying NCP packets
    \brief      Provides functions for establishing and maintaining an NCP connection to a NCP server
    \brief      Provides functions for sending and receiving NCP packets over a TCP conneciton

    \author     Stewart Hyde
    \author     CRFS Ltd

    //          ToDo
    //          *   improved status reporting
    //          *   improved error reporting
    //          *   improved debugging facilities, accessible from Dll
    //          *   testing of jason transmit/recieve
    //          *   full implementation of authentication
    //          *   implementation of ACK packet flag
    //          *   node and intel machines little endian - so NCP little endian
    //              need more robust encoding to allow explicit encoding to little endian if required on big endian machines
    //              (if host is little endian, stick with direct memory access for efficiency at both ends)

*/
/*--------------------------------------------------------------------------*/

#ifndef NCPCLIENT_H_INCLUDED
#define NCPCLIENT_H_INCLUDED

#include <stdint.h>
#include "packets.h"

#ifdef __cplusplus
extern "C" {
#endif
#if 0
}
#endif

#define VERSION_NCP_CLIENT PACKETS_VERSION_STRING


/**
    Structure containing all data assiciated with an NCP client connection
 */
struct ncp_client_connection;
typedef struct ncp_client_connection T_NCP_CLIENT_CONNECTION;
//typedef T_NCP_CLIENT_CONNECTION ncp_client_t;


LIBSPEC T_NCP_CLIENT_CONNECTION* _STDCALL ncp_client_create();
LIBSPEC void _STDCALL ncp_client_free(T_NCP_CLIENT_CONNECTION *ncp_client);

LIBSPEC int32_t _STDCALL ncp_client_connect(T_NCP_CLIENT_CONNECTION *ncp_client,const char *ip_addr_or_host_name, int32_t port,int32_t noblock, int32_t timeout);
LIBSPEC int32_t _STDCALL ncp_client_disconnect(T_NCP_CLIENT_CONNECTION *ncp_client);
LIBSPEC int32_t _STDCALL ncp_client_is_connected(T_NCP_CLIENT_CONNECTION *ncp_client);

LIBSPEC int32_t _STDCALL ncp_client_receive_packet(T_NCP_CLIENT_CONNECTION *ncp_client, T_PACKET *packet);
LIBSPEC int32_t _STDCALL ncp_client_receive(T_NCP_CLIENT_CONNECTION *ncp_client);
LIBSPEC int32_t _STDCALL ncp_client_send(T_NCP_CLIENT_CONNECTION *ncp_client,T_PACKET *packet);

LIBSPEC T_PACKET* _STDCALL ncp_client_get_rx_packet(T_NCP_CLIENT_CONNECTION *ncp_client);
LIBSPEC T_PACKET* _STDCALL ncp_client_get_tx_packet(T_NCP_CLIENT_CONNECTION *ncp_client);
LIBSPEC char* _STDCALL ncp_client_get_hostname(T_NCP_CLIENT_CONNECTION *ncp_client);

LIBSPEC void _STDCALL ncp_client_get_stats(T_NCP_CLIENT_CONNECTION *ncp_client,char *stat_message, int message_size);

LIBSPEC void _STDCALL ncp_client_set_debug_mode(int32_t debug_mode);

extern uint32_t ncp_client_debug_mode;


#if 0
/* static functions, noted here for reference */
static void NCPClientShowconnectionState(T_NCP_CLIENT_CONNECTION *ncp_client);
static void NCPClientSetConnectionState(T_NCP_CLIENT_CONNECTION *ncp_client,
                                        T_NCP_CLIENT_CONNECTION_STATE newConnectionState);
static int32_t NCPClientDecodePacket(T_NCP_CLIENT_CONNECTION *ncp_client);


#endif


#ifdef __cplusplus
}
#endif

#endif /* NCPCLIENT_H_INCLUDED */
