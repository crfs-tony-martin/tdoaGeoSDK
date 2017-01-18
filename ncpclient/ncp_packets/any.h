/*-----------------------------------------------------------------------------
 * (c) CRFS Limited, 2011. All rights reserved.
 *
 * This software is the property of CRFS Limited and may not be copied or
 * reproduced otherwise than on to a single hard disk for backup or
 * archival purposes. The source code is confidential information and must
 * not be disclosed to third parties or used without the express written
 * permission of CRFS Limited.
 *
 * Filename : any.h
 *---------------------------------------------------------------------------*/
#ifndef __ANY_H
#define __ANY_H

/** \file  any.h

	Options that are available to multiple fields
*/

/**
	@defgroup ANY_INPUT_GROUP  "Any" input option's

	\brief List of options that can be used with any field that requires an RF input.

	@{
*/
#define ANY_INPUT_NODE              PKEY_CONST('I','N','P','T') ///< \type{INT}\rw RF input on node
#define ANY_INPUT_SWITCH            PKEY_CONST('S','I','N','P') ///< \type{INT}\rw RF input on switch
#define ANY_INPUT_BDC               PKEY_CONST('B','I','N','P') ///< \type{INT}\rw RF input on BDC
#define ANY_BDC_EXPANSION           PKEY_CONST('B','D','E','X') ///< \type{INT}\rw Expansion port where BDC is connected
#define ANY_SWITCH_EXPANSION		PKEY_CONST('S','W','E','X') ///< \type{INT}\rw Expansion port where switch is connected
#define ANY_ANTENNA_UID             PKEY_CONST('A','U','I','D') ///< \type{INT}\rw Antenna UID
/** @} */

/**
	@defgroup ANY_GROUP "Any" options

	\brief "Any" options can be returned in any packet to the client

	@{
 */
#define ANY_ACKNOWLEDGE_PACKET	PKEY_CONST('A','C','K','N') ///< \type{INT}\ro Acknowlegement packet
#define ANY_ERROR               PKEY_CONST('E','R','R','O') ///< \type{STRING}\ro Error information
#define ANY_ERROR_CODE          PKEY_CONST('E','R','R','C') ///< \type{INT}\ro Error code. See \ref ERROR_GROUP
#define ANY_WARNING             PKEY_CONST('W','A','R','N') ///< \type{STRING}\ro Warning information
#define ANY_WARNING_CODE        PKEY_CONST('W','A','R','C') ///< \type{INT}\ro Warning code. See \ref WARNING_GROUP
#define ANY_REPEAT_NUMBER       PKEY_CONST('R','N','U','M') ///< \type{INT}\ro Number of times the command has executed
#define ANY_DATA_SEGMENT_NUMBER PKEY_CONST('S','E','G','N') ///< \type{INT}\ro Segment number of current segment. See \ref ANY_NUM_DATA_SEGMENTS
/**
	\brief \type{INT}\ro Number of seperate segments

	If a user request is to large, because of HW or memory constraints, it is split into seperate segments. Each
	segment is returned in a seperate packet. This parameter tells you the total number of packets/segments to expect.
*/
#define ANY_NUM_DATA_SEGMENTS   PKEY_CONST('N','S','E','G')

/// \cond No_Documentation
#define ANY_LICENSE_ERROR       PKEY_CONST('L','I','C','E')/*[int32,RO]         license not valid for this function */
/// \endcond
/** @} */


/**
	@defgroup ANY_DSP_GROUP "Any" DSP options

	\brief "Any" DSP options can be included\\returned in any packet of type DSPC or DSPL
	
	@{
*/
/// \cond No_Documentation
#define ANY_DSP_DEBUG_GET_RDB           PKEY_CONST('g','R','D','B')/*[bool,WO]          return CDB as string */
#define ANY_DSP_DEBUG_GET_CDB           PKEY_CONST('g','C','D','B')/*[bool,WO]          return RDB as string */
#define ANY_DSP_DEBUG_GET_BASIC         PKEY_CONST('g','B','A','S')/*[bool,WO]          return basic debug info */
#define ANY_DSP_DEBUG_RDB_STRING        PKEY_CONST('d','R','D','B')/*[string,RO]        return CDB as string */
#define ANY_DSP_DEBUG_CDB_STRING        PKEY_CONST('d','C','D','B')/*[string,RO]        return RDB as string */
#define ANY_DSP_DEBUG_BASIC_STRING		PKEY_CONST('d','B','A','S')/*[string,RO]        return basic debug info */
#define ANY_DSP_RDB_ERROR_MSG           PKEY_CONST('D','R','E','R')/*[string,RO]        return error message */
#define ANY_DSP_RDB_ERROR_CODE          PKEY_CONST('D','R','E','C')/*[int32,RO]         return error code */
/// \endcond
#define ANY_DSP_TIMEOUT                 PKEY_CONST('T','O','U','T') ///< \type{INT}\wo Override inbuilt command timeouts (seconds)
#define ANY_DSP_TIME_US                 PKEY_CONST('D','S','P','T') ///< \type{INT}\ro Time taken be radio to execute command (usec)
#define ANY_DSP_WAIT_US                 PKEY_CONST('D','S','P','W') ///< \type{INT}\ro Idle time waiting for radio to finish a command (usec)
#define ANY_DSP_RTC_UNIX_TIME           PKEY_CONST('U','T','I','M') ///< \type{INT}\ro Time in time_t command was executed 
#define ANY_DSP_RTC_NANO                PKEY_CONST('N','A','N','O') ///< \type{INT}\ro Offset in nanoseconds when command was executed
#define ANY_DSP_RTC_UTIM_STR            PKEY_CONST('U','S','T','R') ///< \type{INT}\ro Time as string when command was executed
#define ANY_DSP_COM_MESSAGE             PKEY_CONST('C','M','S','G') ///< \type{STRING}\ro Radio's returned message
#define ANY_DSP_RADIO_TICK              PKEY_CONST('A','D','R','T') ///< \type{INT}\ro Radio tick count at last roll over 
/**
	\brief \type{INT}\rw User specified data tag

	A user specified tag which if sent to NCP in a field will \b always be returned in the response
*/
#define ANY_DSP_DATA_TAG                PKEY_CONST('D','T','A','G')
/**
	\brief \type{INT}\rw Run the command no quicker than this

	This parameter allows a command to limit how fast it will execute. The rate is specified in ms and the command will
	have atleast x ms between executions. Depending on outstanding commands it may run slower than this.
*/
#define ANY_DSP_REPEAT_RATE             PKEY_CONST('R','R','A','T')
/** @} */

#endif
