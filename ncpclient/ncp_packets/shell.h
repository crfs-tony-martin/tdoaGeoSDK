/*-----------------------------------------------------------------------------
 * (c) CRFS Limited, 2011. All rights reserved.
 *
 * This software is the property of CRFS Limited and may not be copied or
 * reproduced otherwise than on to a single hard disk for backup or
 * archival purposes. The source code is confidential information and must
 * not be disclosed to third parties or used without the express written
 * permission of CRFS Limited.
 *
 * Filename : shell.h
 *---------------------------------------------------------------------------*/
#ifndef __SHELL_H
#define __SHELL_H

/** \file  shell.h

	Field and options to request and retrieve output from a performed shell command
*/

/** 
	\ingroup FIELD_GROUP
	\deprecated Will be removed, for security reasons
	\brief Request OS command to be executed

	This command allows any OS binary to be excuted. The command is run as root so has full access to all
	capablilites of the underlying OS.

	See \ref SHELL_GROUP 
	\frw
*/
#define FIELD_SHELL                             PKEY_CONST('S','H','E','L')

/**
	@defgroup SHELL_GROUP Shell options

	\brief 	Options available for \ref FIELD_SHELL command

	\deprecated Will be removed, for security reasons

	The command in \ref SHELL_EXECUTE is executed with root priviliges and all output, including stderr, is captured
	and returned in \ref SHELL_EXECUTE_RESULT

	@{
 */
#define SHELL_EXECUTE           PKEY_CONST('E','X','E','C') ///< \type{STRING}\wo Command to execute 
#define SHELL_EXECUTE_RESULT	PKEY_CONST('X','R','E','S')	///< \type{STRING}\ro Command result
/** @} */

#endif
