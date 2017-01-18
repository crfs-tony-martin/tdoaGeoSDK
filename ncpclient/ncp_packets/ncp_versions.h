/*-----------------------------------------------------------------------------
 * (c) CRFS Limited, 2011. All rights reserved.
 *
 * This software is the property of CRFS Limited and may not be copied or
 * reproduced otherwise than on to a single hard disk for backup or
 * archival purposes. The source code is confidential information and must
 * not be disclosed to third parties or used without the express written
 * permission of CRFS Limited.
 *
 * Filename : ncp_versions.h
 *---------------------------------------------------------------------------*/
#ifndef __NCP_VERSIONS_H
#define __NCP_VERSIONS_H

/** \file  ncp_versions.h

	Field and options to request\\retreive NCP version information
*/

/** 
	\ingroup FIELD_GROUP
	\brief Retrieve static information about NCP

	See \ref NCP_VERSIONS_GROUP 
	\fro
*/
#define FIELD_NCP_VERSIONS                      PKEY_CONST('N','C','P','V')

/**
	@defgroup NCP_VERSIONS_GROUP NCP Versions options

	Node Communication Protocol (NCP) uses standard TCP socket connections between client systems and RFeye units. This 
	field provides the option to retrieve static version information about NCP. 

	@{
 */
#define NCP_VERSIONS_SERVER_MAJOR       PKEY_CONST('N','V','M','A') ///< \type{INT}\ro NCP major version
#define NCP_VERSIONS_SERVER_MINOR       PKEY_CONST('N','V','M','I') ///< \type{INT}\ro NCP minor version
#define NCP_VERSIONS_SERVER_RELEASE		PKEY_CONST('R','E','L','V') ///< \type{STRING}\ro NCP server version
#define NCP_VERSIONS_CLIENT             PKEY_CONST('C','L','I','V') ///< \type{STRING}\ro NCP client version
#define NCP_VERSIONS_NCP_PACKETS_H      PKEY_CONST('P','A','K','H') ///< \type{STRING}\ro NCP packets version
#define NCP_VERSIONS_PACKETS            PKEY_CONST('P','A','K','V') ///< \type{STRING}\ro NCP packets version
#define NCP_VERSIONS_COM_INTERFACE      PKEY_CONST('C','O','M','V') ///< \type{STRING}\ro NCP COM interface version
/** @} */


#endif
