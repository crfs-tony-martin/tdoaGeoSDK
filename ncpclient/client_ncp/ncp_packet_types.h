/*-----------------------------------------------------------------------------
 * (c) CRFS Limited, 2011. All rights reserved.
 *
 * This software is the property of CRFS Limited and may not be copied or
 * reproduced otherwise than on to a single hard disk for backup or
 * archival purposes. The source code is confidential information and must
 * not be disclosed to third parties or used without the express written
 * permission of CRFS Limited.
 *
 * Filename : ncp_packet_types.h
 * Author : Dan Timson
 *
 * Purpose: NCP packet type defines
 *
 *---------------------------------------------------------------------------*/

#ifndef NCP_PACKET_TYPES_H_INCLUDED
#define NCP_PACKET_TYPES_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif
#if 0
}
#endif

/* Reduce multi character constant to integer constant on compile: */
#define PKEY_CONST(a,b,c,d) ((a) + ((b)<<8) + ((c)<<16) + ((d)<<24))
/* The resulting integer is equivalent to byte array {a,b,c,d} on
   little-endian architectures, and to {d,c,b,a} on big-endian
   architectures. */

/**-------------------------------------------------------------------------
                         NCP PACKET TYPES
--------------------------------------------------------------------------*/
enum packet_type {
    PACKET_TYPE_LINK        = PKEY_CONST('L','I','N','K'), /* [packet,RO]        link admin packet type */
    PACKET_TYPE_STATUS      = PKEY_CONST('S','T','A','T'), /* [packet,RO]        node status - 1 per sec, when implemented */
    PACKET_TYPE_NODE        = PKEY_CONST('N','O','D','E'), /* [packet,RW]        node control packet type */
    PACKET_TYPE_DSP_CONTROL = PKEY_CONST('D','S','P','C'), /* [packet,RW]        DSP operation immediate */
    PACKET_TYPE_DSP_LOOP    = PKEY_CONST('D','S','P','L'), /* [packet,RW]        DSP operation list loop */
    PACKET_TYPE_CRFS        = PKEY_CONST('C','R','F','S')  /* [packet,RW]        CRFS 'under the hood' functions */
};

#ifdef __cplusplus
}
#endif

#endif
