/*-----------------------------------------------------------------------------
 * (c) CRFS Limited, 2011. All rights reserved.
 *
 * This software is the property of CRFS Limited and may not be copied or
 * reproduced otherwise than on to a single hard disk for backup or
 * archival purposes. The source code is confidential information and must
 * not be disclosed to third parties or used without the express written
 * permission of CRFS Limited.
 *
 * Filename : wideband.h
 *---------------------------------------------------------------------------*/
#ifndef __WIDEBAND_H
#define __WIDEBAND_H
/** \file wideband.h

	Request/retrieve information from a wideband detector
*/

/** 
	\ingroup FIELD_GROUP
	\brief Request/retrieve data for a Low-Band array

	This command requires the CRFS Wideband hardware

	See \ref WIDEBAND_GROUP 
	\frw
*/
#define FIELD_WIDEBAND              PKEY_CONST('W','I','D','E')

/**
	@defgroup WIDEBAND_GROUP Wideband options

	\brief 	List of \ref FIELD_WIDEBAND options

	\warning \b EXPERIMENTAL The wideband command and options are all subject to change.

	\note Currently it is only possible to request data from one of the six channels that are available.

	@{
*/
#define WIDEBAND_CHANNEL_1_6        PKEY_CONST('C','_','_','0') ///< \type{INT}\rw Request data from Channel 1 (1 - 6 GHz)
#define WIDEBAND_CHANNEL_6_9        PKEY_CONST('C','_','_','1') ///< \type{INT}\rw Request data from Channel 2 (6 - 9 GHz)
#define WIDEBAND_CHANNEL_9_12       PKEY_CONST('C','_','_','2') ///< \type{INT}\rw Request data from Channel 3 (9 - 12 GHz)
#define WIDEBAND_CHANNEL_12_15      PKEY_CONST('C','_','_','3') ///< \type{INT}\rw Request data from Channel 4 (12 - 15 GHz)
#define WIDEBAND_CHANNEL_15_18      PKEY_CONST('C','_','_','4') ///< \type{INT}\rw Request data from Channel 5 (15 - 18 GHz)
#define WIDEBAND_CHANNEL_18P        PKEY_CONST('C','_','_','5') ///< \type{INT}\rw Request data from Channel 6 (18+ GHz)
#define WIDEBAND_BAND_REJECTION		PKEY_CONST('R','E','J','E') ///< \type{INT}\rw Do not detect in the 1.8GHz to 2.4GHz band
#define WIDEBAND_NUMBER_SAMPLES     PKEY_CONST('S','A','M','P') ///< \type{INT}\rw Number of samples to capture
#define WIDEBAND_DATA               PKEY_CONST('D','A','T','A') ///< \type{INT}\ro Returned data
#define WIDEBAND_STREAM             PKEY_CONST('S','T','R','E') ///< \type{INT}\ro Stream wideband data
/** @} */

#endif
