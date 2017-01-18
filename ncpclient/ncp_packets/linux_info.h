/*-----------------------------------------------------------------------------
 * (c) CRFS Limited, 2011. All rights reserved.
 *
 * This software is the property of CRFS Limited and may not be copied or
 * reproduced otherwise than on to a single hard disk for backup or
 * archival purposes. The source code is confidential information and must
 * not be disclosed to third parties or used without the express written
 * permission of CRFS Limited.
 *
 * Filename : linux_info.h
 *---------------------------------------------------------------------------*/
#ifndef __LINUX_INFO_H
#define __LINUX_INFO_H

/** \file  linux_info.h

	Field and options to request\\retreive Linux information
*/

/** 
	\ingroup FIELD_GROUP
	\brief Retrieve static information about the embedded linux system

	See \ref LINUX_INFO_GROUP 
	\frw
*/
#define FIELD_LINUX_INFO                        PKEY_CONST('L','I','N','F')

/**
	@defgroup LINUX_INFO_GROUP Linux Information options

	The Node is running an embedded Linux operating system based on Debian (kernel version 2.6). This field provides the 
	option to retrieve static information from the kernel of the Linux OS. Note that not all information is available on 
	all types of node. The detailed descriptions of each option are as follows.
	
	@{
 */
#define LINUX_INFO_VERSION                  	PKEY_CONST('V','E','R','S') ///< \type{STRING}\ro Linux version
#define LINUX_INFO_RELEASE                  	PKEY_CONST('R','E','L','E') ///< \type{STRING}\ro Linux release
#define LINUX_INFO_MACHINE                  	PKEY_CONST('M','A','C','H') ///< \type{STRING}\ro Machine information
#define LINUX_INFO_SYSNAME                      PKEY_CONST('S','Y','S','N') ///< \type{STRING}\ro System name
#define LINUX_INFO_NODENAME                     PKEY_CONST('N','O','D','N') ///< \type{STRING}\ro Node Name
#define LINUX_INFO_CLOCK_TICKS                  PKEY_CONST('C','L','K','T') ///< \type{INT}\ro Linux clock ticks
#define LINUX_INFO_REALTIME                     PKEY_CONST('R','T','O','S') ///< \type{INT}\ro Linux supports real-time extensions
#define LINUX_INFO_REALTIME_THREADS             PKEY_CONST('R','T','T','H') ///< \type{INT}\ro Linux supports real-time threads
#define LINUX_INFO_THREAD_PRIORITY_SCHEDULING   PKEY_CONST('T','P','R','I') ///< \type{INT}\ro Linux thread priority scheduling
#define LINUX_INFO_MAC_ADDRESS                  PKEY_CONST('M','A','C','A') ///< \type{INT}\ro MAC address of the hardware
/** @} */

#endif
