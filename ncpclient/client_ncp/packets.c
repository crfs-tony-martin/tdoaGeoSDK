/*-----------------------------------------------------------------------------
 * (c) CRFS Limited, 2011. All rights reserved.
 *
 * This software is the property of CRFS Limited and may not be copied or
 * reproduced otherwise than on to a single hard disk for backup or
 * archival purposes. The source code is confidential information and must
 * not be disclosed to third parties or used without the express written
 * permission of CRFS Limited.
 *
 * Filename : packets.c
 * Author : Stewart Hyde
 *
 * Purpose: NCP packet API.
 *
 * Description:
 *---------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <fcntl.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <errno.h>

#ifdef _WIN32
	#ifndef __BORLANDC__
		//#include <sys/time.h>
	#endif
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <sys/time.h>
    #include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
#endif

#include "packets.h"
#include "packets_impl.h"
#include "ncp_packets.h"
#include "crfs_debug.h"
#include "utils.h"

/* debug macros */
/* method print function, etc */
//#define DEBUG_REALLOC
#ifdef DEBUG_REALLOC
#define MPRINT(P,T) do {                                                \
        if (!P || (P->num_reallocs > 0)) {                             \
            print_info("%s %s(packet=%p, line=%d)", T, __FUNCTION__, P, __LINE__); \
        }                                                               \
    } while (0)
#else
#define MPRINT(P,T)
#endif
/* method header & footer */
#define MHDR(P) MPRINT(P,"entering")
#define MMID(P) MPRINT(P,"  inside")
#define MFTR(P) MPRINT(P," leaving")

/* global variables */
int debug_network_echo=0;
int doecho=0;

int32_t debug_tx_bytes=0;
int32_t debug_rx_bytes=0;
int32_t force_no_link_packets=0;
int32_t force_debug_show_rx_packets=0;
int32_t force_debug_show_tx_packets=0;
int32_t debug_show_tx_packets=0;
int32_t debug_show_rx_packets=0;

int32_t rxbytecount=0;
int32_t txbytecount=0;

int32_t bad_rx_packets=0;
int32_t good_rx_packets=0;
int32_t incomplete_rx_packets=0;
int32_t rx_bad_headers=0;
int32_t packet_receive_call_count=0;

/* local variables */
static uint32_t packet_debug_mode=0;

/* local prototypes */
static char*packet_param_data_type_to_string(T_PARAM_DATA_TYPE param_data_type);
static int32_t packet_scan_fields(T_PACKET *packet);
static int32_t packet_scan_params(T_PACKET *packet, int32_t field_number);
static int32_t packet_check_buffer_size(T_PACKET *packet, int32_t required_size);
static int32_t resize(void **ptr, int32_t new_size);
static int32_t packet_receive_header(T_PACKET *packet, int32_t sock);
static int type_scale_fac(T_PARAM_DATA_TYPE typ);
static void packet_print_type_warning(const T_PACKET *packet, int32_t atyp, int32_t etyp,
                                      int32_t param_name);
static int32_t _STDCALL packet_get_named_typed_param(T_PACKET *packet, int32_t name,
                                              T_PARAM_DATA_TYPE typ,
                                              void **data_pointer, int32_t *len);

/****************************************************************/
/* API function definitions */
/****************************************************************/

/****************************************************************/
/* 1. Utilities */
/****************************************************************/

char* _STDCALL
NCP_version()
{
    return PACKETS_VERSION_STRING;
}

/**
   Set mode for debug output. Uses global variable.
 */
void _STDCALL
packet_set_debug_mode(uint32_t debug_mode)
{
    packet_debug_mode=debug_mode;
}

/**
   Create a string from a 32bit integer packet key.
   \param[in] key
   \param[in] str pointer to char array of length >= 5. If = NULL,
   a static string is used (non-reentrant)
   \return str
 */
char* _STDCALL
packet_key_to_str(int32_t key, char *str)
{
    static char s_str[5];
    char i;

    if (str == NULL)
        str = s_str;

    i = key & 0x7F;
    str[0] = isprint(i) ? i : '#';

    i = (key & 0x7F00) >> 8;
    str[1] = isprint(i) ? i : '#';

    i = (key & 0x7F0000) >> 16;
    str[2] = isprint(i) ? i : '#';

    i = (key & 0x7F000000) >> 24;
    str[3] = isprint(i) ? i : '#';

    str[4] = 0;

    return str;
}
char *pks(int32_t key)
{
    return packet_key_to_str(key, NULL);
}

/**
   Create a 32bit integer packet key from a string.
 */
int32_t _STDCALL
packet_str_to_key(char *str)
{
	return PKEY(str);
}

/****************************************************************/
/* 2. Constructors & accessors */
/****************************************************************/

/**
    Create a packet. Allocate memory and initialize variables.
    \param[in] name ignored
    \param[in] buffer_size The size in bytes of the packet_buffer to be allocated.
    Should be large enough to contain all data (header, fields, parameters, footer)
    expected to be written, received or loaded.
    If it is less than the minimum, it gets silently increased to a value suitable
    for small packets.
    \returns Pointer to the packet data structure, or NULL on failure.
    \pre None.
    \post Packet is in CREATED state (if not NULL).
 */
T_PACKET* _STDCALL
packet_create(char *name, int32_t buffer_size)
{
    T_PACKET *self;

    if (buffer_size < MIN_BUFFER_SIZE_BYTES)
        buffer_size = MIN_BUFFER_SIZE_BYTES;

    if ( (self = (T_PACKET*)Malloc(sizeof(T_PACKET))) == NULL)
        return NULL;
    MMID(self);
    memset(self, 0, sizeof(T_PACKET));

    if ( (self->packet_buffer = (int32_t*)Malloc(buffer_size)) == NULL) {
        Free(self);
        return NULL;
    }
    memset(self->packet_buffer, 0, buffer_size);
    self->packet_buffer_size = buffer_size;

    self->packet_buffer[PACKET_INDEX_HEAD] = PACKET_HEAD_VALUE;
    self->packet_buffer[PACKET_INDEX_FORMAT] = PACKET_FORMAT_VALUE;
    self->state = PACKET_STATE_CREATED;

    MFTR(self);
    return self;
}
#ifndef EXCLUDE_PACKET_CLONE
/**
   Create a packet that has the same content as a given COMPLETE packet.
   A copy of the packet buffer is performed, so that the two packets are
   independent.
 */
T_PACKET* _STDCALL packet_clone(T_PACKET *packet)
{
    int nbytes;
    T_PACKET *new_pkt;

    MHDR(packet);
    if (packet == NULL)
        return NULL;

    if (packet->state != PACKET_STATE_COMPLETE)
        return NULL;

    nbytes = packet->packet_buffer_size;
    new_pkt = packet_create("", nbytes);
    if (!new_pkt)
        return NULL;

    memcpy(new_pkt->packet_buffer, packet->packet_buffer, nbytes);
    new_pkt->state = PACKET_STATE_COMPLETE;
    //new_pkt->packet_buffer_size = nbytes;

    MFTR(packet);
    return new_pkt;
}
#endif
/**
    Free a packet. Free associated memory.
    \param[in] packet The packet to free.
 */
void _STDCALL
packet_free(T_PACKET *packet)
{
    MHDR(packet);
    if (packet == NULL)
        return;
    Free(packet->field_data);
    Free(packet->param_data);
    Free(packet->packet_buffer);
    Free(packet);
}

/** Get the type of a packet. */
int32_t _STDCALL
packet_get_packet_type(T_PACKET *packet)
{
    MHDR(packet);
    return packet->packet_buffer[PACKET_INDEX_TYPE];
}

/** Set the type of a packet. Use in server code only. */
void _STDCALL
packet_set_packet_type(T_PACKET *packet, int packet_type)
{
    MHDR(packet);
    packet->packet_buffer[PACKET_INDEX_TYPE] = packet_type;
}

/** Get the size of a packet. */
int32_t _STDCALL
packet_get_packet_size(T_PACKET *packet)
{
    MHDR(packet);
    return packet->packet_buffer[PACKET_INDEX_SIZE];
}

/** Get the packet_id of a packet. */
int32_t _STDCALL
packet_get_packet_id(T_PACKET *packet)
{
    MHDR(packet);
    return packet->packet_buffer[PACKET_INDEX_ID];
}

/** Get the format of a packet. */
int32_t _STDCALL
packet_get_packet_format(T_PACKET *packet)
{
    MHDR(packet);
    return packet->packet_buffer[PACKET_INDEX_FORMAT];
}

/** Get the seconds field of a packet. */
int32_t _STDCALL
packet_get_packet_sec(T_PACKET *packet)
{
    MHDR(packet);
    return packet->packet_buffer[PACKET_INDEX_INFO_1];
}

/** Get the nanoseconds field of a packet. */
int32_t _STDCALL
packet_get_packet_nanosec(T_PACKET *packet)
{
    MHDR(packet);
    return packet->packet_buffer[PACKET_INDEX_INFO_2];
}

/****************************************************************/
/* 3. Writing before transmission */
/****************************************************************/

static void packet_prepare_first_field(T_PACKET *packet)
{
    MHDR(packet);
    packet->write_position_tag = 0; /* keep track of previous field */
    packet->write_field_id = 0; /* automatic field_id counter */
    packet->state = PACKET_STATE_WRITING;
    packet->position = HEADER_SIZE;
    MFTR(packet);
}

/**
    Set a packet for writing. Write the header so that we're ready for fields.
    \param[in] packet The packet.
    \param[in] packet_type The type to be given to the packet. A value of -1
       causes the packet's type to be left unchanged.
    \param[in] packet_identifier The ID to be given to the packet. A value
       of -1 causes the packet's existing identifier to be incremented.
    \pre None.
    \post Packet is in WRITING state.
 */
void _STDCALL
packet_write(T_PACKET *packet, int32_t packet_type, int32_t packet_identifier)
{
    int32_t *buffer = packet->packet_buffer;
    double secs, nanosecs;

    MHDR(packet);
    if (packet_identifier == -1)
        buffer[PACKET_INDEX_ID]++;
    else
        buffer[PACKET_INDEX_ID] = packet_identifier;

    if (packet_type != -1)
        buffer[PACKET_INDEX_TYPE] = packet_type;

    get_time_double2(&secs, &nanosecs);

    buffer[PACKET_INDEX_SIZE] = HEADER_SIZE;
    buffer[PACKET_INDEX_INFO_1] = (uint32_t)secs;
    buffer[PACKET_INDEX_INFO_2] = (int32_t)nanosecs;
    buffer[PACKET_INDEX_INFO_3] = 0;
    packet->read_ok = 0;
    packet_prepare_first_field(packet);
    MFTR(packet);
}

static void packet_append_aux(T_PACKET *packet)
{
    int nfields;

    MHDR(packet);
    packet_read(packet);
    nfields = packet->field_count;
    if (nfields == 0) {
        packet_prepare_first_field(packet);
    } else {
        T_FIELD_DATA *last_field = &packet->field_data[nfields - 1];

        packet->write_position_tag = last_field->start_position + 1;
        packet->write_field_id = last_field->id; /* count up from here */
        packet->state = PACKET_STATE_FIELD_WRITTEN;
        packet->position = last_field->end_position;
    }

    packet_read_complete(packet);
    MFTR(packet);
}

/**
   Set a packet for appending fields & parameters.
   If packet is in WRITING or FIELD_WRITTEN state, does nothing.
   If packet is in COMPLETE state, finds last field and last param,
   so subsequent add commands append fields and params. If a COMPLETE
   packet contains no field, the packet will be in state WRITING, else
   in state FIELD_WRITTEN.
   Other states are not permitted.
   \return 0 if OK, -1 on error (wrong state)
*/
int32_t _STDCALL
packet_append(T_PACKET *packet)
{
    int32_t retval = 0;

    MHDR(packet);
    switch (packet->state) {
    case PACKET_STATE_WRITING:
    case PACKET_STATE_FIELD_WRITTEN:
        break;
    case PACKET_STATE_COMPLETE:
        packet_append_aux(packet);
        break;
    default:
        print_error("packet_append: Invalid state 0x%x", packet->state);
        retval = -1;
        break;
    }

    MFTR(packet);
    return retval;
}

/**
	Add a named packet field. Attempts to increase (realloc) buffer if
      necessary. If packet is not in WRITING or W_IN_FIELD state, calls
      packet_write to initialise, i.e. start as first field after header.
    \param[in] packet The packet.
    \param[in] name The field name.
    \param[in] field_id The field ID. If = -1, set automatically.
    \returns packet position, or -1 on failure of buffer realloc.
    \pre None.
    \post Packet is in W_IN_FIELD state, or in unchanged state on failure.
 */
int32_t _STDCALL
packet_add_field(T_PACKET *packet, int32_t name, int32_t field_id)
{
    int32_t tag = packet->write_position_tag;
    int32_t pos = packet->position;

    MHDR(packet);
    if ((packet->state & (PACKET_STATE_WRITING|PACKET_STATE_FIELD_WRITTEN)) == 0)
        packet_write(packet, -1, -1);

    if (packet_check_buffer_size(packet, pos + FIELD_SIZE + FOOTER_SIZE) != ERROR_NONE)
        return -1;

    /* update packet_buffer */
    {
        int32_t *buffer = packet->packet_buffer;

        if (tag > 0) /* add next field position pointer */
            buffer[tag] = pos - tag + 1;

        buffer[pos++] = name;
        packet->write_position_tag = pos; /* remember position of next field position info */
        buffer[pos++] = 0;/* set next field position to 0 */
        if (field_id == -1)
            buffer[pos++] = ++(packet->write_field_id);
        else
            buffer[pos++] = field_id;
    }
    packet->position = pos;
    packet->state = PACKET_STATE_FIELD_WRITTEN;

    MFTR(packet);
    return packet->position;
}

/**
    Add a named integer parameter to a packet
    \param[in] packet The packet.
    \param[in] name The parameter name.
    \param[in] data32 The parameter data value.
    \returns packet position, or -1 on error:
    - pre-condition failure, or
	- failure of buffer realloc.
    \pre Packet is in W_IN_FIELD state.
    \post Packet is in W_IN_FIELD state, or in unchanged state on failure.
 */
int32_t _STDCALL
packet_add_param_int(T_PACKET *packet, int32_t name, int32_t data32)
{
    int32_t pos = packet->position;

    MHDR(packet);
    if (packet->state != PACKET_STATE_FIELD_WRITTEN) {
        print_error("packet_add_param_int: invalid state 0x%x", packet->state);
        return -1;
    }

    if (packet_check_buffer_size(packet, pos + INT_PARAM_SIZE + FOOTER_SIZE) != ERROR_NONE)
        return -1;

    /* update packet_buffer */
    {
        int32_t *buffer = packet->packet_buffer;

        buffer[pos++] = name;
        buffer[pos++] = INT_PARAM_SIZE + (PARAM_INT << 24); /* position of next item */
        buffer[pos++] = data32;
    }
    packet->position = pos;

    MFTR(packet);
    return packet->position;
}

/**
    Add a named string parameter to a packet
    \param[in] packet The packet.
    \param[in] name The parameter name.
    \param[in] string_to_add The parameter data value.
    \returns packet position, or -1 on error:
    - pre-condition failure, or
	- failure of buffer realloc.
    \pre Packet is in W_IN_FIELD state.
    \post Packet is in W_IN_FIELD state, or in unchanged state on failure.
 */
int32_t _STDCALL
packet_add_param_string(T_PACKET *packet, int32_t name, const char *string_to_add)
{
    MHDR(packet);
    return packet_add_param_data(packet, name, PARAM_STRING,
                                 string_to_add,
                                 strlen(string_to_add) + 1);
}

/**
    Add a named, typed data array with a data identifier to a packet
    \param[in] packet The packet.
	\param[in] name The parameter name.
    \param[in] data_type The parameter data type.
    \param[in] data The parameter data value.
    \param[in] length The length in bytes of the data.
    \returns packet position, or -1 on error:
    - pre-condition failure, or
	- failure of buffer realloc.
    \pre Packet is in W_IN_FIELD state.
    \post Packet is in W_IN_FIELD state, or in unchanged state on failure.
 */
int32_t _STDCALL
packet_add_param_data(T_PACKET *packet, int32_t name, T_PARAM_DATA_TYPE data_type,
                      const void *data, int32_t length)
{
    void *data_ptr;

    MHDR(packet);
    data_ptr = packet_add_param_data_get_ptr(packet, name, data_type, length);

    if (!data_ptr)
        return -1;

    memcpy(data_ptr, data, length);

    MFTR(packet);
    return packet->position;
}

/**
    Add a named, typed data array to a packet, but return a pointer to be written to,
    rather than actually doing the copy. Similar to packet_add_param_data.
    \param[in] packet The packet.
	\param[in] name The parameter name.
    \param[in] data_type The parameter data type.
    \param[in] length The length in bytes of the data.
    \returns pointer to data area, or NULL on error:
    - pre-condition failure, or
	- failure of buffer realloc.
    \pre Packet is in W_IN_FIELD state.
    \post Packet is in W_IN_FIELD state, or in unchanged state on failure.
 */
void* _STDCALL
packet_add_param_data_get_ptr(T_PACKET *packet, int32_t name, T_PARAM_DATA_TYPE data_type,
                      int32_t length)
{
    int32_t pos = packet->position;
    int32_t len32;
    void *data_ptr;

    MHDR(packet);
    if (packet->state != PACKET_STATE_FIELD_WRITTEN) {
        print_error("packet_add_param_int: invalid state 0x%x", packet->state);
        return NULL;
    }

    len32 = (length >> 2);
    if (length & 0x03)
        len32++;

    if (packet_check_buffer_size(packet, pos + 2 + len32 + FOOTER_SIZE) != ERROR_NONE) {
        return NULL;
    }

    /* update packet_buffer */
    {
        int32_t *buffer = packet->packet_buffer;

        buffer[pos++] = name;
        buffer[pos++] = 2 + len32 + (data_type << 24);
        //memcpy(&buffer[pos], data, length);
        data_ptr = &buffer[pos];
    }
    pos += len32;
    packet->position = pos;

    MFTR(packet);
    return data_ptr;
}

/**
    Finish a packet write and set state. If packet is not in WRITING or
      W_IN_FIELD state, calls packet_write, giving an 'empty' packet with
      no fields.
    \param[in] packet The packet.
    \pre None.
    \post Packet is in COMPLETE state.
 */
void _STDCALL
packet_write_complete(T_PACKET *packet)
{
    int32_t pos = packet->position;
    int32_t tag = packet->write_position_tag;

    MHDR(packet);
    if (packet->state == PACKET_STATE_COMPLETE)
        /* OK, do nothing */
        return;

    if ((packet->state & (PACKET_STATE_WRITING|PACKET_STATE_FIELD_WRITTEN)) == 0)
        packet_write(packet, -1, -1);

    /* update packet_buffer */
    {
        int32_t *buffer = packet->packet_buffer;

        if (tag > 0) /* add next field position pointer */
            buffer[tag] = pos - tag + 1;

        buffer[pos++] = 0; /* checksum, omitted */
        buffer[pos++] = PACKET_FOOT_VALUE;
        buffer[PACKET_INDEX_SIZE] = pos;
    }
    packet->position = pos;
    packet->state = PACKET_STATE_COMPLETE;
    MFTR(packet);
}


/****************************************************************/
/* 4. Reading a packet  */
/****************************************************************/

/**
    Prepare a packet for reading.
    \param[in] packet The packet
    \return packet_type, or 0 on error.
    \pre packet must be in COMPLETE state
    \post read_ok flag is set
 */
int32_t _STDCALL
packet_read(T_PACKET *packet)
{
    MHDR(packet);
    if (packet->state != PACKET_STATE_COMPLETE) {
        print_error("packet_read, state=0x%08x", packet->state);
        return 0;
    }

    packet->position = HEADER_SIZE; /* ?? */
    packet->param_count = 0;
    packet->param_index = 0;

    packet->read_ok = 1;
    packet_scan_fields(packet);

    MFTR(packet);
    return packet->packet_buffer[PACKET_INDEX_TYPE];
}

/**
   Get the number of fields in a packet that has been opened for reading.
   \return number of fields, or -1 on error (packet_read not called).
*/
int32_t _STDCALL
packet_get_num_fields(T_PACKET *packet)
{
    MHDR(packet);
    if (!packet->read_ok)
        return -1;

    return packet->field_count;
}

/**
   Get the number of parameters in the current field of a packet.
   \return number of params, or -1 on error (packet_read not called
   or packet_get_next_field not called).
*/
int32_t _STDCALL
packet_get_num_params(T_PACKET *packet)
{
    MHDR(packet);
    if (!packet->read_ok)
        return -1;

    if (packet->param_data == NULL)
        return -1;

    return packet->param_count;
}

/**
    Get the next field in sequence during a packet read.
    \param[in] packet The packet
    \param[out] name name of field
    \param[out] field_id id of field
    \return 1 if found, else 0
 */
int32_t _STDCALL
packet_get_next_field(T_PACKET *packet, int32_t *name, int32_t *field_id)
{
    const int32_t pos = packet->field_index;
    T_FIELD_DATA *field;

    MHDR(packet);
    if (!packet->read_ok)
        return 0;
    if (pos >= packet->field_count)
        return 0;

    packet_scan_params(packet, pos);
    field = &packet->field_data[pos];
    *name = field->name;
    *field_id = field->id;

    packet->field_index++;

    MFTR(packet);
    return 1;
}

/**
    Get details (type, data pointer and data length) of given parameter.
    \param[in] param The parameter
    \param[out] typ type of param. Omitted if null.
    \param[out] data pointer to data of param. Omitted if null.
    \param[out] len length of data in units of data type. Omitted if null.
 */
static void get_param_info(T_PACKET* packet, T_PARAM_DATA *param, T_PARAM_DATA_TYPE *typ,
                      void **data_pointer, int32_t *len)
{
    MHDR(packet);
    if (typ)
        *typ = (T_PARAM_DATA_TYPE)param->data_type;

    if (data_pointer)
        *data_pointer = &(packet->packet_buffer[param->start_position + 2]);
    /* *data_pointer = param->data_ptr; */

    if (len)
        *len = param->data_len;
    MFTR(packet);
}

/**
    Get the next parameter in sequence during a field read.
    \param[in] packet The packet
    \param[out] name name of param
    \param[out] typ type of param. Omitted if null.
    \param[out] data pointer to data of param. Omitted if null.
    \param[out] len length of data in units of data type. Omitted if null.
    \return 1 if found, else 0
 */
int32_t _STDCALL
packet_get_next_param(T_PACKET *packet, int32_t *name, T_PARAM_DATA_TYPE *typ,
                      void **data_pointer, int32_t *len)
{
    int32_t pos;
    T_PARAM_DATA *param;

    MHDR(packet);
    if (!packet->read_ok)
        return 0;

    pos = packet->param_index;
    if ((pos < 0) || (pos >= packet->param_count))
        return 0;

    param = &packet->param_data[pos];
    *name = param->name;
    get_param_info(packet, param, typ, data_pointer, len);

    packet->param_index++;

    MFTR(packet);
    return 1;
}

/**
    Get named parameter during a field read.
    \param[in] packet The packet
    \param[in] name name of pram
    \param[out] typ type of param
    \param[out] data pointer to data of param. Omitted if null.
    \param[out] len length of data in units of data type. Omitted if null.
    \return 1 if found, else 0
 */
int32_t _STDCALL
packet_get_named_param(T_PACKET *packet, int32_t name, T_PARAM_DATA_TYPE *typ,
                      void **data_pointer, int32_t *len)
{
    int i, N;

    MHDR(packet);
    if (!packet->read_ok)
        return 0;

    N = packet->param_count;
    for (i=0; i<N; i++) {
        T_PARAM_DATA *param = &packet->param_data[i];

        if (param->name == name) {
            param->read = 1;
            get_param_info(packet, param, typ, data_pointer, len);
            return 1;
        }
    }
    MFTR(packet);
    return 0;
}

/**
    Get named parameter during a field read.
    \param[in] packet The packet
    \param[in] name name of pram
    \param[in] typ type of param
    \param[out] data pointer to data of param. Omitted if null.
    \param[out] len length of data in units of data type. Omitted if null.
    \return 1 if found, else 0
 */
static int32_t _STDCALL
packet_get_named_typed_param(T_PACKET *packet, int32_t name, T_PARAM_DATA_TYPE typ,
                      void **data_pointer, int32_t *len)
{
    T_PARAM_DATA_TYPE actual_typ;
    void *ptr;
    int32_t local_len;

    MHDR(packet);
    if (!packet_get_named_param(packet, name, &actual_typ, &ptr, &local_len))
        return 0;

    if (actual_typ != typ) {
        packet_print_type_warning(packet, actual_typ, typ, name);
        return 0;
    }

    *len = local_len;
    *data_pointer = ptr;
    MFTR(packet);
    return 1;
}

/**
    Get named integer parameter, or a default if not found.
    \param[in] packet The packet
    \param[in] name name of pram
    \param[in] default_val default value
    \return value if found, else default_val
 */
int32_t _STDCALL
packet_get_int_param_def(T_PACKET *packet,int32_t name, int32_t default_val)
{
    void *ptr;
    int32_t len;

    MHDR(packet);
    if (!packet_get_named_typed_param(packet, name, PARAM_INT, &ptr, &len))
        return default_val;

    return *(int32_t*)ptr;
}

int32_t _STDCALL
packet_get_int_param_spec3(T_PACKET *packet, int32_t name,
                      int32_t default_val, int32_t lower_limit, int32_t upper_limit)
{
    int32_t val = packet_get_int_param_def(packet, name, default_val);
    if (val > upper_limit)
        return upper_limit;
    else if (val < lower_limit)
        return lower_limit;
    else
        return val;
}

/**
    Get named integer parameter, or a default if not found.
    \param[in] packet The packet
    \param[in] name name of pram
    \param[in] default_val default value
    \return value if found, else default_val
 */
char* _STDCALL
packet_get_string_param_def(T_PACKET *packet,int32_t name, char *default_val)
{
    void *ptr;
    int32_t len;

    MHDR(packet);
    if (!packet_get_named_typed_param(packet, name, PARAM_STRING, &ptr, &len))
        return default_val;

    return (char*)ptr;
}

#ifdef INCLUDE_DEPRECATED
/**
   \deprecated
    Retrieve a named 32 bit integer parameter from the current field.
    \return 1 if packet is readable and param is found, else -1
 */
int32_t _STDCALL
packet_get_param_int(T_PACKET *packet, int32_t name, int32_t *data32)
{
    T_PARAM_DATA_TYPE typ;
    void *ptr;
    int32_t len;

    MHDR(packet);
    if (!packet_get_named_param(packet, name, &typ, &ptr, &len))
        return -1;

    if (typ != PARAM_INT) {
        packet_print_type_warning(packet, typ, PARAM_INT, name);
        return -1;
    }

    *data32 = *(int32_t*)ptr;
    MFTR(packet);
    return 1;
}


/**
   \deprecated
    Retrieve a named string parameter from a packet.
    If string is not found, return -1;
 */
int32_t _STDCALL
packet_get_param_string(T_PACKET *packet, int32_t name, char *str, int32_t max_length)
{
    T_PARAM_DATA_TYPE typ;
    void *ptr;
    int32_t len;

    MHDR(packet);
    if (!packet_get_named_param(packet, name, &typ, &ptr, &len))
        return -1;

    if (typ != PARAM_STRING) {
        packet_print_type_warning(packet, typ, PARAM_STRING, name);
        return -1;
    }

    if (len >= max_length)
        len = max_length - 1;
    str[0] = '\0';
    strncat(str, (char*)ptr, len);

    MFTR(packet);
    return len;
}


/**
   \deprecated
    Retrieve a named raw data array from a packet and return the data length.
    If not present, return -1.
 */
int32_t _STDCALL
packet_get_param_data(T_PACKET *packet, int32_t name, T_PARAM_DATA_TYPE *data_type,
                      void *raw_data, int32_t max_length)
{
    T_PARAM_DATA_TYPE typ;
    void *ptr;
    int32_t len, raw_len;

    MHDR(packet);
    if (!packet_get_named_param(packet, name, &typ, &ptr, &len))
        return -1;

    if (typ < 0x80) {
        packet_print_type_warning(packet, typ, PARAM_DATA_RAW, name);
        return -1;
    }

    /* effectively 'cast' type ' typ' to type 'raw', which is a bit horrible */
    raw_len = len * type_scale_fac(PARAM_DATA_RAW) / type_scale_fac(typ);
    if (raw_len > max_length)
        raw_len = max_length;

    *data_type = typ;
    memcpy(raw_data, ptr, raw_len);
    MFTR(packet);
    return raw_len;
}

/**
   \deprecated
    Returns the start of data pointer for param in packet if present
    *datatype       will return the data type as specified in packer
    *dataPointer    will return pointer to start of data in packet (BE WARNED WILL NOT REMAIN VALID AFTER OTHER CALLS)
    *dataLen        will return the number of bytes
    If not present, return -1.

    NOTE THAT THIS IS A TRANSIENT POINTER only valid until other calls that may change the packet
    USE WITH CAUTION
 */
int32_t _STDCALL
packet_access_param(T_PACKET *packet, int32_t name, T_PARAM_DATA_TYPE *datatype, void **dataPointer)
{
    T_PARAM_DATA_TYPE typ;
    void *ptr;
    int32_t len, raw_len;

    MHDR(packet);
    if (!packet_get_named_param(packet, name, &typ, &ptr, &len))
        return -1;

    /* effectively 'cast' type ' typ' to type 'raw', which is a bit horrible */
    raw_len = len * type_scale_fac(PARAM_DATA_RAW) / type_scale_fac(typ);

    if (datatype)
        *datatype = typ;

    if (dataPointer)
        *dataPointer = ptr;

    MFTR(packet);
    return raw_len;
}
#endif /* def INCLUDE_DEPRECATED */

/**
    Finish a packet read.
 */
void _STDCALL
packet_read_complete(T_PACKET *packet)
{
    MHDR(packet);
    packet->read_ok = 0;
}

/**
    Report unread field parameters following a field read.
 */
int32_t _STDCALL
packet_get_unread_field_params(T_PACKET *packet)
{
    int32_t i;
    int32_t N = packet->param_count;
    char fname[5];
    char name[5];
    int32_t res=0;

    MHDR(packet);
    for (i=0; i<N; i++)
    {
        if (packet->param_data[i].read==0)
        {
            packet_key_to_str(packet->field_data[packet->field_index-1].name,fname);
            packet_key_to_str(packet->param_data[i].name,name);
            print_warning("Unrecognised parameter in field %s: name %s, type:%s",
                          fname,
                          name,
                          packet_param_data_type_to_string((T_PARAM_DATA_TYPE)packet->param_data[i].data_type));
            res++;
        }
    }
    MFTR(packet);
    return res;
}
//#ifdef _WIN32
//#if _MSC_VER < 0 
//#define snprintf _snprintf
//#define snprintf _snprintf
//#endif
//#endif

/** \todo unify with packet_get_unread_field_params */
int32_t _STDCALL
packet_get_unread_field_params_ack(T_PACKET *packet, T_PACKET *ack_packet)
{
    int32_t i;
    int32_t N = packet->param_count;
    char fname[5];
    char name[5];
    int32_t res=0;
    int field_name;
    char mbuf[128];

    MHDR(packet);
    for (i=0; i<N; i++)
    {
        if (packet->param_data[i].read == 0)
        {
            field_name = packet->field_data[packet->field_index-1].name;
            packet_key_to_str(field_name, fname);
            packet_key_to_str(packet->param_data[i].name, name);
            //packet_add_field(ack_packet, field_name, packet->field_data[packet->field_index-1].id);
            snprintf(mbuf, 128, "Unrecognised parameter in field %s: name %s, type:%s",
                          fname,
                          name,
                          packet_param_data_type_to_string((T_PARAM_DATA_TYPE)packet->param_data[i].data_type));
            packet_add_param_string(ack_packet, ANY_WARNING, mbuf);
            packet_add_param_int(ack_packet, ANY_WARNING_CODE, WARNING_UNRECOGNISED_PARAMETER);
            res++;
        }
    }
    MFTR(packet);
    return res;
}

/****************************************************************/
/* n. Transmission on TCP */
/****************************************************************/

// Direction 0 = out, 1 = in
static void debug_echo(T_PACKET *packet, int32_t sock, int32_t direction)
{
    struct sockaddr_in sa;
    socklen_t sa_len = sizeof(sa);
    double secs, nanosecs;

    MHDR(packet);
    if ((debug_network_echo <= 0) || (debug_network_echo == sock))
        return;

    get_time_double2(&secs, &nanosecs);
    packet_append(packet);

    if (getpeername(sock, (struct sockaddr*)&sa,&sa_len) == -1) {
        print_sys_error("getpeername() failed");
        return;
    }

    packet_add_field(packet,FIELD_DEBUG,-1);
    packet_add_param_string(packet,DEBUG_NETWORK_MONITOR_CLIENT_IP,inet_ntoa(sa.sin_addr));
    if (direction == 1)
    {
        packet_add_param_int(packet, DEBUG_NETWORK_MONITOR_RX_TIME_SEC, (uint32_t)secs);
        packet_add_param_int(packet, DEBUG_NETWORK_MONITOR_RX_TIME_NSEC, (int32_t)nanosecs);
    }
    else
    {
        packet_add_param_int(packet, DEBUG_NETWORK_MONITOR_TX_TIME_SEC, (uint32_t)secs);
        packet_add_param_int(packet, DEBUG_NETWORK_MONITOR_TX_TIME_NSEC, (int32_t)nanosecs);
    }
    packet_send(packet,debug_network_echo);
    MFTR(packet);
}

/**
    Send packet on active TCP connection on socket.
    \param[in] packet The packet.
	\param[in] sock The previously opened socket to send it on.
    \returns Total number of bytes sent, or error code.
    \pre None
    \post Packet is in COMPLETE state, or in unchanged state on failure.
 */
int32_t MY_STDCALL
packet_send(T_PACKET *packet, int32_t sock)
{
    int32_t na, totalsent, txsize, sendsize;
    char *buf = (char*)packet->packet_buffer;

    MHDR(packet);
    /* Put packet in COMPLETE state. */
    packet_write_complete(packet);

#ifdef PRINT_PACKETS
    printf("Sending:");
    packet_print(packet);
#endif
    totalsent = 0;
    txsize = packet->packet_buffer[PACKET_INDEX_SIZE] * 4;

    while (totalsent < txsize) {
        sendsize = txsize - totalsent;
        if( (na = send(sock, buf + totalsent, sendsize, 0)) == -1) {
            print_sys_error("tx of packet %d failed on socket:%d",
                            packet->packet_buffer[PACKET_INDEX_ID], sock);
            if (sock==debug_network_echo) {
                debug_network_echo = -debug_network_echo;
                print_info("network echo reset\n");
            }
            return -1;
        }

        txbytecount += na;
        if (debug_tx_bytes)
            print_debug("TX:%d bytes on socket:%d\n", na, sock);
        totalsent += na;

        if (na != sendsize)
            print_info("incomplete send, sent:%d bytes , wanted:%d bytes\n", na, sendsize);
    }

    debug_echo(packet, sock, 0);

    MFTR(packet);
    return totalsent;
}




/*
//
//void resync(T_PACKET *packet,int32_t sock)
//{
//    char *buffer;
//    buffer=packet->packet_buffer;
//    char *header;
//    header=packet->header;
//    for (;;)
//    {
//        int32_t i;
//        for (i=0;i<16;i++)
//        {
//            if (buffer[i]==header[0])
//            {
//                if (buffer[i+1]==header[1]) & (buffer[i+2]==header[2]) & (buffer[i+3]==header[3])
//                {
//                    break;
//                }
//            }
//        }
//        na=recv(sock,(char*) packet->packet_buffer, 32, 0);
//    }
//}
*/

#ifndef  _WIN32
/**
   Peek at (i.e. read without consuming data) a packet on a socket.
   \param[in] sock the socket to read
   \param[out] packet_type type of packet
   \return 1 if OK, -1 if not
 */
int32_t MY_STDCALL
peek_packet(int sock, int *packet_type)
{
    int buf[HEADER_SIZE];
    int nbytes;

    nbytes = recv(sock, buf, HEADER_BYTES, MSG_DONTWAIT | MSG_PEEK);
    if (nbytes != HEADER_BYTES)
        return -1;
    if (buf[PACKET_INDEX_HEAD] != PACKET_HEAD_VALUE)
        return -1;

    *packet_type = buf[PACKET_INDEX_TYPE];

    return 1;
}
#endif
/**
   Receive header of packet and check header value and packet buffer size.
    \param[in] packet The packet.
    \param[in] sock The previously opened socket to receive the header on.
    \return 0 if OK else <0
    \pre None
    \post Packet is in state RX_HDR_OK, or unchanged on any error
 */
static int32_t
packet_receive_header(T_PACKET *packet, int32_t sock)
{
    int32_t na, error, buffer_bytes;
    int32_t header[HEADER_SIZE];

    MHDR(packet);
    /* return silently if header already received */
    if (packet->state == PACKET_STATE_RX_HDR_OK)
        return ERROR_NONE;

    /* receive header into local buffer */
    na = recv(sock, (char*)header, HEADER_BYTES, 0);
    if (na == -1) {
        print_sys_error("failed to receive packet header on socket %d", sock);
        return -1;
    } else if (na != HEADER_BYTES) {
        print_error("packet_receive_header: received %d bytes instead of %d on socket %d",
                    na, HEADER_BYTES, sock);
        return -1;
    }
    rxbytecount += na;

    /* check header is OK */
    if (header[PACKET_INDEX_HEAD] != PACKET_HEAD_VALUE) {
        rx_bad_headers++;
        return -E_BADHDR;
    }

	/* Ensure packet buffer is big enough. */
    buffer_bytes = header[PACKET_INDEX_SIZE] << 2;
    if (packet->packet_buffer_size < buffer_bytes) {
        if ( (error = resize((void**)&packet->packet_buffer, buffer_bytes)) != ERROR_NONE)
            return -1;
        packet->packet_buffer_size = buffer_bytes;
    }

    /* copy header into into packet_buffer and update state */
    memcpy((char*)packet->packet_buffer, (char*)header, HEADER_BYTES);
    packet->state = PACKET_STATE_RX_HDR_OK;
    packet->read_ok = 0;
    packet->rx_byte_position = HEADER_BYTES;

    MFTR(packet);
    return ERROR_NONE;
}

/**
	Receive packet on TCP socket.
    \param[in] packet The packet.
    \param[in] sock The previously opened socket to receive it on.
    \return 1 if packet is complete or 0 if not complete, or a
      negative error code.
    \pre None
    \post Packet is in state RX_HDR_OK or COMPLETE or RX_ERROR.
*/
int32_t MY_STDCALL
packet_receive(T_PACKET *packet, int32_t sock)
{
    int32_t error, packet_bytes, packet_size, bytes_to_read;

    MHDR(packet);
    packet_receive_call_count++;

    if ( (error = packet_receive_header(packet, sock)) != 0)
        return error;

    packet_size = packet->packet_buffer[PACKET_INDEX_SIZE];
    packet_bytes = 4 * packet_size;
    bytes_to_read = packet_bytes - packet->rx_byte_position;

    /* receive bytes into body of packet */
    if (bytes_to_read > 0) {
        int32_t na;
        char *buf = (char*)packet->packet_buffer;

        if ( (na = recv(sock, buf + packet->rx_byte_position, bytes_to_read , 0)) < 0) {
            print_sys_error("recv error on socket:%d", sock);
            return -E_RECV;
        }

        if (na == 0)
            return 0;

        /* assert na > 0 */
        rxbytecount += na;
        packet->rx_byte_position += na;

        if (debug_rx_bytes)
            printf("RX:%d bytes on socket:%d",na,sock);

        if (na < bytes_to_read) {
            incomplete_rx_packets++;
            return 0;
        }

    }
    /* assert packet is completely received */

    /* check footer */
    if (packet->packet_buffer[packet_size - 1] != PACKET_FOOT_VALUE) {
        bad_rx_packets++;
        packet->state = PACKET_STATE_RX_ERROR;
        return -E_BADFOOT;
    }

    good_rx_packets++;
    packet->state = PACKET_STATE_COMPLETE;

    debug_echo(packet, sock, 1);

    if (debug_show_rx_packets) {
        if ((force_no_link_packets <= 0)
            || (packet->packet_buffer[PACKET_INDEX_TYPE] != PACKET_TYPE_LINK))
            packet_print(packet);
    }

#ifdef PRINT_PACKETS
    printf("Received:");
    packet_print(packet);
#endif

    MFTR(packet);
    return 1;
}


/**
   Simple wrapper for stdio fopen, with mode "rb"
   \param[in] filename
*/
FILE* _STDCALL
file_open_read(const char *filename)
{
	return fopen(filename, "rb");
}

/**
   Simple wrapper for stdio fopen, with mode "wb"
   \param[in] filename
*/
FILE* _STDCALL
file_open_write(const char *filename)
{
	return fopen(filename, "wb");
}

/** simple wrapper for stdio fclose */
int32_t _STDCALL
file_close(FILE *stream)
{
	return fclose(stream);
}

/** simple wrapper for stdio fflush */
int32_t _STDCALL
file_flush(FILE *stream)
{
	return fflush(stream);
}

/** write a packet to a file */
int32_t _STDCALL
packet_file_write(T_PACKET *packet, FILE *stream)
{
	int n;
	int32_t count = packet->packet_buffer[PACKET_INDEX_SIZE];

    MHDR(packet);
	if ( (n = fwrite(packet->packet_buffer, sizeof(int32_t), count, stream)) != count) {
		print_sys_error("wrote %d out of %d int32's to file", n, count);
		return -1;
	}
	return ERROR_NONE;
}

/**
   Read a packet from a file.
   \param[in] packet The packet.
   \param[in] stream File stream previously opened for (at least) reading.
   \return 0 if OK, or <0
   \pre None
   \post packet is in state FREAD_ERROR or COMPLETE
*/
int32_t _STDCALL
packet_file_read(T_PACKET *packet, FILE *stream)
{
    int32_t n, error, packet_size, buffer_bytes, count;
    int32_t header[HEADER_SIZE];

    MHDR(packet);
    count = HEADER_SIZE;
	if ( (n = fread(header, sizeof(int32_t), count, stream)) != count) {
        if (feof(stream))
            print_info("read to end of file");
        else
            print_sys_error("read %d out of %d (header) int32's from file", n, count);
		return -1;
    }

    /* check header is OK */
    if (header[PACKET_INDEX_HEAD] != PACKET_HEAD_VALUE)
        return -E_BADHDR;

    /* Ensure packet buffer is big enough. */
    packet_size = header[PACKET_INDEX_SIZE];
    buffer_bytes = packet_size << 2;
    if (packet->packet_buffer_size < buffer_bytes) {
        if ( (error = resize((void**)&packet->packet_buffer, buffer_bytes)) != ERROR_NONE)
            return error;
        packet->packet_buffer_size = buffer_bytes;
    }

    /* ready for error exits */
    packet->state = PACKET_STATE_FREAD_ERROR;
    packet->read_ok = 0;

    /* copy header into packet_buffer and update state */
    memcpy((char*)packet->packet_buffer, (char*)header, HEADER_BYTES);

    count = packet_size  - HEADER_SIZE;
	if ( (n = fread(&packet->packet_buffer[HEADER_SIZE],
                    sizeof(int32_t), count, stream)) != count) {
		print_sys_error("read %d out of %d (body) int32's from file", n, count);
		return -1;
    }

    /* check footer */
    if (packet->packet_buffer[packet_size - 1] != PACKET_FOOT_VALUE)
        return -E_BADFOOT;

    packet->state = PACKET_STATE_COMPLETE;

    MFTR(packet);
	return ERROR_NONE;
}


/****************************************************************/
/* Local functions */
/****************************************************************/

/**
    Convert a param data type to a printable string
 */
static char*
packet_param_data_type_to_string(T_PARAM_DATA_TYPE param_data_type)
{
	switch (param_data_type)
	{
        case PARAM_STRING: return "string";
        case PARAM_UNSIGNED_INT: return "unsigned int";
        case PARAM_INT: return "int";
        case PARAM_DATA_RAW: return "data_raw";
        case PARAM_DATA_UNSIGNED_8: return "unsigned_8_bit_array";
        case PARAM_DATA_UNSIGNED_16: return "unsigned_16_bit_array";
        case PARAM_DATA_UNSIGNED_32: return "unsigned_32_bit_array";
        case PARAM_DATA_SIGNED_8: return "signed_8_bit_array";
        case PARAM_DATA_SIGNED_16: return "signed_16_bit_array";
        case PARAM_DATA_SIGNED_32: return "signed_32_bit_array";
        default: return "un-recognised data type";
    }
}

/** Factor required to convert from length in int32 to length in data type. */
static int type_scale_fac(T_PARAM_DATA_TYPE typ)
{
    switch (typ) {
    case PARAM_STRING:
    case PARAM_DATA_RAW:
    case PARAM_DATA_UNSIGNED_8:
    case PARAM_DATA_SIGNED_8:
        return 4;
    case PARAM_DATA_UNSIGNED_16:
    case PARAM_DATA_SIGNED_16:
        return 2;
    default:
        return 1;
    }
}

/**
    Scan a packet for field data. Create a table of names and positions.
    \todo In future, could dispense with field_data, and just use
    packet_buffer itself.
 */
static int32_t
packet_scan_fields(T_PACKET *packet)
{
    /* TODO: review packet_scan_fields */

    int32_t *buffer;
    int32_t i, count, error;
    int32_t start_pos;
    int32_t next_field;
    int32_t foot_start;

    MHDR(packet);
    foot_start = packet->packet_buffer[PACKET_INDEX_SIZE] - 2;
    buffer = packet->packet_buffer;

    /* start after header */
    start_pos = HEADER_SIZE;

    /* determine number of fields */
    count = 0;
    for (;;) {
        if (start_pos == foot_start)
            break; /* OK */

        next_field = buffer[start_pos + 1] + start_pos;
        if ((next_field < start_pos) || (next_field > foot_start)) {
            print_warning("packet_scan_fields: packet (ID=%d) corrupted (nf=%d) near offset %d",
                          packet->packet_buffer[PACKET_INDEX_ID],
                          next_field,
                          start_pos);
            break;
        }

        start_pos = next_field;
        count++;
    }

    /* ensure field_data is big enough */
    if (packet->field_size < count) {
        const int32_t nbytes = count * sizeof(T_FIELD_DATA);

        if ( (error = resize((void**)&packet->field_data, nbytes)) != ERROR_NONE)
            return error;
        packet->field_size = count;
    }

    /* fill in all the fields */
    start_pos = HEADER_SIZE;
    for (i = 0; i < count; i++) {
        T_FIELD_DATA *data = &packet->field_data[i];

        data->start_position = start_pos;
        data->name = buffer[start_pos];

        next_field = buffer[start_pos + 1] + start_pos;

        data->id = buffer[start_pos + 2];
        data->end_position = next_field;
        start_pos = next_field;
    }
    packet->field_count = count;
    packet->field_index = 0;

    MFTR(packet);
    return ERROR_NONE;
}

/**
    Scan the parameters for a given field. Create a lookup table of names, types and positions.
 */
static int32_t
packet_scan_params(T_PACKET *packet, int32_t field_number)
{
    int32_t *buffer = packet->packet_buffer;
    const int32_t field_start = packet->field_data[field_number].start_position;
    const int32_t field_end = packet->field_data[field_number].end_position;
    int32_t i, count, error, start_pos, next_param;


    MHDR(packet);
    /* determine number of params */
    start_pos = field_start + 3;
    count = 0;
    for (;;) {

        if (start_pos == field_end)
            break; /* OK */

        next_param = (buffer[start_pos + 1] & 0xFFFFFF) + start_pos;
        if ((next_param < start_pos) || (next_param > field_end)) {
            print_warning("packet_scan_params: packet (ID=%d) corrupted (np=%d) near offset %d",
                          packet->packet_buffer[PACKET_INDEX_ID],
                          next_param,
                          start_pos);
            break;
        }

        start_pos = next_param;
        count++;
    }

    /* ensure param_data is big enough */
    if (packet->param_size < count) {
        const int32_t nbytes = count * sizeof(T_PARAM_DATA);

        if ( (error = resize((void**)&packet->param_data, nbytes)) != ERROR_NONE)
            return error;
        packet->param_size = count;
    }

    /* fill in all the params */
    start_pos = field_start + 3;
    for (i = 0; i < count; i++) {
        T_PARAM_DATA *param = &packet->param_data[i];
        size_t len32;

        param->start_position = start_pos;
        /*param->data_ptr = &buffer[start_pos + 2];*/
        param->name = buffer[start_pos];
        param->read = 0;
        param->data_type = (buffer[start_pos + 1] >> 24) & 0xFF;
        len32 =  (buffer[start_pos + 1] & 0xFFFFFF);
        param->data_len = (len32 - 2) * type_scale_fac((T_PARAM_DATA_TYPE)param->data_type);
        next_param = len32 + start_pos;
        param->end_position = next_param;
        start_pos = next_param;
    }
    packet->param_count = count;
    packet->param_index = 0;

    MFTR(packet);
    return ERROR_NONE;
}

static void
packet_print_type_warning(const T_PACKET *packet, int32_t atyp, int32_t etyp,
                                      int32_t param_name)
{
    char name1[5], name2[5];

    MHDR(packet);
    print_warning(
       "[Mismatched data type in field %s: param %s expected %s, got %s]",
       packet_key_to_str(packet->field_data[packet->field_index].name, name1),
       packet_key_to_str(param_name, name2),
       packet_param_data_type_to_string((T_PARAM_DATA_TYPE)etyp),
       packet_param_data_type_to_string((T_PARAM_DATA_TYPE)atyp));
    MFTR(packet);
}

/**
   Check that the packet buffer is large enough.
   If it is not, attempt to realloc.
   \param[in] packet The packet.
   \param[in] required_size The required size of the buffer in int32's.
   \returns 0 if OK, -1 if realloc fails.
*/
static int32_t
packet_check_buffer_size(T_PACKET *packet, int32_t required_size)
{
    const int32_t required_bytes = required_size << 2;
    const int32_t headroom = BUFFER_HEADROOM_BYTES;
    int32_t new_size_bytes;
    void *new_ptr;

    MHDR(packet);
    if (packet->packet_buffer_size < required_bytes)
    {
        new_size_bytes = required_bytes + headroom;
        new_ptr = realloc(packet->packet_buffer, new_size_bytes);
        if (new_ptr == NULL)
        {
            print_sys_error("check_buffer_size: realloc failed (%d bytes requested)", new_size_bytes);
            return -1;
        }
        else
        {
            packet->packet_buffer = (int32_t*)new_ptr;
            packet->packet_buffer_size = new_size_bytes;
            packet->num_reallocs++;
        }
    }

    MFTR(packet);
    return 0;
}

/**
   Resize buffer to new_size bytes without maintaining contents.
   If malloc fails, *ptr is unchanged.
   \param[inout] ptr pointer to previously malloc'ed pointer
   \param[in] new_size required size in bytes
 */
static int32_t
resize(void **ptr, int32_t new_size)
{
    void *new_ptr;

    if ( (new_ptr = Malloc(new_size)) == NULL) {
        print_sys_error("malloc failed (%d bytes requested)", new_size);
        return -1;
    }
    Free(*ptr);
    *ptr = new_ptr;

    return 0;
}

