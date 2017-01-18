/*-----------------------------------------------------------------------------
 * (c) CRFS Limited, 2011. All rights reserved.
 *
 * This software is the property of CRFS Limited and may not be copied or
 * reproduced otherwise than on to a single hard disk for backup or
 * archival purposes. The source code is confidential information and must
 * not be disclosed to third parties or used without the express written
 * permission of CRFS Limited.
 *
 * Filename : warnings.h
 *---------------------------------------------------------------------------*/
#ifndef __WARNINGS_H
#define __WARNINGS_H

/** \file warnings.h

	Field and options returned by NCPd for warnings
*/

/** 
	\ingroup FIELD_GROUP	
	\brief Reports warnings

	See \ref WARNING_GROUP 
	\fro
*/
#define FIELD_WARN                              PKEY_CONST('W','A','R','N')

/**
	@defgroup WARNING_GROUP Warning Codes

	\brief 	List of all warnings that can be returned in response to a NCPd request.

	@{
 */
#define WARNING_UNKNOWN                     0x0f00 ///< An internal error has occured
#define WARNING_UNRECOGNISED_FIELD          0x0f01 ///< Unrecognised field in packet
#define WARNING_UNRECOGNISED_PARAMETER      0x0f02 ///< Unrecognised parameter in packet
#define WARNING_CALIBRATION_NOT_SUPPORTED   0x0f03 ///< Calibration requested but not supported
#define WARNING_AGC_OVERFLOW                0x0f04 ///< AGC overflow. Data may be effected
#define WARNING_UNCALIBRATED_SWEEP          0x0f05 ///< Unused
#define WARNING_BDC_NOT_FOUND               0x0f06 ///< Command requested use of BDC and no BDC found
#define WARNING_ADC_OVERFLOW                0x0f07 ///< ADC overflow. Data may be effected
#define WARNING_DBM_MEAN_UNDERFLOW          0x0f08 ///< Unused
#define WARNING_DBM_MEAN_OVERFLOW           0x0f09 ///< Conversion of mean data to dBm overflowed
#define WARNING_DBM_PEAK_UNDERFLOW          0x0f0A ///< Unused
#define WARNING_DBM_PEAK_OVERFLOW           0x0f0B ///< Conversion of peak data to dBm overflowed
#define WARNING_SYNTH_FAILED                0x0f0C ///< The synth(High Speed, High quality) requested has failed. Other synth is used for command
#define WARNING_NO_GPS_USING_INTERNAL       0x0f0D ///< GPS has been requested for timing and there is no fix. Internal clock used for timing
#define WARNING_LOCKED                      0x0f0E ///< Another client is using the client exclusivily
#define WARNING_INVALID_CONFIGURATION_FILE  0x0f0F ///< NCPd configuration file is invalid. This is _ONLY_ returned once on connection
#define WARNING_INVALID_UID                 0x0f10 ///< No antenna exists with given UID
#define WARNING_DEFAULT_CONFIGURATION       0x0f11 ///< Default configuration in use. This is _ONLY_ returned once on connection
#define WARNING_ANTENNA_OUTSIDE_RANGE       0x0f12 ///< Antenna selected is being used outside of its specified range
#define WARNING_MODEM_TRANSMISSION          0x0f13 ///< Internal modem was transmitting during data capture. Data may be effected
/** @} */


#endif
