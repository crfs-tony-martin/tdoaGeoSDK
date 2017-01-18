/*-----------------------------------------------------------------------------
 * (c) CRFS Limited, 2011. All rights reserved.
 *
 * This software is the property of CRFS Limited and may not be copied or
 * reproduced otherwise than on to a single hard disk for backup or
 * archival purposes. The source code is confidential information and must
 * not be disclosed to third parties or used without the express written
 * permission of CRFS Limited.
 *
 * Filename : power.h
 *---------------------------------------------------------------------------*/
#ifndef __POWER_H
#define __POWER_H

/// \cond No_Documentation

/** \file  power.h
*/

/** 
	\ingroup FIELD_GROUP
	\brief Update/retrieve radio power modes 

	See \ref RADIO_AUTO_MODE_GROUP 
	\frw
*/
#define FIELD_RADIO_AUTO_MODE                   PKEY_CONST('R','A','U','T')
/** 
	\ingroup FIELD_GROUP
	\brief Update/retrieve power management of the radio

	See \ref RADIO_AUTO_TIMEOUT_GROUP 
	\frw
*/
#define FIELD_RADIO_AUTO_TIMEOUT                PKEY_CONST('R','A','T','O')
/** 
	\ingroup FIELD_GROUP
	\todo
*/
#define FIELD_RADIO_POWER_UP                    PKEY_CONST('R','P','U','P')
/** 
	\ingroup FIELD_GROUP
	\todo
*/
#define FIELD_RADIO_POWER_DOWN                  PKEY_CONST('R','P','D','O')

/**
	@defgroup RADIO_AUTO_TIMEOUT_GROUP Radio power parameter list

	\brief 	List of \ref FIELD_RADIO_AUTO_TIMEOUT parameters
	@{
*/
/**
	\brief \type{INT}\rw Radio power managment

	Enable or Disable radio power managment. If power managment is enabled then the radio will power down if no
	command has been received for 1 minute.

	0 = Off, 1 = On(Default)

*/
#define RADIO_AUTO_POWER_MODE       PKEY_CONST('R','P','O','W')
/**
	\brief \type{INT}\rw Radio thermal managment

	Enable or Disable radio thermal managment. If thermal managment is enabled then the radio will power \b up if the
	temperature of the device gets to \b cold. 

	0 = Off, 1 = On(Default)
*/
#define RADIO_AUTO_THERMAL_MODE		PKEY_CONST('R','T','H','M')

/// \cond No_Documentation
/** FIELD_RADIO_AUTO_TIMEOUT */
#define RADIO_AUTO_POWER_TIMEOUT                PKEY_CONST('R','T','I','M')/*[int32,WO]         radio power mgmt timeout in secs, 0-65535, default 60s */
/// \endcond
/** @} */

/// \endcond

#endif
