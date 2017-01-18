/*-----------------------------------------------------------------------------
 * (c) CRFS Limited, 2011. All rights reserved.
 *
 * This software is the property of CRFS Limited and may not be copied or
 * reproduced otherwise than on to a single hard disk for backup or
 * archival purposes. The source code is confidential information and must
 * not be disclosed to third parties or used without the express written
 * permission of CRFS Limited.
 *
 * Filename : linux_monitor.h
 *---------------------------------------------------------------------------*/
#ifndef __LINUX_MONITOR_H
#define __LINUX_MONITOR_H

/** \file  linux_monitor.h

	Field and options to request\\retreive linux monitor information
*/

/** 
	\ingroup FIELD_GROUP
	\brief Retrieve dynamic status information about the embedded linux system

	See \ref LINUX_MONITOR_GROUP 
	\frw
*/
#define FIELD_LINUX_MONITOR                     PKEY_CONST('L','M','O','N')

/**
	@defgroup LINUX_MONITOR_GROUP Linux Monitor options

	The node is running an embedded Linux operating system based on Debian (kernel version 2.6). This field provides the 
	option to retrieves dynamic information from the kernel of the Linux OS. Note that not all information is available 
	on all types of Node. The detailed descriptions of each option are as follows.

	@{
 */
#define LINUX_MONITOR_MEM_FREE      PKEY_CONST('M','F','R','E') ///< \type{INT}\ro Linux system free memory
#define LINUX_MONITOR_MEM_USED      PKEY_CONST('M','U','S','E') ///< \type{INT}\ro Linux system used memory
#define LINUX_MONITOR_USER_SEC      PKEY_CONST('U','S','E','C') ///< \type{INT}\ro Time spent executing user instructions (seconds)
#define LINUX_MONITOR_USER_USEC		PKEY_CONST('U','U','S','E') ///< \type{INT}\ro Time spent executing user instructions (usec)
#define LINUX_MONITOR_OS_SEC        PKEY_CONST('O','S','E','C') ///< \type{INT}\ro Time spent in operating system code on behalf of processes(sec)
#define LINUX_MONITOR_OS_USEC       PKEY_CONST('O','U','S','E') ///< \type{INT}\ro Time spent in operating system code on behalf of processes(usec)
#define LINUX_MONITOR_MAXRSS        PKEY_CONST('M','R','S','S') ///< \type{INT}\ro The maximum resident set size used, in kilobytes. That is, the maximum number of kilobytes of physical memory that processes used simultaneously
#define LINUX_MONITOR_IXRSS         PKEY_CONST('X','R','S','S') ///< \type{INT}\ro An integral value expressed in kilobytes times ticks of execution, which indicates the amount of memory used by text that was shared with other processes
#define LINUX_MONITOR_IDRSS         PKEY_CONST('D','R','S','S') ///< \type{INT}\ro An integral value expressed the same way, which is the amount of unshared memory used for data
#define LINUX_MONITOR_ISRSS         PKEY_CONST('I','R','S','S') ///< \type{INT}\ro An integral value expressed the same way, which is the amount of unshared memory used for stack space
#define LINUX_MONITOR_MINFLT        PKEY_CONST('M','I','N','F') ///< \type{INT}\ro The number of page faults which were serviced without requiring any I/O
#define LINUX_MONITOR_MAJFLT        PKEY_CONST('M','A','J','F') ///< \type{INT}\ro The number of page faults which were serviced by doing I/O
#define LINUX_MONITOR_NSWAP         PKEY_CONST('N','S','W','A') ///< \type{INT}\ro The number of times processes was swapped entirely out of main memory
#define LINUX_MONITOR_INBLOCK       PKEY_CONST('I','N','B','L') ///< \type{INT}\ro The number of times the file system had to read from the disk on behalf of processes
#define LINUX_MONITOR_OUBLOCK       PKEY_CONST('O','U','B','L') ///< \type{INT}\ro The number of times the file system had to write to the disk on behalf of processes
#define LINUX_MONITOR_MSGSND        PKEY_CONST('M','S','G','S') ///< \type{INT}\ro Number of IPC messages sent
#define LINUX_MONITOR_MSGRCV        PKEY_CONST('M','S','G','R') ///< \type{INT}\ro Number of IPC messages received
#define LINUX_MONITOR_NSIGNALS      PKEY_CONST('N','S','I','G') ///< \type{INT}\ro Number of signals received
#define LINUX_MONITOR_NVCSW         PKEY_CONST('N','V','C','S') ///< \type{INT}\ro The number of times processes voluntarily invoked a context switch (usually to wait for some service)
#define LINUX_MONITOR_NIVCSW        PKEY_CONST('N','I','V','C') ///< \type{INT}\ro The number of times an involuntary context switch took place (because a time slice expired, or another process of higher priority was scheduled)
/** @} */


#endif
