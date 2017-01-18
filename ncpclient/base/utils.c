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

#include "utils.h"
#include <stddef.h>
#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/time.h>
#endif
#if 1
double get_time_double(void)
{
    return get_time_double2(0,0);
}
#endif
/**
	Return the current system time in seconds as a double
 */
double get_time_double2(double *secs, double *nanosecs)
{
	double now;
	struct timeval time;
    double my_secs, my_nanosecs;

	gettimeofday(&time, NULL);
    my_secs = time.tv_sec;
    my_nanosecs = time.tv_usec*1e3;
	now = my_secs + my_nanosecs/1e9;

    if (secs)
        *secs = my_secs;
    if (nanosecs)
        *nanosecs = my_nanosecs;

	return now;
}

void swap_endian(char *source, char *dest,int length)
{
    int count=length;
    int offset=3;
    int i;
    int wordcount=0;
    for (i=0;i<count;i++)
    {
        dest[i]=source[offset+wordcount*4];
        offset--;
        if (offset<0)
        {
            offset=3;
            wordcount++;
        }
    }
}

#ifdef WIN32
#include <time.h>
#include <math.h>
/**
   Provide gettimeofday facility to Windows environment.
   Precision is milliseconds. Requires struct timeval,
   which is declared in <winsock2.h>.
*/
int
gettimeofday(struct timeval *tv, void *tzp /* unused */)
{
	double now_double, sec, usec;
	time_t t;
	SYSTEMTIME systime;

	time(&t);
	GetLocalTime(&systime);
	now_double = t + (systime.wMilliseconds / 1000.0);
	usec = modf(now_double, &sec) * 1e6;
	tv->tv_sec = (int32_t)floor(sec);
	tv->tv_usec = (int32_t)floor(usec);
	return 0;
}



#endif
