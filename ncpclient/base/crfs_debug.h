/*-----------------------------------------------------------------------------
 * (c) CRFS Limited, 2011. All rights reserved.
 *
 * This software is the property of CRFS Limited and may not be copied or
 * reproduced otherwise than on to a single hard disk for backup or
 * archival purposes. The source code is confidential information and must
 * not be disclosed to third parties or used without the express written
 * permission of CRFS Limited.
 *
 * Filename : error.h
 * Author : Dan Timson
 *
 * Purpose: Error reporting
 *
 * Description:
 *---------------------------------------------------------------------------*/

/**
   \file error.h Facilities for internal error reporting.

   Needed so as to abtract away from particular output streams.
 */

#ifndef ERROR_H_INCLUDED
#define ERROR_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif
#if 0
}
#endif

/* from previous NCP server code */
void debug_message(int debug_id, char* message);
void debug_int(int debug_id, char* message,int i);
extern int debug_pause;
extern int debug_mask;
extern int debug_all;

void vsyslog_message(const char *fmt, ...);
void syslog_message(char *message);
/* end */

#define DEBUG_ERROR print_error
#define DEBUG_WARN  print_warning
#define DEBUG_DEBUG print_debug

#if 0
typedef void (*PRINT_FUNC)(const char*);
_DLL int32_t _STDCALL register_error_print_callback(PRINT_FUNC pf);
_DLL int32_t _STDCALL clear_error_print_callbacks();
#endif

#ifdef DEBUG_MALLOC
static void *Malloc(size_t size);
static void Free(void *ptr);
#else
#define Malloc(X) malloc(X)
#define Free(X) free(X)
#endif

/* keep it simple for now */

#ifdef _WIN32
void print_wsa_error(const char *fmt, ...);
#if _MSC_VER < 1800
#define snprintf _snprintf
#endif
#endif

void print_error(const char *fmt, ...);
void print_warning(const char *fmt, ...);
void print_info(const char *fmt, ...);
void print_debug(const char *fmt, ...);

void print_sys_error(const char *fmt, ...);
void print_sys_warning(const char *fmt, ...);
void print_sys_info(const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* ERROR_H_INCLUDED */
