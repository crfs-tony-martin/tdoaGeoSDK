/*-----------------------------------------------------------------------------
 * (c) CRFS Limited, 2011. All rights reserved.
 *
 * This software is the property of CRFS Limited and may not be copied or
 * reproduced otherwise than on to a single hard disk for backup or
 * archival purposes. The source code is confidential information and must
 * not be disclosed to third parties or used without the express written
 * permission of CRFS Limited.
 *
 * Filename : versions.h
 *---------------------------------------------------------------------------*/
#ifndef __VERSIONS_H
#define __VERSIONS_H

/** \file  versions.h

	Request/retrieve version information
*/

/** 
	\ingroup FIELD_GROUP
	\brief Retrieve version information from the DSP subsystem

	See \ref VERSIONS_GROUP 
	\frw
*/
#define FIELD_VERSIONS                          PKEY_CONST('V','E','R','S')

/**
	@defgroup VERSIONS_GROUP Versions options

	This field provides the options to retrieve the static version information from the on-board radio DSP subsystem and 
	FPGA.

	All integer values are returned in 4-Digit Binary Coded Decimal. Use the following to unpack the values

	\code{.c}
	static int BCD4DTOI(int BCD)
	{
		int result = ((BCD >> 12) & 0xf) * 1000;
		result += ((BCD >> 8) & 0xf) * 100;
		result += ((BCD >> 4) & 0xf) * 10;
		result += ((BCD) & 0xf);

		return result;
	}	
	\endcode

	@{
*/
#define VERSIONS_RADIO_FPGA             PKEY_CONST('R','F','P','G') ///< \type{INT}\ro Radio FPGS version
#define VERSIONS_RADIO_MICROCODE        PKEY_CONST('R','M','I','C') ///< \type{INT}\ro Radio Microcode version
#define VERSIONS_DSP_FPGA               PKEY_CONST('B','F','P','G') ///< \type{INT}\ro DSP FPGA version
#define VERSIONS_DSP_MICROCODE          PKEY_CONST('B','M','I','C') ///< \type{INT}\ro DSP Microcode version
#define VERSIONS_CPLD                   PKEY_CONST('C','P','L','D') ///< \type{INT}\ro CPLD version
#define VERSIONS_RADIO_INIT_TABLE		PKEY_CONST('R','I','N','I') ///< \type{INT}\ro Radio init table version
#define VERSIONS_RADIO_TUNE_TABLE       PKEY_CONST('R','T','U','N') ///< \type{INT}\ro Radio tune table version
#define VERSIONS_RADIO_SERIAL           PKEY_CONST('R','S','E','R') ///< \type{STRING}\ro] Radio serial number
/** @} */

#endif
