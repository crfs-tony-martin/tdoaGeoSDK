/*-----------------------------------------------------------------------------
 * (c) CRFS Limited, 2011. All rights reserved.
 *
 * This software is the property of CRFS Limited and may not be copied or
 * reproduced otherwise than on to a single hard disk for backup or
 * archival purposes. The source code is confidential information and must
 * not be disclosed to third parties or used without the express written
 * permission of CRFS Limited.
 *
 * Filename : error.c
 * Author : Dan Timson
 *
 * Purpose: Error reporting
 *
 * Description:
 *---------------------------------------------------------------------------*/
#include "crfs_debug.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>


#ifdef _WIN32
    /* from /usr/sys/syslog.h on Linux kernel 2.6.31 */
    #define	LOG_EMERG	0	/* system is unusable */
    #define	LOG_ALERT	1	/* action must be taken immediately */
    #define	LOG_CRIT	2	/* critical conditions */
    #define	LOG_ERR		3	/* error conditions */
    #define	LOG_WARNING	4	/* warning conditions */
    #define	LOG_NOTICE	5	/* normal but significant condition */
    #define	LOG_INFO	6	/* informational */
    #define	LOG_DEBUG	7	/* debug-level messages */
#else
    #include <syslog.h>
#endif

/* from previous NCP server code */
int debug_pause=0;
int debug_mask=0xFFFFFFFF;
int debug_all=0;

void debug_message(int debug_id, char* message)
{
    if ((debug_mask & debug_id)>0)
    {
        if (debug_pause==1)
        {
            char c;
            printf("paused - %s\n",message);
            c=getchar();
            if (c=='c') debug_pause=0;
        }
        else  printf("%s\n",message);
    }
}

void debug_int(int debug_id, char* message,int i)
{
    if ((debug_mask & debug_id)>0)
    {
        if (debug_pause==1)
        {
            char c;
            printf("paused - %s:%d\n",message,i);
            c=getchar();
            if (c=='c') debug_pause=0;
        }
        else  printf("%s:%d\n",message,i);
    }
}
/* end */

/* from ncp_server */

void
vsyslog_message(const char *fmt, ...)
{
    char msg[1024];
	va_list     ap;

    va_start(ap, fmt);
    vsnprintf(msg, 1024, fmt, ap);
    va_end(ap);
    syslog_message(msg);
}

void syslog_message(char *message)
{
    printf("%s\n",message);
#ifndef _WIN32
    setlogmask (LOG_UPTO (LOG_NOTICE));
    openlog ("ncp_server", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
    syslog (LOG_NOTICE,"%s",message);
    closelog ();
#endif
}
/* end */

static void err_doit(int, int, const char *, va_list);

#ifdef _WIN32
void
print_wsa_error(const char *fmt, ...)
{
/* for now, identical to print_error */
	va_list     ap;

#if 0
	/* can get at error number (eg for printing explanatory string) thus: */
	va_start(ap, fmt);
	printf("WS ERR: %d\n", va_arg(ap, int));
#endif

	va_start(ap, fmt);
	err_doit(0, LOG_ERR, fmt, ap);
	va_end(ap);
	return;
}
#endif

void
print_error(const char *fmt, ...)
{
	va_list     ap;

	va_start(ap, fmt);
	err_doit(0, LOG_ERR, fmt, ap);
	va_end(ap);
	return;
}

void
print_warning(const char *fmt, ...)
{
    va_list     ap;

    va_start(ap, fmt);
    err_doit(0, LOG_WARNING, fmt, ap);
    va_end(ap);
    return;
}

void
print_info(const char *fmt, ...)
{
#ifndef NO_PRINT_INFO
    va_list     ap;

    va_start(ap, fmt);
    err_doit(0, LOG_INFO, fmt, ap);
    va_end(ap);
#endif
    return;
}

void
print_debug(const char *fmt, ...)
{
    va_list     ap;

    va_start(ap, fmt);
    err_doit(0, LOG_DEBUG, fmt, ap);
    va_end(ap);
    return;
}

void
print_sys_error(const char *fmt, ...)
{
    va_list     ap;

    va_start(ap, fmt);
    err_doit(1, LOG_ERR, fmt, ap);
    va_end(ap);
    return;
}

void
print_sys_warning(const char *fmt, ...)
{
    va_list     ap;

    va_start(ap, fmt);
    err_doit(1, LOG_WARNING, fmt, ap);
    va_end(ap);
    return;
}

void
print_sys_info(const char *fmt, ...)
{
    va_list     ap;

    va_start(ap, fmt);
    err_doit(1, LOG_INFO, fmt, ap);
    va_end(ap);
    return;
}

#define ERR_MAXLINE 256

/** Print message and return to caller.
    Edited from Unix Network Programming, by WR Stevens.
*/

static void
err_doit(int errnoflag, int level, const char *fmt, va_list ap)
{
    int errno_save, n;
    char buf[ERR_MAXLINE + 1];

    errno_save = errno;     /* value caller might want printed */

    buf[0] = 0;
    switch (level) {
    case LOG_ERR: strcat(buf, "ERROR: "); break;
    case LOG_WARNING: strcat(buf, "WARNING: "); break;
    case LOG_INFO: strcat(buf, "INFO: "); break;
    case LOG_DEBUG: strcat(buf, "DEBUG: "); break;
    default: break;
    }

    n = strlen(buf);
    vsnprintf(buf + n, ERR_MAXLINE - 2 - n, fmt, ap);   /* safe */

    n = strlen(buf);
    if (errnoflag)
        snprintf(buf + n, ERR_MAXLINE - 2 - n, ": %s", strerror(errno_save));
    strcat(buf, "\n");

#if 0
    syslog(level, buf);
#endif

    if (level < LOG_DEBUG) {
        /* just output to stderr for now */
        fflush(stdout);     /* in case stdout and stderr are the same */
        fputs(buf, stderr);
        fflush(stderr);
    }

    return;
}

#ifdef DEBUG_MALLOC
/**
   wrapper for free to enable debugging
*/
static void *Malloc(size_t size)
{
    void *ptr = malloc(size);

    fprintf(stderr, "Malloc: %p\n", ptr);
    return ptr;
}

/**
   wrapper for free to enable debugging
*/
static void Free(void *ptr)
{
    fprintf(stderr, "Free: %p\n", ptr);
    free(ptr);
    fprintf(stderr, "Done free: %p\n", ptr);
}
#endif
