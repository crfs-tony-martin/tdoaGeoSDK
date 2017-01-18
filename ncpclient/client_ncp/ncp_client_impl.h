/*-----------------------------------------------------------------------------
 * (c) CRFS Limited, 2011. All rights reserved.
 *
 * This software is the property of CRFS Limited and may not be copied or
 * reproduced otherwise than on to a single hard disk for backup or
 * archival purposes. The source code is confidential information and must
 * not be disclosed to third parties or used without the express written
 * permission of CRFS Limited.
 *
 * Filename : ncp_client_impl.h
 * Author : Dan Timson
 *
 * Purpose: NCP client connection API implementaion details.
 *
 * Description:
 *---------------------------------------------------------------------------*/
#ifndef NCP_CLIENT_IMPL_H_INCLUDED
#define NCP_CLIENT_IMPL_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#ifdef _WIN32
    #define INET_ADDRSTRLEN 16
#else
    #include <netdb.h>
#endif

/**
	Client connection states. Used to track status of ncp client connection.
*/
typedef enum t_ncp_client_connection_state
{
    NCPClient_IllegalState,
    NCPClient_NoTCPConnection,
    NCPClient_TCPConnectionEstablished,
    NCPClient_ReceivedGreeting,
    NCPClient_SentClientConnectRequest,
    NCPClient_ReceivedAuthenticationRequest,
    NCPClient_SentAuthenticationResponse,
    NCPClient_ReceivedAuthenticationBad,
    NCPClient_ReceivedAuthenticationOk,
    NCPClient_ReceivedConnectionAccept,
    NCPClient_ReceivedConnectionReject,
    NCPClient_ConnectionAccepted,
    NCPClient_ConnectionRejected,
    NCPClient_TerminateConnection,
    NCPClient_DisconnectMessageSent,
    NCPClient_ConnectionActive
} T_NCP_CLIENT_CONNECTION_STATE;

/**
    Structure containing all data assiciated with an NCP client connection
 */
struct ncp_client_connection
{
    int32_t socket;                 /**< socket descriptor associated with client the connection */
    char hostname[64];              /**< hostname associated with the connection */
    char ipaddr[INET_ADDRSTRLEN];   /**< ip address associated with the connection */
    char ServerAuthentication[64];  /**< Server authentication word received on establishing connection */
    char ClientIdentification[64];  /**< Client identification word sent on establishing connection */
    char ClientAuthentication[64];  /**< Client authentication word sent on establishing connection */
    char server_id[64];             /**< server identification information */
    char server_mac_addr[64];       /**< server MAC address */
    char server_ncp_version[64];    /**< server NCP version */
    int32_t testdata;               /**< test data loop back size */
    int32_t rx_bitrate_bps;
    int32_t rx_packetrate_ppmin;
    int32_t rxbytecount;
    int32_t rxpacketcount;
    int32_t tx_bitrate_bps;
    int32_t tx_packetrate_ppmin;
    int32_t txbytecount;
    int32_t txpacketcount;
    double connectiontime;
    int32_t non_blocking_connection;
    double txstarttime;             /**< time stamp used for timing measurements */
    double last_tx_time;            /**< time stamp used for timing measurements */
    double rxstarttime;             /**< time stamp used for timing measurements */
    char statusmessage[255];        /**< human readable connection status message */
    T_PACKET *rx_packet;            /**< tx packet used for sending information to the server */
    T_PACKET *tx_packet;            /**< rx packet used for receiving information from the server */
    T_NCP_CLIENT_CONNECTION_STATE connectionstate;  /**< ncp client connection state */
};

#ifdef __cplusplus
}
#endif

#endif /* NCP_CLIENT_IMPL_H_INCLUDED */
