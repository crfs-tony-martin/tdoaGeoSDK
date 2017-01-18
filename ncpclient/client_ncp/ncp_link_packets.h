/*-----------------------------------------------------------------------------
 * (c) CRFS Limited, 2011. All rights reserved.
 *
 * This software is the property of CRFS Limited and may not be copied or
 * reproduced otherwise than on to a single hard disk for backup or
 * archival purposes. The source code is confidential information and must
 * not be disclosed to third parties or used without the express written
 * permission of CRFS Limited.
 *
 * Filename : ncp_link_packets.h
 * Author : Dan Timson
 *
 * Purpose: NCP LINK packet key defines
 *
 *---------------------------------------------------------------------------*/
#ifndef NCP_LINK_PACKETS_H_INCLUDED
#define NCP_LINK_PACKETS_H_INCLUDED

#include "ncp_packet_types.h"

enum link_field {
    LINK_FIELD_SERVER_GREETING  = PKEY_CONST('H','E','L','O'),
    LINK_FIELD_CLIENT_CONN_REQ  = PKEY_CONST('C','C','R','E'),

    LINK_FIELD_SERVER_AUTH_REQ  = PKEY_CONST('S','C','A','R'),
    LINK_FIELD_CLIENT_AUTH_RESP = PKEY_CONST('C','A','R','E'),
    LINK_FIELD_SERVER_CONFIRM   = PKEY_CONST('S','C','O','N'),

    LINK_FIELD_TERM_REQ = PKEY_CONST('T','E','R','M')
};

enum link_param {
    LINK_PARAM_CLIENT_ID   = PKEY_CONST('C','I','W', 0),
    LINK_PARAM_CLIENT_AUTH = PKEY_CONST('C','A','R', 0),
    LINK_PARAM_REASON      = PKEY_CONST('R','E','S','N'),

    LINK_PARAM_SERVER_ID   = PKEY_CONST('S','E','I','D'),
    LINK_PARAM_NCP_VERSION = PKEY_CONST('N','C','P','V'),
    LINK_PARAM_SERVER_MAC  = PKEY_CONST('M','A','C','A'),
    LINK_PARAM_SERVER_AUTH = PKEY_CONST('S','E','C','R'),
    LINK_PARAM_SERVER_ID2  = PKEY_CONST('S','C','N', 0)
};

#endif /* NCP_LINK_PACKETS_H_INCLUDED */
