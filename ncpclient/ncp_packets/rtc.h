/*-----------------------------------------------------------------------------
 * (c) CRFS Limited, 2011. All rights reserved.
 *
 * This software is the property of CRFS Limited and may not be copied or
 * reproduced otherwise than on to a single hard disk for backup or
 * archival purposes. The source code is confidential information and must
 * not be disclosed to third parties or used without the express written
 * permission of CRFS Limited.
 *
 * Filename : rtc.h
 *---------------------------------------------------------------------------*/
#ifndef __RTC_H
#define __RTC_H

/** \file  rtc.h

	Field and options to request\\retreive real time clock
*/

/** 
	\ingroup FIELD_GROUP
	\brief Update/Retrieve RTC settings

	See \ref DSP_RTC_GROUP 
	\frw
*/
#define FIELD_DSP_RTC                           PKEY_CONST('R','T','C','L')

/**
	@defgroup DSP_RTC_GROUP  DSP RTC options

	\brief 	List of \ref FIELD_DSP_RTC options

	\warning This function only works as expected if the source is configured to internal. i.e. If the node is 
			 configured to use the GPS as source then the time will immediatly be synced back to GPS time. 

	\code{.c}
		// Set the time
		packet_write(packet, PACKET_TYPE_DSPC, -1);
		// Add the RTC field
		packet_add_field(packet, FIELD_DSP_RTC, -1)
			// We want to set the date/time
			packet_add_param_int(packet, DSP_RTC_SET_TIME, 1);
			// Set it to 01/01/2015 @ 12:00
			packet_add_param_int(packet, DSP_RTC_NOW_UNIX_TIME, 1420113600);
		// Finish packet
		packet_write_complete();
	
	\endcode

	@{
*/
/**
	@name Function to perform

	Send one of \ref DSP_RTC_GET_TIME or \ref DSP_RTC_GET_TIME_FUTURE_NS to get the time now, or at some time in the future.
	\ref DSP_RTC_SET_TIME will set the time.
*/
///@{
#define DSP_RTC_GET_TIME   				PKEY_CONST('G','T','I','M') ///< \type{INT}\rw Get the current DSP time
#define DSP_RTC_GET_TIME_FUTURE_NS		PKEY_CONST('G','T','F','U') ///< \type{INT}\rw Get the DSP time in the future. The value passed is the a value in ns
#define DSP_RTC_SET_TIME                PKEY_CONST('S','T','I','M') ///< \type{INT}\rw Set the DSP time. Time is set based on DSP_RTC_NOW_* parameters
///@}
#define DSP_RTC_NOW_DATE                PKEY_CONST('N','D','A','T') ///< \type{TimeDMYC}\rw Date set\\returned
#define DSP_RTC_NOW_TIME                PKEY_CONST('N','T','I','M') ///< \type{TimeHMSC}\rw Time set\\returned
#define DSP_RTC_NOW_UNIX_TIME           PKEY_CONST('N','U','N','X') ///< \type{INT}\rw Date set\\returned in unix time_t format
#define DSP_RTC_NOW_NANO                PKEY_CONST('N','N','A','N') ///< \type{INT}\rw Nanoseconds
#define DSP_RTC_NOW_STR                 PKEY_CONST('N','S','T','R') ///< \type{STRING}\rw Date\\Time string
#define DSP_RTC_FUTURE_UNIX_TIME        PKEY_CONST('F','U','N','X') ///< \type{INT}\rw Future Date\\Time set\\returned in unix time_t format
#define DSP_RTC_FUTURE_NANO             PKEY_CONST('F','N','A','N') ///< \type{INT}\rw Nanoseconds
#define DSP_RTC_FUTURE_STRING           PKEY_CONST('F','S','T','R') ///< \type{STRING}\rw Future Date\\Time string
/** @} */

#endif
