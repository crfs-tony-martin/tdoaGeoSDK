/*-----------------------------------------------------------------------------
 * (c) CRFS Limited, 2011. All rights reserved.
 *
 * This software is the property of CRFS Limited and may not be copied or
 * reproduced otherwise than on to a single hard disk for backup or
 * archival purposes. The source code is confidential information and must
 * not be disclosed to third parties or used without the express written
 * permission of CRFS Limited.
 *
 * Filename : packets_impl.h
 * Author : Dan Timson
 *
 * Purpose: Packet API implementaion details, used by NCP.
 *
 * Description:
 *---------------------------------------------------------------------------*/
#ifndef PACKETS_IMPL_H_INCLUDED
#define PACKETS_IMPL_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef enum
{
    PACKET_INDEX_HEAD = 0,
    PACKET_INDEX_TYPE,
    PACKET_INDEX_SIZE,
    PACKET_INDEX_ID,
    PACKET_INDEX_FORMAT,
    PACKET_INDEX_INFO_1,
    PACKET_INDEX_INFO_2,
    PACKET_INDEX_INFO_3,
    HEADER_SIZE  /* =8 */
} T_PACKET_INDEX;

#define HEADER_BYTES (4 * HEADER_SIZE)
/* sizes in int32's */
#define FOOTER_SIZE 2
#define FIELD_SIZE 3
#define INT_PARAM_SIZE 3

/* extra space on realloc, to reduce calls to realloc */
#define BUFFER_HEADROOM_BYTES 256

#define MIN_PAYLOAD_BYTES 256
/* 4*(hdr + footer) = 'empty' packet, plus some for payload */
#define MIN_BUFFER_SIZE_BYTES (4 * (HEADER_SIZE + FOOTER_SIZE) + MIN_PAYLOAD_BYTES)

/**
    Struct to store information about packet fields and parameters
*/
typedef struct
{
    int32_t name;
    int32_t id;
    int32_t start_position;
    int32_t end_position;
}   T_FIELD_DATA;

typedef struct
{
    int32_t name;
    int32_t data_type;

    int32_t start_position;
    int32_t end_position;
#if 0
    void *data_ptr; /**< pointer into packet buffer, at start of param data */
#endif
    size_t data_len;

    int32_t read;
}   T_PARAM_DATA;

struct packet
{
    int32_t packet_buffer_size; /**< size of buffer in bytes */
    int32_t *packet_buffer; /**< pointer to buffer */

    int32_t state; /**< state of the packet, type T_PACKET_STATE */
    int32_t rx_byte_position; /**< byte offset into buffer between calls to packet_receive */

    int32_t read_ok;
    int32_t position;

    int32_t write_field_id;
    int32_t write_position_tag;
    int32_t num_reallocs;

    T_FIELD_DATA *field_data; /**< dynamic array for fields in the packet */
    int32_t field_size; /**< current size of packet_field_data array */
    int32_t field_count; /**< current number of actual fields in packet */
    int32_t field_index; /**< index to current field in field_data */

    T_PARAM_DATA *param_data; /**< dynamic array for parameters in a field */
	int32_t param_size; /**< current size of field_param_data array */
    int32_t param_count; /**< current number of actual params in the field */
    int32_t param_index; /**< index to current param in param_data */
};

#define PACKET_HEAD_VALUE 0xAABBCCDD
#define PACKET_FORMAT_VALUE 1
#define PACKET_FOOT_VALUE 0xDDCCBBAA

#if 0
int32_t packet_send(T_PACKET *packet, int32_t sock);
int32_t packet_receive(T_PACKET *packet, int32_t sock);
#endif

#ifdef __cplusplus
}
#endif

#endif /* PACKETS_IMPL_H_INCLUDED */
