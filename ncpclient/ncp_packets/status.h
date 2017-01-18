/*-----------------------------------------------------------------------------
 * (c) CRFS Limited, 2011. All rights reserved.
 *
 * This software is the property of CRFS Limited and may not be copied or
 * reproduced otherwise than on to a single hard disk for backup or
 * archival purposes. The source code is confidential information and must
 * not be disclosed to third parties or used without the express written
 * permission of CRFS Limited.
 *
 * Filename : status.h
 *---------------------------------------------------------------------------*/
#ifndef __STATUS_H
#define __STATUS_H

/** \file  status.h

	Field and options to returned in a status packet
*/

/** 
	\ingroup FIELD_GROUP
	\brief Returns current status of the node

	This field is returned once a second and can be enabled using
	\ref FIELD_CONFIGURE

	See \ref STATUS_GROUP 
	\fro
*/
#define FIELD_STATUS_INFORMATION	            PKEY_CONST('S','T','A','T')

/**
	@defgroup STATUS_GROUP Status options

	This field provides the options to retrieve general status information from the system. 
	
	@{
*/
#define STATUS_OPEN_CONNECTIONS             PKEY_CONST('O','C','O','N') ///< \type{INT}\ro Number of open connections
#define STATUS_CONNECTION_IP_ADDR           PKEY_CONST('C','A','D','D') ///< \type{STRING}\ro Connected IP addess list. CSV format
#define STATUS_CLIENT_IDS                   PKEY_CONST('C','I','D','S') ///< \type{STRING}\ro connected Client ID's. CSV format
#define STATUS_GPS_RAW_STRING               PKEY_CONST('R','G','P','S') ///< \type{STRING}\ro Raw GPS string. Internal format.
#define STATUS_EXP_1                        PKEY_CONST('E','X','P','1') ///< \type{STRING}\ro No longer supported
#define STATUS_EXP_2                        PKEY_CONST('E','X','P','2') ///< \type{STRING}\ro No longer supported
#define STATUS_CONFIG_FILE_PRESENT          PKEY_CONST('C','O','N','P')	///< \type{INT}\ro Configuration file present. 1 = Yes, 0 = No
#define STATUS_CALIBRATION_SUPPORTED        PKEY_CONST('C','A','L','S') ///< \type{INT}\ro No longer supported
#define STATUS_EXCLUSIVE_IP                 PKEY_CONST('E','L','I','P')	///< \type{STRING}\ro IP address of client holding exclusive lock
#define STATUS_EXCLUSIVE_ID                 PKEY_CONST('E','L','I','D')	///< \type{STRING}\ro Client ID holding exclusive lock
#define STATUS_EXCLUSIVE_LOCK               PKEY_CONST('E','L','O','C') ///< \type{INT}\ro Lock status. 0 = Not Locked, 1 = Locked, 2 = Locked to you
/** @} */

#endif
