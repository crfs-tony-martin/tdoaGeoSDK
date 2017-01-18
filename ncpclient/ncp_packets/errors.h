/*-----------------------------------------------------------------------------
 * (c) CRFS Limited, 2011. All rights reserved.
 *
 * This software is the property of CRFS Limited and may not be copied or
 * reproduced otherwise than on to a single hard disk for backup or
 * archival purposes. The source code is confidential information and must
 * not be disclosed to third parties or used without the express written
 * permission of CRFS Limited.
 *
 * Filename : errors.h
 *---------------------------------------------------------------------------*/
#ifndef __ERRORS_H
#define __ERRORS_H

/** \file errors.h

	Field and options returned by NCPd in error situations
*/

/** 
	\ingroup FIELD_GROUP
	\brief Reports error status

	See \ref ERROR_GROUP 
	\fro
*/
#define FIELD_ERROR                             PKEY_CONST('E','R','R','O') 


/**
	@defgroup ERROR_GROUP Error Codes

	\brief 	List of errors that can be returned in response to a NCPd request.

	Errors are returned in the field that caused the error to occur. If the
	error is returned in the ACK packet then there will be no data.

	If an error is not associated with a command request it will be returned in 
	an extra \ref FIELD_ERROR field.

	@{
 */
#define ERROR_UNKNOWN                       0xf000 ///< An internal error has occured
#define ERROR_TIMEOUT                       0xf002 ///< A command sent to the radio has timed out
#define ERROR_NCP_INIT                      0xf003 ///< NCP Initialization failed. 
#define ERROR_UNRECOGNISED_PACKET           0xf004 ///< Packet type not recongnized
#define ERROR_MULTI_SYNC                    0xf005 ///< Multi sync sweeps failed. Timing is to tight
#define ERROR_OUT_OF_MEMORY                 0xf006 ///< Out of memory
#define ERROR_NO_LOCK                       0xf007 ///< Unused
#define ERROR_NO_ANTENNA                    0xf008 ///< No antenna to perform specified request
#define ERROR_NO_LIGHTS                     0xf009 ///< No lights configure for use in lights command
#define ERROR_NO_CHANNEL                    0xf010 ///< No channel specified
#define ERROR_NO_DEVICE                     0xf011 ///< Unused
#define ERROR_RADIO                         0xf012 ///< Radio returned error code
/** @} */


#endif
