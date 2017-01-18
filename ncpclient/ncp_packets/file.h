/*-----------------------------------------------------------------------------
 * (c) CRFS Limited, 2011. All rights reserved.
 *
 * This software is the property of CRFS Limited and may not be copied or
 * reproduced otherwise than on to a single hard disk for backup or
 * archival purposes. The source code is confidential information and must
 * not be disclosed to third parties or used without the express written
 * permission of CRFS Limited.
 *
 * Filename : file.h
 *---------------------------------------------------------------------------*/
#ifndef __FILE_H
#define __FILE_H

/** \file  file.h

	Field and options to access files on the node

	\deprecated Will be removed, for security reasons, at a future time
*/

/** 
	\ingroup FIELD_GROUP
	\deprecated Will be removed, for security reasons, at a future time
	\brief Request manipulation of the file system

	This command will allow the retrieval and writing of files on the node. It has full access to the entire
	file system.

	See \ref FILE_GROUP 
	\frw
*/
#define FIELD_FILE                              PKEY_CONST('F','I','L','E')

/**
	@defgroup FILE_GROUP File options

	\brief 	Options available for \ref FIELD_FILE command

	\deprecated Will be removed, for security reasons, at a future time

	@{
 */
#define FILE_READ_FILENAME      PKEY_CONST('F','R','E','A') ///< \type{STRING}\rw Full path to file to read. File data returned in \ref FILE_DATA
#define FILE_WRITE_FILENAME		PKEY_CONST('F','W','R','I') ///< \type{STRING}\rw Full path of file to write. Data in \ref FILE_DATA is written.
#define FILE_DATA               PKEY_CONST('F','D','A','T') ///< \type{DATA_RAW}\rw File data
/** @} */

#endif
