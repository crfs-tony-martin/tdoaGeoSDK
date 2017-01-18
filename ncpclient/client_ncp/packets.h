/*-----------------------------------------------------------------------------
 * (c) CRFS Limited, 2011. All rights reserved.
 *
 * This software is the property of CRFS Limited and may not be copied or
 * reproduced otherwise than on to a single hard disk for backup or
 * archival purposes. The source code is confidential information and must
 * not be disclosed to third parties or used without the express written
 * permission of CRFS Limited.
 *
 * Filename : packets.h
 * Author : Stewart Hyde
 *
 * Purpose: NCP packet API.
 *
 * Description:
 *---------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
/**
    \defgroup   Generic_Packets Generic Packets

    \brief      Generic packet processing routines.
    \brief      Functions for creating and destroying generic packets.
    \brief      Functions for loading and unloading data from a packet.
    \brief      Functions for displaying packet content in human readable form.
    \brief      Functions for sending and receiving packets on a TCP connection.

    \author     Stewart Hyde
    \author     Dan Timson
    \author     CRFS Ltd

    //          ToDo:-
    //          * packet changes for source identification and more efficient encoding of ints and bools
    //          * boolean type
    //          * automatic packet resync
    //          * non blocking socket not working correctly in windows

    @{
*/
/*--------------------------------------------------------------------------*/

#ifndef PACKETS_H_INCLUDED
#define PACKETS_H_INCLUDED


#ifdef __cplusplus
extern "C" {
#endif
#if 0
}
#endif

#include <stdint.h>
#include <stdio.h>

#define PACKETS_VERSION_STRING "CRFS NCP client version: 0.9.5 (build 1)"
#define VERSION_PACKETS PACKETS_VERSION_STRING

// These macros work with the MS Visual Studio 2010 projects in client_ncp/bindings -
// please do not change them.
#ifdef _WIN32
    #ifdef _USRDLL
        //#define _STDCALL	__stdcall
        #define _STDCALL
        #ifdef NCP_CLIENT_M_IMPORTS
            #define LIBSPEC __declspec(dllimport)
        #else
            #define LIBSPEC __declspec(dllexport)
        #endif
    #else
        #define LIBSPEC
        #define _STDCALL
    #endif
#else
    #define LIBSPEC
    #define _STDCALL
#endif


/**
   Packet state
*/
typedef enum {
    PACKET_STATE_CREATED       = 1, /**< freshly created, no header or type */
    PACKET_STATE_WRITING       = 2, /**< header written, ready for fields */
    PACKET_STATE_FIELD_WRITTEN = 3, /**< able to accept parameter addition */
    PACKET_STATE_COMPLETE      = 4, /**< writing, freading, rx: checked */
    PACKET_STATE_RX_HDR_OK     = 5,
    PACKET_STATE_RX_ERROR      = 6,
    PACKET_STATE_FREAD_ERROR   = 7
} T_PACKET_STATE;

/* error codes */
#define ERROR_NONE 0
/* #define E_GENERAL  1 */
#define E_RECV     2
#define E_BADHDR   3
#define E_BADFOOT  4

/**
    Packet field parameter data types
*/
#include "ncp_param_types.h"
typedef enum param_type T_PARAM_DATA_TYPE;

#define PARAM_IS_SCALAR(X) (((X) & 0x80) == 0)
#define PKEY *(int*) /* casts string to int32 key */

/**
    Struct to hold all associated information and data for a packet.
*/
/* Incomplete data type to point to, representing a packet. */
struct packet;
typedef struct packet T_PACKET;

LIBSPEC char* _STDCALL packet_key_to_str(int32_t key, char *str);
LIBSPEC int32_t _STDCALL packet_str_to_key(char *str);

LIBSPEC T_PACKET* _STDCALL packet_create(char *name, int32_t buffer_size);
LIBSPEC void _STDCALL packet_free(T_PACKET *packet);
#ifndef EXCLUDE_PACKET_CLONE
LIBSPEC T_PACKET* _STDCALL packet_clone(T_PACKET *packet);
#endif
LIBSPEC int32_t _STDCALL packet_get_packet_type(T_PACKET *packet);
LIBSPEC void _STDCALL packet_set_packet_type(T_PACKET *packet, int packet_type);
LIBSPEC int32_t _STDCALL packet_get_packet_size(T_PACKET *packet);
LIBSPEC int32_t _STDCALL packet_get_packet_id(T_PACKET *packet);
LIBSPEC int32_t _STDCALL packet_get_packet_format(T_PACKET *packet);
LIBSPEC int32_t _STDCALL packet_get_packet_sec(T_PACKET *packet);
LIBSPEC int32_t _STDCALL packet_get_packet_nanosec(T_PACKET *packet);

LIBSPEC void _STDCALL packet_write(T_PACKET *packet, int32_t packet_type, int32_t packet_identifier);
LIBSPEC int32_t _STDCALL packet_append(T_PACKET *packet);
LIBSPEC int32_t _STDCALL packet_add_field(T_PACKET *packet, int32_t name, int32_t field_id);
LIBSPEC int32_t _STDCALL packet_add_param_int(T_PACKET *packet,int32_t name, int32_t data32);
LIBSPEC int32_t _STDCALL packet_add_param_string(T_PACKET *packet, int32_t name, const char *string_to_add);
LIBSPEC int32_t _STDCALL packet_add_param_data(T_PACKET *packet, int32_t name, T_PARAM_DATA_TYPE data_type, const void *data, int32_t length);
LIBSPEC void* _STDCALL packet_add_param_data_get_ptr(T_PACKET *packet, int32_t name,
                                                     T_PARAM_DATA_TYPE data_type, int32_t length);
LIBSPEC void _STDCALL packet_write_complete(T_PACKET *packet);

LIBSPEC int32_t _STDCALL packet_read(T_PACKET *packet);
LIBSPEC int32_t _STDCALL packet_get_num_fields(T_PACKET *packet);
LIBSPEC int32_t _STDCALL packet_get_next_field(T_PACKET *packet,int32_t *name, int32_t *field_identifier);
LIBSPEC int32_t _STDCALL packet_get_num_params(T_PACKET *packet);
LIBSPEC int32_t _STDCALL packet_get_next_param(T_PACKET *packet, int32_t *name, T_PARAM_DATA_TYPE *typ,
                                            void **data, int32_t *len);
LIBSPEC int32_t _STDCALL packet_get_named_param(T_PACKET *packet, int32_t name, T_PARAM_DATA_TYPE *typ,
                                             void **data_pointer, int32_t *len);

LIBSPEC int32_t _STDCALL packet_get_int_param_def(T_PACKET *packet,int32_t name, int32_t default_val);
LIBSPEC int32_t _STDCALL packet_get_int_param_spec3(T_PACKET *packet, int32_t name,
                      int32_t default_val, int32_t lower_limit, int32_t upper_limit);
LIBSPEC char* _STDCALL packet_get_string_param_def(T_PACKET *packet,int32_t name, char *default_val);

#define INCLUDE_DEPRECATED
#ifdef INCLUDE_DEPRECATED
LIBSPEC int32_t _STDCALL packet_get_param_int(T_PACKET *packet,int32_t name, int32_t *data32);
LIBSPEC int32_t _STDCALL packet_get_param_string(T_PACKET *packet,int32_t name, char *str, int32_t max_length);
LIBSPEC int32_t _STDCALL packet_get_param_data(T_PACKET *packet,int32_t name,T_PARAM_DATA_TYPE *data_type, void *raw_data, int32_t max_length);
LIBSPEC int32_t _STDCALL packet_access_param(T_PACKET *packet, int32_t name, T_PARAM_DATA_TYPE *datatype, void **dataPointer);
#endif
LIBSPEC void _STDCALL packet_read_complete(T_PACKET *packet);

LIBSPEC int32_t _STDCALL packet_get_unread_field_params(T_PACKET *packet);
LIBSPEC int32_t _STDCALL packet_get_unread_field_params_ack(T_PACKET *packet, T_PACKET *ack_packet);

LIBSPEC char *_STDCALL NCP_version();

#ifdef EXPORT_DIRECT_TXRX
#define MY_LIBSPEC LIBSPEC
#define MY_STDCALL _STDCALL
#else /* not exported, but stil used by ncp_client */
#define MY_LIBSPEC
#define MY_STDCALL
#endif
MY_LIBSPEC int32_t MY_STDCALL packet_send(T_PACKET *packet, int32_t sock);
MY_LIBSPEC int32_t MY_STDCALL packet_receive(T_PACKET *packet, int32_t sock);
#ifndef  _WIN32
MY_LIBSPEC int32_t MY_STDCALL peek_packet(int sock, int *packet_type);
#endif
/* file access */
/* simple wrapper for stdio fopen, fclose, fflush */
LIBSPEC FILE* _STDCALL file_open_read(const char *filename);
LIBSPEC FILE* _STDCALL file_open_write(const char *filename);
LIBSPEC int32_t _STDCALL file_close(FILE *stream);
LIBSPEC int32_t _STDCALL file_flush(FILE *stream);
/* write and read packets to/from files */
LIBSPEC int32_t _STDCALL packet_file_write(T_PACKET *packet, FILE *stream);
LIBSPEC int32_t _STDCALL packet_file_read(T_PACKET *packet, FILE *stream);

LIBSPEC void _STDCALL packet_print(T_PACKET *packet);
LIBSPEC char* _STDCALL json_create_from_packet(T_PACKET *packet);
LIBSPEC char* _STDCALL json_full_create_from_packet(T_PACKET *packet);
LIBSPEC void _STDCALL json_free(char *json);
LIBSPEC void _STDCALL packet_set_debug_mode(uint32_t debug_mode);

extern int32_t debug_tx_bytes;
extern int32_t debug_rx_bytes;

extern int32_t force_no_link_packets;
extern int32_t force_debug_show_tx_packets;
extern int32_t force_debug_show_rx_packets;
extern int32_t debug_show_tx_packets;
extern int32_t debug_show_rx_packets;

extern int32_t rxbytecount;
extern int32_t txbytecount;

extern int32_t bad_rx_packets;
extern int32_t good_rx_packets;
extern int32_t incomplete_rx_packets;
extern int32_t rx_bad_headers;

extern int32_t packet_receive_call_count;

extern int32_t debug_network_echo;

    /** @} */
#ifdef __cplusplus
}
#endif

#endif /* PACKETS_H_INCLUDED */
