/*-----------------------------------------------------------------------------
 * (c) CRFS Limited, 2011. All rights reserved.
 *
 * This software is the property of CRFS Limited and may not be copied or
 * reproduced otherwise than on to a single hard disk for backup or
 * archival purposes. The source code is confidential information and must
 * not be disclosed to third parties or used without the express written
 * permission of CRFS Limited.
 *
 * Filename : monitors.h
 *---------------------------------------------------------------------------*/
#ifndef __MONITORS_H
#define __MONITORS_H

/** \file  monitors.h

	Fields and options to request\\retreive HW monitor information
*/

/**********************************************************************************************************************/
/** 
	\ingroup FIELD_GROUP
	\brief Retrieve monitor information from the DSP radio subsystem

	This command can be executed on demand, however, it is automatically executed peridodically by the node as a 
	background task, the information is returned in a 'STAT' packet. See FIELD_CONFIGURE to enable stat packets.

	See \ref RADIO_MONITOR_GROUP 
	\fro
*/
#define FIELD_RADIO_MONITOR                     PKEY_CONST('R','M','O','N')

/**
	@defgroup RADIO_MONITOR_GROUP Radio monitor options

	The Radio DSP controls all functions on the RFeye radio and provides a control interface to the host baseband board.  
	The radio is entirely controlled and configured via a register interface on the radio board. This field provides the 
	option to retrieve radio monitoring information from the DSP radio subsystem.	
	
	@{
*/
#define RADIO_MONITOR_TEMPERATURE	PKEY_CONST('T','E','M','P')	///< \type{INT}\ro Temperature (<SUP>o</SUP>C)
#define RADIO_MONITOR_VOLT_1        PKEY_CONST('V','O','L','1') ///< \type{INT}\ro 3.3v Supply rail (mV)
#define RADIO_MONITOR_VOLT_2        PKEY_CONST('V','O','L','2') ///< \type{INT}\ro 6.0v Supply rail (mV)
#define RADIO_MONITOR_TICK          PKEY_CONST('T','I','C','K') ///< \type{INT}\ro Tick @ 1PPS
/** @} */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/** 
	\ingroup FIELD_GROUP
	\brief Retrieve monitor information from the DSP baseband subsystem

	This command can be executed on demand, however, it is automatically executed peridodically by the node as a 
	background task, the information is returned in a 'STAT' packet. See FIELD_CONFIGURE to enable stat packets.

	See \ref BASEBAND_MONITOR_GROUP 
	\fro
*/
#define FIELD_BASEBAND_MONITOR                  PKEY_CONST('B','M','O','N')

/**
	@defgroup BASEBAND_MONITOR_GROUP Baseband monitor options

	The baseband DSP is based around a microblaze processor attached to an IBM PLB (processor local bus) multi-master, 
	fully arbitrated internal communications bus. This field provides the option to retrieve baseband monitoring 
	information from the DSP baseband subsystem.

 	@{
*/
#define BASEBAND_MONITOR_TEMPERATURE        PKEY_CONST('T','E','M','P') ///< \type{INT}\ro Temperature (<SUP>o</SUP>C)
#define BASEBAND_MONITOR_VOLT_1             PKEY_CONST('V','O','L','1') ///< \type{INT}\ro 3.3v Supply rail (mV)
#define BASEBAND_MONITOR_VOLT_2             PKEY_CONST('V','O','L','2') ///< \type{INT}\ro 3.8v Supply rail (mV)
#define BASEBAND_MONITOR_VOLT_3             PKEY_CONST('V','O','L','3') ///< \type{INT}\ro 5.0v Supply rail (mV)
#define BASEBAND_MONITOR_VOLT_4             PKEY_CONST('V','O','L','4') ///< \type{INT}\ro 6.0v Supply rail (mV)
#define BASEBAND_MONITOR_INPUT_CURRENT		PKEY_CONST('I','I','N','P') ///< \type{INT}\ro Supply current (mA)
#define BASEBAND_MONITOR_INPUT_VOLTAGE      PKEY_CONST('V','I','N','P') ///< \type{INT}\ro Externel Supply rail (mV)
/** @} */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/** 
	\ingroup FIELD_GROUP
	\brief Retrieve monitor information from a Low-Band array

	\note Returns an ACK packet containing an \ref ERROR_NO_DEVICE if no Low-Band array is found
	See \ref WW_MONITOR_GROUP 
	\fro
*/
#define FIELD_WW_MONITOR                        PKEY_CONST('W','M','O','N')

/**
	@defgroup WW_MONITOR_GROUP  Low-Band array options

	This field provides the options to retrieve monitoring information from the low-band subsystem. 

	@{
*/
#define WW_MONITOR_TEMPERATURE		PKEY_CONST('T','E','M','P') ///< \type{INT}\ro Temperature (<SUP>o</SUP>C)
#define WW_MONITOR_3V3              PKEY_CONST('V','O','L','1') ///< \type{INT}\ro 3.3v Supply rail (mV)
#define WW_MONITOR_1V5              PKEY_CONST('V','O','L','2') ///< \type{INT}\ro 1.5v Supply rail (mV)
#define WW_MONITOR_5V               PKEY_CONST('V','O','L','3') ///< \type{INT}\ro 5v Supply rail (mV)r
#define WW_MONITOR_6V               PKEY_CONST('V','O','L','4') ///< \type{INT}\ro 6v Supply rail (mV)
#define WW_MONITOR_12V              PKEY_CONST('V','O','L','5') ///< \type{INT}\ro 12v Supply rail(mV)
#define WW_CURRENT_WEST             PKEY_CONST('C','U','R','1') ///< \type{INT}\ro West current (mA)
#define WW_CURRENT_EAST             PKEY_CONST('C','U','R','2') ///< \type{INT}\ro East current (mA)
#define WW_CURRENT_SOUTH            PKEY_CONST('C','U','R','3') ///< \type{INT}\ro South current (mA)
#define WW_CURRENT_NORTH            PKEY_CONST('C','U','R','4') ///< \type{INT}\ro North current (mA)
#define WW_CURRENT_SENSE            PKEY_CONST('C','U','R','5') ///< \type{INT}\ro Sense current (mA)
/** @} */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/** 
	\ingroup FIELD_GROUP
	\brief Retrieve status information of the PDDM hardware
	\note Returns an ACK packet containing an \ref ERROR_NO_DEVICE if no PDDM is found
	See \ref PDDM_MONITOR_GROUP 

	\fro
*/
#define FIELD_PDDM_MONITOR                      PKEY_CONST('P','M','O','N')

/**
	@defgroup PDDM_MONITOR_GROUP  PDDM monitor options

	This field provides the options to retrieve monitoring information from the Power and Data Distribution Module 
	(PDDM, optionally fitted to some RFeye systems).

 	@{
*/
#define PDDM_MONITOR_3V3                PKEY_CONST('P','D','D','1') ///< \type{INT}\ro 3.3v Supply (mV)
#define PDDM_MONITOR_3V3_STANDBY        PKEY_CONST('P','D','D','2') ///< \type{INT}\ro 3.3v Standby (mV)
#define PDDM_MONITOR_AUX                PKEY_CONST('P','D','D','3') ///< \type{INT}\ro Aux (mV)
#define PDDM_MONITOR_SOLAR              PKEY_CONST('P','D','D','4') ///< \type{INT}\ro Solar (mV)
#define PDDM_MONITOR_BATTERY_STATUS		PKEY_CONST('P','D','D','5') ///< \type{INT}\ro Battery Status. 0 = Float mode, 2 = Bulk charge, 3 = Boost Mood
#define PDDM_MONITOR_TEMP               PKEY_CONST('P','D','D','6') ///< \type{INT}\ro Temperature (<SUP>o</SUP>C)
#define PDDM_MONITOR_15V_COMBO          PKEY_CONST('P','D','D','7') ///< \type{INT}\ro 15v Combo (mV)
#define PDDM_MONITOR_15V_AUX            PKEY_CONST('P','D','D','8') ///< \type{INT}\ro 15v Aux (mV)
#define PDDM_MONITOR_15V_POE_AUX        PKEY_CONST('P','D','D','9') ///< \type{INT}\ro 15v POE Aux (mv)
#define PDDM_MONITOR_15V_POE            PKEY_CONST('P','D','D','C') ///< \type{INT}\ro 15v POE (mV)
#define PDDM_MONITOR_WIRE_OR_POWER      PKEY_CONST('P','D','D','0') ///< \type{INT}\ro Wire or power (mV)
#define PDDM_MONITOR_VBAT               PKEY_CONST('P','D','D','A') ///< \type{INT}\ro Battery voltage (mV)
#define PDDM_MONITOR_POE_POWER          PKEY_CONST('P','D','D','B') ///< \type{INT}\ro POE Power (mV)
/** @} */
/**********************************************************************************************************************/

/** 
	\ingroup FIELD_GROUP
	\brief Retrieve status information of the Wideband hardware
	\note Returns an ACK packet containing an \ref ERROR_NO_DEVICE if no PDDM is found

	This command requires the CRFS Wideband detector hardware

	See \ref WIDEBAND_MONITOR_GROUP 
	\fro
*/
#define FIELD_WIDEBAND_MONITOR              	PKEY_CONST('W','B','M','O')

/**
	@defgroup WIDEBAND_MONITOR_GROUP  Wideband options

	The wideband detector is a staring receiver which provides frequency banded power detection over the range 1GHz to 
	beyond 18GHz.This field provides the options to retrieve monitor information from the wideband detector.

 	@{
*/
#define WBD_MONITOR_TEMP_1				PKEY_CONST('T','E','M','1') ///< \type{INT}\ro Temperature (<SUP>o</SUP>C)
#define WBD_MONITOR_TEMP_2              PKEY_CONST('T','E','M','2') ///< \type{INT}\ro Temperature (<SUP>o</SUP>C)
#define WBD_MONITOR_5V                  PKEY_CONST('V','O','L','5') ///< \type{INT}\ro 5v Supply (mV)
#define WBD_MONITOR_CURRENT             PKEY_CONST('C','U','R','R') ///< \type{INT}\ro Current (mA)
#define WBD_MONITOR_3V3                 PKEY_CONST('V','3','V','3') ///< \type{INT}\ro 3.3v Supply (mV)
#define WBD_MONITOR_V18A                PKEY_CONST('V','1','8','A') ///< \type{INT}\ro 1.8v Analogue Supply (mV)
#define WBD_MONITOR_V18D                PKEY_CONST('V','1','8','D') ///< \type{INT}\ro 1.8v Supply (mV)
#define WBD_MONITOR_V12                 PKEY_CONST('V','1','2',' ') ///< \type{INT}\ro 12v Supply (mV)
/** @} */
/**********************************************************************************************************************/


#endif











