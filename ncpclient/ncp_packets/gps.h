/*-----------------------------------------------------------------------------
 * (c) CRFS Limited, 2011. All rights reserved.
 *
 * This software is the property of CRFS Limited and may not be copied or
 * reproduced otherwise than on to a single hard disk for backup or
 * archival purposes. The source code is confidential information and must
 * not be disclosed to third parties or used without the express written
 * permission of CRFS Limited.
 *
 * Filename : gps.h
 *---------------------------------------------------------------------------*/
#ifndef __GPS_H
#define __GPS_H

/** \file  gps.h

	Field and options to request\\retreive GPS data
*/

/** 
	\ingroup FIELD_GROUP
	\brief Retrieve GPS information

	This command can be executed on demand, it is automatically executed once per second by the node as a 
	background task, and the information returned in the 'STAT' packet. GPS information can also be attached 
	to all packets received from the node. See \ref FIELD_CONFIGURE for information on these options.

	See \ref GPS_GROUP 
	\fro
*/
#define FIELD_GPS                               PKEY_CONST('S','G','P','S')

/**
	@defgroup GPS_GROUP GPS Options

	This field provides the option to retrieve a number of GPS parameters from the embedded GPS module. The detailed 
	descriptions of each option are as follows.

	@{
*/
#define GPS_LATITUDE            PKEY_CONST('L','A','T','I') ///< \type{INT}\ro Latitude. Latitude / 1000000 = Decimal degrees
#define GPS_LONGITUDE           PKEY_CONST('L','O','N','G') ///< \type{INT}\ro Longitude. Latitude / 1000000 = Decimal degrees
#define GPS_SATELLITES          PKEY_CONST('S','A','T','S') ///< \type{INT}\ro Satellite's in view
#define GPS_FIX                 PKEY_CONST('G','F','I','X') ///< \type{INT}\ro Fix. 0 = No fix, 1 = Fix, 2 = DGPS Fix
#define GPS_STATUS              PKEY_CONST('S','T','A','T') ///< \type{INT}\ro Status. 0 = Not OK, 1 = Ok
#define GPS_UTIM                PKEY_CONST('U','T','I','M') ///< \type{INT}\ro GPS time in time_t format
#define GPS_SPEED               PKEY_CONST('S','P','E','E') ///< \type{INT}\ro Speed. Speed / 1000 = KPH
#define GPS_HEADING             PKEY_CONST('H','E','A','D') ///< \type{INT}\ro Heading. Heading / 100 = Degrees
#define GPS_ALTITUDE            PKEY_CONST('A','L','T','I') ///< \type{INT}\ro Altitude. Altitude / 1000 = Meters
#define GPS_DATETIME_STRING		PKEY_CONST('T','S','T','R') ///< \type{STRING}\ro Date\\time
/** @} */

#endif
