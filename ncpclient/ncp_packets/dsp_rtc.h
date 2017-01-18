/*-----------------------------------------------------------------------------
 * (c) CRFS Limited, 2011. All rights reserved.
 *
 * This software is the property of CRFS Limited and may not be copied or
 * reproduced otherwise than on to a single hard disk for backup or
 * archival purposes. The source code is confidential information and must
 * not be disclosed to third parties or used without the express written
 * permission of CRFS Limited.
 *
 * Filename : dsp_rtc.h
 *---------------------------------------------------------------------------*/
#ifndef __DSP_RTC_H
#define __DSP_RTC_H

/** \file  dsp_rtc.h

	Field and options to configure the CRFS synclinc<SUP>TM</SUP> system
*/

/** 
	\ingroup FIELD_GROUP
	\brief Configure RTC external synchronisation

	This command requires the CRFS synclinc<SUP>TM</SUP> system

	See \ref DSP_RTC_SYNC_GROUP 
	\frw
*/
#define FIELD_DSP_RTC_SYNC                      PKEY_CONST('S','Y','N','C')

/**
	@defgroup DSP_RTC_SYNC_GROUP  DSP RTC Sync Options

	In order to perform synchronous sweeps with other RFeye nodes, it is necessary to synchronise the real-time clocks 
	of all nodes in the synchronous group. This is achieved by using the RFeye SyncLinc system to transmit the internal 
	timecode from a designated ‘master’ node to all other ‘slave’ nodes. This field provides the options to configure 
	the RFeye SyncLinc system to achieve higher timing accuracy. The detailed descriptions of each option are as follows.

	@{
*/
#define DSP_RTC_SYNC_DELAY_COMP_NS              PKEY_CONST('S','D','L','Y') ///< \type{INT}\rw
#define DSP_RTC_SYNC_DELAY_CYC10M_NS            PKEY_CONST('S','C','Y','C') ///< \type{INT}\ro
#define DSP_RTC_SYNC_DELAY_TAP                  PKEY_CONST('S','T','A','P') ///< \type{INT}\ro
#define DSP_RTC_CONFIG_SLAVE                    PKEY_CONST('S','C','F','G') ///< \type{INT}\rw
#define DSP_RTC_CONFIG_SLAVE_MEASURE_DELAY      PKEY_CONST('S','C','M','D') ///< \type{INT}\rw
#define DSP_RTC_CONFIG_MASTER                   PKEY_CONST('M','C','F','G') ///< \type{INT}\rw
#define DSP_RTC_CONFIG_MASTER_MEASURE_DELAY		PKEY_CONST('M','C','M','D') ///< \type{INT}\rw
#define DSP_RTC_MASTER_MEASURE_DELAY            PKEY_CONST('D','M','E','S') ///< \type{INT}\rw
#define DSP_RTC_MASTER_PATH_DELAY_NS            PKEY_CONST('M','P','D','Y') ///< \type{INT}\rw
#define DSP_RTC_MASTER_RX_OFFSET_NS             PKEY_CONST('M','O','F','F') ///< \type{INT}\rw
/** @} */

#endif
