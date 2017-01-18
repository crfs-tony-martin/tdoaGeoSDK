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
#ifndef TCP_H_INCLUDED
#define TCP_H_INCLUDED

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

int32_t tcp_connect(const char *ip_addr_or_host_name, int32_t port, int32_t noblock,
                                  char *ipaddr, size_t ipaddr_len);
int32_t tcp_disconnect(int32_t sock);

#ifdef __cplusplus
}
#endif

#endif /* TCP_H_INCLUDED */
