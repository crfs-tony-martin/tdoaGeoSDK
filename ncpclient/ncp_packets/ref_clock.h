/*-----------------------------------------------------------------------------
 * (c) CRFS Limited, 2011. All rights reserved.
 *
 * This software is the property of CRFS Limited and may not be copied or
 * reproduced otherwise than on to a single hard disk for backup or
 * archival purposes. The source code is confidential information and must
 * not be disclosed to third parties or used without the express written
 * permission of CRFS Limited.
 *
 * Filename : ref_clock.h
 *---------------------------------------------------------------------------*/
#ifndef __REF_CLOCK_H
#define __REF_CLOCK_H

/** \file  ref_clock.h

	Field and options to request\\retreive reference clock
*/

/** 
	\ingroup FIELD_GROUP
	\brief Update/Retrieve reference clock source

	See \ref REF_CLOCK_GROUP 
	\frw
*/
#define FIELD_REF_CLOCK                         PKEY_CONST('R','C','L','K')


/**
	@defgroup REF_CLOCK_GROUP  Reference clock options

	The system relies on a reference clock to be able to perform a number of radio functions. This field provides the 
	options to configure the radio reference clock source. 

	\code{.c}
		// Select GPS as the reference clock
		packet_write(packet, PACKET_TYPE_DSPC, -1);
		// Add the reference clock field
		packet_add_field(packet, FIELD_REF_CLOCK, -1)
			packet_add_param_int(packet, REF_CLOCK_SOURCE_GPS, 1);
		// Finish packet
		packet_write_complete();
	\endcode

	@{
*/
/**
	@name Reference clock sources

	External sources that can be selected for referecence clock

	\note Only one of \ref REF_CLOCK_SOURCE_DAC, \ref REF_CLOCK_SOURCE_GPS, \ref REF_CLOCK_SOURCE_EXP1, \ref REF_CLOCK_SOURCE_EXP2 can
		  be specified at any given time.

	\warning If the reference clock selected is not available then the unit will not be able to perform certian radio
			 functions, an error will be returned.
*/
///@{
#define REF_CLOCK_SOURCE_DAC 	PKEY_CONST('R','D','A','C') ///< \type{INT}\rw Interal 10MHz AFC DAC
#define REF_CLOCK_SOURCE_GPS   	PKEY_CONST('R','G','P','S') ///< \type{INT}\rw 10MHz reference from GPS. GPS is more accurate than \ref REF_CLOCK_SOURCE_DAC
#define REF_CLOCK_SOURCE_EXP1  	PKEY_CONST('R','E','X','1') ///< \type{INT}\rw 10MHz external source supplied on EXP1
#define REF_CLOCK_SOURCE_EXP2   PKEY_CONST('R','E','X','2') ///< \type{INT}\rw 10MHz external source supplied on EXP1
///@}
/**
	\brief \type{INT}\rw Output reference clock on expansion port 1 (EXP 1)

	0 = None (Default), 1 = GPS, 2 = 1 Pulse per second (1PPS), 3 = Radio clock
*/
#define REF_CLOCK_OUT_EXP1                      PKEY_CONST('O','E','X','1')
/**
	\brief \type{INT}\rw Output reference clock on expansion port 2 (EXP 2)

	0 = None (Default), 1 = GPS, 2 = 1 Pulse per second (1PPS), 3 = Radio clock
*/
#define REF_CLOCK_OUT_EXP2                      PKEY_CONST('O','E','X','2')
/**
	\brief \type{INT}\rw Disable time transmission

	By default a time code is transmitted when either \ref REF_CLOCK_OUT_EXP1 or \ref REF_CLOCK_OUT_EXP2 is set. This
	can be used to disable the transmission.

	0 = Off, 1 = On
*/
#define REF_CLOCK_DISABLE_TIME_TRANSMISSION     PKEY_CONST('R','D','T','T')
/**
	\brief \type{INT}\rw Radio reference AFC DAC

	\warning Change this setting if you wish to move away from the calibrated AFC DAC setting. The nominal value of the 
 		     reference clock DAC is calibrated during production, this is the default state following system power cycle.
	 		 Setting the reference clock DAC to a user value will override the production cal value until the next 
			 system power cycle
*/
#define REF_CLOCK_DAC_SETTING                   PKEY_CONST('S','D','A','C')
/** @} */


#endif
