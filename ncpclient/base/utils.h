/*-----------------------------------------------------------------------------
 * (c) CRFS Limited, 2011. All rights reserved.
 *
 * This software is the property of CRFS Limited and may not be copied or
 * reproduced otherwise than on to a single hard disk for backup or
 * archival purposes. The source code is confidential information and must
 * not be disclosed to third parties or used without the express written
 * permission of CRFS Limited.
 *
 * Filename : utils.h
 * Author : Dan Timson
 *
 * Purpose: General utility functions.
 *
 * Description:
 *---------------------------------------------------------------------------*/
#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif
#if 0
}
#endif

#include <stdint.h>

double get_time_double(void);
double get_time_double2(double *secs, double *nanosecs);
void swap_endian(char *source, char *dest,int length);

#ifdef WIN32
#include <winsock2.h>
/* Windows does not have <sys/time.h> */
int gettimeofday(struct timeval*, void* /* tzp (unused) */);
#endif

#ifdef __cplusplus
}
#endif

#endif /* UTILS_H_INCLUDED */

