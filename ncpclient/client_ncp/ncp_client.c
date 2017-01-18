/*-----------------------------------------------------------------------------
 * (c) CRFS Limited, 2011. All rights reserved.
 *
 * This software is the property of CRFS Limited and may not be copied or
 * reproduced otherwise than on to a single hard disk for backup or
 * archival purposes. The source code is confidential information and must
 * not be disclosed to third parties or used without the express written
 * permission of CRFS Limited.
 *
 * Filename : ncp_client.c
 * Author : Stewart Hyde
 *
 * Purpose:  Connect to NCP server
 *
 * Description: Provides functions to establish and maintain an NCP connection
 *    to a NCP server.
 *
 *---------------------------------------------------------------------------*/
#include "ncp_client.h"
#include "ncp_client_impl.h"

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#ifndef _WIN32
	#include <unistd.h>
#endif

#include "packets.h"
#include "crfs_debug.h"
#include "ncp_link_packets.h"
#include "tcp.h"
#include "utils.h"

#define VERBOSE 0




/* local data */
/**
    String translation for enumerated ncp client connection states.
    Used for printing messages indicating the surrent connection state
 */
static char *NCPClientConnectionStates[]={
                            "NCPClient_IllegalState",
                            "NCPClient_NoTCPConnection",
                            "NCPClient_TCPConnectionEstablished",
                            "NCPClient_ReceivedGreeting",
                            "NCPClient_SentClientConnectRequest",
                            "NCPClient_ReceivedAuthenticationRequest",
                            "NCPClient_SentAuthenticationResponse",
                            "NCPClient_ReceivedAuthenticationBad",
                            "NCPClient_ReceivedAuthenticationOk",
                            "NCPClient_ReceivedConnectionAccept",
                            "NCPClient_ReceivedConnectionReject",
                            "NCPClient_ConnectionAccepted",
                            "NCPClient_ConnectionRejected",
                            "NCPClient_TerminateConnection",
                            "NCPClient_DisconnectMessageSent",
                            "NCPClient_ConnectionActive"
                            };

/* local prototypes */
static void NCPClientSetConnectionState(T_NCP_CLIENT_CONNECTION *ncp_client, T_NCP_CLIENT_CONNECTION_STATE newConnectionState);

/* external functions */

/**
    Constructor for NCP client connection.
    Allocates memory and initializes structure.
    Currently allocates minimum packet size: this
    can grow dynamically.
	?? _ Uses prefixed header "NCP{" and footer "}NCP"
 */
T_NCP_CLIENT_CONNECTION* _STDCALL
ncp_client_create()
{
  /*    signal (SIGPIPE, catch_sigpipe); */
    T_NCP_CLIENT_CONNECTION *self;

    self = (T_NCP_CLIENT_CONNECTION*)Malloc(sizeof(T_NCP_CLIENT_CONNECTION));
    if (self == NULL)
        return self;
    memset(self, 0, sizeof(T_NCP_CLIENT_CONNECTION));

    self->socket = -1;

    strcpy(self->ClientIdentification,"Client Identification Word");
	char name[64];
    size_t size = 0;
    if (gethostname(name, size) == 0)
        strcpy(self->ClientIdentification,name);
    strcpy(self->ClientAuthentication,"Client Authentication Word");
    NCPClientSetConnectionState(self, NCPClient_NoTCPConnection);
    self->tx_packet = packet_create("", 0);
    self->rx_packet = packet_create("", 0);
    return self;
}


/**
    Destructor for NCP client connection.
    Frees associated memory.
 */
void _STDCALL
ncp_client_free(T_NCP_CLIENT_CONNECTION *ncp_client)
{
    if (ncp_client == NULL)
        return;
    packet_free(ncp_client->tx_packet);
	packet_free(ncp_client->rx_packet);
    Free(ncp_client);
}

/**
    Returns the client rx_packet.
 */
T_PACKET* _STDCALL
ncp_client_get_rx_packet(T_NCP_CLIENT_CONNECTION *ncp_client)
{
    return ncp_client->rx_packet;
}

/**
    Returns the client tx_packet.
 */
T_PACKET* _STDCALL
ncp_client_get_tx_packet(T_NCP_CLIENT_CONNECTION *ncp_client)
{
    return ncp_client->tx_packet;
}

/**
   Get pointer to hostname string.
*/
char* _STDCALL
ncp_client_get_hostname(T_NCP_CLIENT_CONNECTION *ncp_client)
{
    return ncp_client->hostname;
}

/**
    Show a message associated with a socket connection.
 */
static void
NCPClientShowconnectionState(T_NCP_CLIENT_CONNECTION *ncp_client)
{
#if VERBOSE
    print_info("Socket %d: Connection State:%s",
                ncp_client->socket,
                NCPClientConnectionStates[ncp_client->connectionstate]);
#endif
}

/**
    Keep track of NCP client connection state.
    An NCP client must negotiate with an NCP server in order to establish a connection.
    A question and response authentication sequence must be successfully negotiated in
    order to achieve an active connection.
    This routine manages the connection state and initiates actions associated with entering
    a particular state.
 */
static void
NCPClientSetConnectionState(T_NCP_CLIENT_CONNECTION *ncp_client, T_NCP_CLIENT_CONNECTION_STATE newConnectionState)
{
    T_PACKET *txpacket = ncp_client->tx_packet;

    ncp_client->connectionstate = newConnectionState;
    NCPClientShowconnectionState(ncp_client);
    switch (ncp_client->connectionstate)
    {
        case NCPClient_NoTCPConnection:
            ncp_client->testdata=0;
            break;
        case NCPClient_TCPConnectionEstablished:
            break;
        case NCPClient_ReceivedGreeting:
            packet_write(txpacket, PACKET_TYPE_LINK, -1);
            packet_add_field(txpacket, LINK_FIELD_CLIENT_CONN_REQ, 0);
            packet_add_param_string(txpacket, LINK_PARAM_CLIENT_ID,ncp_client->ClientIdentification);
            packet_send(txpacket, ncp_client->socket);
            NCPClientSetConnectionState(ncp_client, NCPClient_SentClientConnectRequest);
            break;
        case NCPClient_ReceivedAuthenticationRequest:
            packet_write(txpacket, PACKET_TYPE_LINK, -1);
            packet_add_field(txpacket, LINK_FIELD_CLIENT_AUTH_RESP, 0);
            packet_add_param_string(txpacket, LINK_PARAM_CLIENT_AUTH,ncp_client->ClientAuthentication);
            packet_send(txpacket, ncp_client->socket);
            NCPClientSetConnectionState(ncp_client, NCPClient_SentAuthenticationResponse);
            break;
        case NCPClient_ReceivedAuthenticationOk:
            NCPClientSetConnectionState(ncp_client, NCPClient_ConnectionAccepted);
            break;
        case NCPClient_ConnectionAccepted:
            NCPClientSetConnectionState(ncp_client, NCPClient_ConnectionActive);
            break;
        case NCPClient_SentAuthenticationResponse:
            break;
        case NCPClient_SentClientConnectRequest:
            break;
        case NCPClient_ConnectionActive:
            break;
        case NCPClient_TerminateConnection:
            packet_write(txpacket, PACKET_TYPE_LINK, -1);
            packet_add_field(txpacket, LINK_FIELD_TERM_REQ, 0);
            packet_add_param_string(txpacket, LINK_PARAM_REASON, "Client disconnect");
            packet_send(txpacket, ncp_client->socket);
            NCPClientSetConnectionState(ncp_client, NCPClient_DisconnectMessageSent);
            break;
        case NCPClient_DisconnectMessageSent:
            NCPClientSetConnectionState(ncp_client, NCPClient_NoTCPConnection);
            break;
        default:
            break;
    }
}



/**
    Decode a received NCP packet and act accordingly.
 */
static int32_t
NCPClientDecodePacket(T_NCP_CLIENT_CONNECTION *ncp_client)
{
    int32_t DecodeOK=0;
    int32_t packet_type;
    int32_t fname, fid;

    T_PACKET *packet = ncp_client->rx_packet;

    packet_type = packet_read(packet);
    if (packet_type == PACKET_TYPE_LINK) {
        while (packet_get_next_field(packet, &fname, &fid)) {

            if (fname == LINK_FIELD_SERVER_GREETING) {
                packet_get_param_string(packet, LINK_PARAM_SERVER_ID, ncp_client->server_id, 64);
                packet_get_param_string(packet, LINK_PARAM_NCP_VERSION,
                                        ncp_client->server_ncp_version, 64);
                packet_get_param_string(packet, LINK_PARAM_SERVER_MAC,
                                        ncp_client->server_mac_addr, 64);
#if VERBOSE
                sprintf(ncp_client->statusmessage, "Server Greeting Received:\"%s\"",
                        ncp_client->server_id);
                printf("Socket %d: %s\n", ncp_client->socket,ncp_client->statusmessage);
#endif
                NCPClientSetConnectionState(ncp_client, NCPClient_ReceivedGreeting);

            } else if (fname == LINK_FIELD_SERVER_AUTH_REQ) {
                packet_get_param_string(packet, LINK_PARAM_SERVER_AUTH,
                                        ncp_client->ServerAuthentication, 64);
#if VERBOSE
                sprintf(mbuf, "Server Authentication Request Received:\"%s\"",
                        ncp_client->ServerAuthentication);
                printf("Socket %d: %s\n", ncp_client->socket, mbuf);
#endif
                NCPClientSetConnectionState(ncp_client, NCPClient_ReceivedAuthenticationRequest);

            } else if (fname == LINK_FIELD_SERVER_CONFIRM) {
                packet_get_param_string(packet, LINK_PARAM_SERVER_ID2, ncp_client->server_id, 64);
#if VERBOSE
                sprintf(mbuf, "Server Authentication Confirmed, Serverid:\"%s\"",
                        ncp_client->server_id);
                printf("Socket %d: %s\n",ncp_client->socket,mbuf);
#endif
                NCPClientSetConnectionState(ncp_client, NCPClient_ReceivedAuthenticationOk);
            }
        }
    }
    return DecodeOK;
}

/**
    Receive bytes from a TCP connection and decode when a complete NCP packet is buffered.
    Monitor received bitrate.
 */
int32_t _STDCALL
ncp_client_receive(T_NCP_CLIENT_CONNECTION *ncp_client)
{
	return ncp_client_receive_packet(ncp_client, NULL);
}
int32_t _STDCALL
ncp_client_receive_packet(T_NCP_CLIENT_CONNECTION *ncp_client, T_PACKET *packet)
{
    double now;
    double timed;
	int32_t packetcomplete=0;
	int32_t res;

    now = get_time_double();

	if (packet == NULL)
		packet = ncp_client->rx_packet;

    /** \todo review packet status settings here */
	/*if (ncp_client->non_blocking_connection==0)
      packet_set_status(ncp_client->rx_packet, PACKET_STATUS_PACKET_COMPLETE);*/

	for (;;) {
		res = packet_receive(packet, ncp_client->socket);

		if (res < 0) {
            print_error("ncp_client_receive: packet_receive error = %d", res);
            break; /* return ? */
        }

        if (res > 0) {
			NCPClientDecodePacket(ncp_client);

            /** \todo put in separate update_rxstats function */
			ncp_client->rxbytecount += packet_get_packet_size(ncp_client->rx_packet) * 4;
			ncp_client->rxpacketcount++;
			timed = now - ncp_client->rxstarttime;
			if (timed > 1) {
				ncp_client->rx_bitrate_bps = (int)floor(ncp_client->rxbytecount*8.0/timed);
				ncp_client->rx_packetrate_ppmin = (int)floor(ncp_client->rxpacketcount*60.0/timed);
				ncp_client->rxbytecount = 0;
				ncp_client->rxpacketcount = 0;
				ncp_client->rxstarttime = now;
			}

			packetcomplete = 1;
            break;
        }

		if (ncp_client->non_blocking_connection == 1)
            break;
	}

    /** \todo remove the following keep alive tx */
	if ((ncp_client->connectionstate == NCPClient_ConnectionActive))
	{
		if (now-ncp_client->last_tx_time>1)
		{
			packet_write(ncp_client->tx_packet,PACKET_TYPE_LINK,-1);
			packet_write_complete(ncp_client->tx_packet);
			res=ncp_client_send(ncp_client,ncp_client->tx_packet);
			if (res==-1)
			{
				NCPClientSetConnectionState(ncp_client,NCPClient_NoTCPConnection);
				return -1;
			}
		}
	}
	else packetcomplete=-1;



	return packetcomplete;
}


/**
    Create a sting showing client conneciton status and statistics
    \todo tidy up string handling
 */
void _STDCALL
ncp_client_get_stats(T_NCP_CLIENT_CONNECTION *ncp_client,char *stat_message, int message_size)
{
    char mbuf[1024];
    double now;

    now = get_time_double();
	/*
//#ifdef _GCC
//	int total_mem=getpagesize()*get_phys_pages();
//	int mem_avail=getpagesize()*get_avphys_pages();
//	int mem_used=total_mem-mem_avail;
//#endif
*/
    if (ncp_client->connectionstate==NCPClient_ConnectionActive)
    {
        snprintf(stat_message, message_size, "NCP client:%s on socket %d\n", ncp_client->hostname, ncp_client->socket);
		snprintf(mbuf, 1024, "%s,  connected for %g seconds\n",NCPClientConnectionStates[ncp_client->connectionstate],(double) floor(now-ncp_client->connectiontime));
        strncat(stat_message, mbuf, message_size - strlen(stat_message) - 1);
        snprintf(mbuf, 1024, "Rx bit rate:%.3fMb/s, Rx packet rate:%.1fpkt/s\n",ncp_client->rx_bitrate_bps/1000000.0,ncp_client->rx_packetrate_ppmin/60.0);
        strncat(stat_message, mbuf, message_size - strlen(stat_message) - 1);
        snprintf(mbuf, 1024, "Tx bit rate:%.3fMb/s, Tx packet rate:%.1fpkt/s\n",ncp_client->tx_bitrate_bps/1000000.0,ncp_client->tx_packetrate_ppmin/60.0);
        strncat(stat_message, mbuf, message_size - strlen(stat_message) - 1);
        snprintf(mbuf, 1024, "rx byte count:%d  tx byte count:%d\n",rxbytecount,txbytecount);
        strncat(stat_message, mbuf, message_size - strlen(stat_message) - 1);
        snprintf(mbuf, 1024, "rx_packets  good:%d, bad:%d\n",good_rx_packets,bad_rx_packets);
        strncat(stat_message, mbuf, message_size - strlen(stat_message) - 1);
        snprintf(mbuf, 1024, "rx_packets  incomplete:%d, bad_header:%d\n",incomplete_rx_packets,rx_bad_headers);
        strncat(stat_message, mbuf, message_size - strlen(stat_message) - 1);
    }
    else
    {
        snprintf(stat_message, message_size, "Connection State:%s\n",NCPClientConnectionStates[ncp_client->connectionstate]);
    }
}


/**
    Print client connection status and statistics
 */
void ncp_client_print_status(T_NCP_CLIENT_CONNECTION *ncp_client)
{
    char mbuf[4096];
    ncp_client_get_stats(ncp_client,mbuf, 4096);
    printf("%s\n",mbuf);
}

/**
    Send a packet to a TCP connection.
    \todo check state (must be connected)
 */
int32_t _STDCALL
ncp_client_send(T_NCP_CLIENT_CONNECTION *ncp_client,T_PACKET *packet)
{
    double now;
    double timed;
    int32_t res;

    res = packet_send(packet,ncp_client->socket);

    now = get_time_double();
    ncp_client->last_tx_time=now;
    ncp_client->txbytecount += packet_get_packet_size(packet) * 4;
    ncp_client->txpacketcount++;
    timed=now-ncp_client->txstarttime;
    if (timed>1)
    {
        ncp_client->tx_bitrate_bps=(int)(ncp_client->txbytecount*8.0/timed);
        ncp_client->tx_packetrate_ppmin=(int)(ncp_client->txpacketcount*60.0/timed);
        ncp_client->txbytecount=0;
        ncp_client->txpacketcount=0;
        ncp_client->txstarttime=now;
    }

    return res;
}

/**
    Establish NCP connection with NCP server.
 */
int32_t _STDCALL
ncp_client_connect(T_NCP_CLIENT_CONNECTION *ncp_client, const char *ip_addr_or_host_name,
                   int32_t port,int32_t noblock, int32_t timeout)
{
    double now;
	int32_t sd;

    ncp_client->non_blocking_connection = noblock;
	sd = tcp_connect(ip_addr_or_host_name, port, noblock, ncp_client->ipaddr, INET_ADDRSTRLEN);
	if (sd > 0) {
        /* update ncp_client */
        now = get_time_double();
	    ncp_client->connectiontime = now;
	    ncp_client->rxstarttime = now;
	    ncp_client->txstarttime = now;
	    ncp_client->socket = sd;
        strncpy(ncp_client->hostname, ip_addr_or_host_name, sizeof(ncp_client->hostname));
        NCPClientSetConnectionState(ncp_client, NCPClient_TCPConnectionEstablished);

        /* rx */
        for (;;) {
			ncp_client_receive(ncp_client);
            if (ncp_client->connectionstate==NCPClient_ConnectionActive) break;
            now = get_time_double();
			if (timeout>0)
            {
                if ((now-ncp_client->connectiontime)>timeout)break;
			}
        }
        if (ncp_client->connectionstate!=NCPClient_ConnectionActive)
        {
            snprintf(ncp_client->statusmessage, 255, "Timed out waiting for NCP connection, socket:%d\n",sd);
            NCPClientSetConnectionState(ncp_client,NCPClient_NoTCPConnection);
            //printf("%s",ncp_client->statusmessage);
            return 0;
        }
        snprintf(ncp_client->statusmessage, 255, "NCP Connection Established, socket:%d",sd);
    }
    return sd;
}

/**
    Terminate NCP client conneciton and clean up.
 */
int32_t _STDCALL
ncp_client_disconnect(T_NCP_CLIENT_CONNECTION *ncp_client)
{
    NCPClientSetConnectionState(ncp_client, NCPClient_TerminateConnection);
    tcp_disconnect(ncp_client->socket);
    print_info("ncp_disconnect: closed socket %d ", ncp_client->socket);
    return 1;
}

/** \return 1 if connected, else 0 */
int32_t _STDCALL
ncp_client_is_connected(T_NCP_CLIENT_CONNECTION *ncp_client)
{
    return ncp_client->connectionstate == NCPClient_ConnectionActive;
}
