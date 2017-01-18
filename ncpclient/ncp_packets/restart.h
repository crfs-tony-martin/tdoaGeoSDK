/*-----------------------------------------------------------------------------
 * (c) CRFS Limited, 2011. All rights reserved.
 *
 * This software is the property of CRFS Limited and may not be copied or
 * reproduced otherwise than on to a single hard disk for backup or
 * archival purposes. The source code is confidential information and must
 * not be disclosed to third parties or used without the express written
 * permission of CRFS Limited.
 *
 * Filename : restart.h
 *---------------------------------------------------------------------------*/
#ifndef __RESTART_H
#define __RESTART_H

/** \file  restart.h

	Field and options to request a restart of node functionality
*/

/** 
	\ingroup FIELD_GROUP
	\brief Request a restart of NCPd or Node

	See \ref RESTART_GROUP 
	\fro
*/
#define FIELD_RESTART                           PKEY_CONST('R','E','S','T')

/**
	@defgroup RESTART_GROUP Restart options

	This field provides the two options to restart either the on-board Linux operating system or just the NCP server.
	
	\warning Command returns no response as it is executed immediatly
	@{
 */
#define RESTART_NCP         PKEY_CONST('R','N','C','P') ///< \type{INT}\wo Restart \b just the NCP server
#define RESTART_LINUX		PKEY_CONST('R','L','I','N') ///< \type{INT}\wo Restart the \b entire node
/** @} */


#endif
