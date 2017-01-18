/*-----------------------------------------------------------------------------
 * (c) CRFS Limited, 2011. All rights reserved.
 *
 * This software is the property of CRFS Limited and may not be copied or
 * reproduced otherwise than on to a single hard disk for backup or
 * archival purposes. The source code is confidential information and must
 * not be disclosed to third parties or used without the express written
 * permission of CRFS Limited.
 *
 * Filename : ncptime.h
 *---------------------------------------------------------------------------*/
#ifndef __NCPTIME_H
#define __NCPTIME_H

/** \file  time.h

	Request/retrieve IQ time data from the node
*/

/** 
	\ingroup FIELD_GROUP
	\brief Request/retrieve data for a time capture

	See \ref TIME_GROUP 
	\frw
*/
#define FIELD_TIME                              PKEY_CONST('T','I','M','E')

/**
	@defgroup TIME_GROUP  Time options

	The RFeye Node radio can ‘stare’ at a band to perform a time domain spectral capture. This field provides the 
	options to set and retrieve in-phase / quadrature (I/Q) related parameters required for an I/Q time capture task.

	\code{.c}
		// Request a IQ data from the node
		packet_write(packet, PACKET_TYPE_DSPC, -1);
		// Add the time field
		packet_add_field(packet, FIELD_TIME, -1)
			// Centre at 450MHz
			packet_add_param_int(packet, SWEEP_START_FREQ_MHZ, 450);
			// A bandwidth of 10MHz (5Mhz either side of centre). The node captures 40MHz bandwidth. 
			// Decimating by 4 brings this down to 10MHz still centred on the centre frequency
			packet_add_param_int(packet, TIME_DECIMATION, 4);
		// Finish packet
		packet_write_complete();
	\endcode

	@{
*/
#define TIME_CENTER_FREQ_MHZ		PKEY_CONST('F','C','T','R') ///< \type{INT}\rw Center frequency (MHz)
#define TIME_DDS_OFFSET_HZ          PKEY_CONST('D','D','S','O') ///< \type{INT}\rw DDS offset frequency (Hz)
/**
	\brief \type{INT}\rw Scale the DDS

	Scale the DDS. Values 0 - 7 accepted.

	\note This if for use of you are seeing DDS overflow errors 
*/
#define TIME_DDS_SCALE              PKEY_CONST('D','D','S','S')
/**
	\brief \type{INT}\rw Number of IQ samples to capture

	The maximum continoes number of samples that can be captured is 12000000 samples. If the requested number of samples
	is larger than this than two packets will be returned but the data is not continuos.
*/
#define TIME_NUM_SAMPLES            PKEY_CONST('S','A','M','P')
/**
	\brief \type{INT}\rw\auto Manual Attenuation

	Specify the level of attenuation to use during the capture. If not specified the capture will use Automatic Gain 
	Control.
*/
#define TIME_MANUAL_ATTEN           PKEY_CONST('A','T','E','N')
#define TIME_AGC                    PKEY_CONST('A','G','C',' ') ///< \type{INT}\ro AGC was used
#define TIME_AGC_TABLE_MODE         PKEY_CONST('A','G','C','T') ///< \type{INT}\rw Use AGC table mode
#define TIME_INPUT                  PKEY_CONST('I','N','P','T') ///< \type{INT}\rw \deprecated See \ref ANY_INPUT_NODE
#define TIME_ANTENNA_UID            PKEY_CONST('A','U','I','D') ///< \type{INT}\rw \deprecated See \ref ANY_ANTENNA_UID
#define TIME_DECIMATION             PKEY_CONST('D','E','C','I') ///< \type{INT}\rw Hardware decimation. Accepted values 0,2,4-8191
/**
	@name Quality options
	
	There are two quality methods \ref TIME_QUALITY_LOW_NOISE or \ref TIME_QUALITY_FAST_TUNE. The first is slower with
	better phase noise. The second is faster with worse phase noise.

	If neither synth is selected then NCPd will select low noise.

	\note Only one of \ref TIME_QUALITY_LOW_NOISE or \ref TIME_QUALITY_FAST_TUNE can be specified.
*/
///@{
#define TIME_QUALITY_LOW_NOISE      PKEY_CONST('L','N','O','I')
#define TIME_QUALITY_FAST_TUNE      PKEY_CONST('F','T','U','N')
/// \cond No_Documentation
#define TIME_SIMPLE_DC_NUM_CAP      PKEY_CONST('T','S','D','C')///< \type{INT}\rw DC offset compensation
/// \endcond

/**
	@name Time trigger options

	A time capture can be triggered by an external event

	\note Only one of \ref TIME_TRIG_MODE_IMMEDIATE, \ref TIME_TRIG_MODE_ABS_TIME, \ref TIME_TRIG_MODE_EXP_1, 
		  \ref TIME_TRIG_MODE_EXP_2 or \ref TIME_TRIG_MODE_GPS can be specified at one time. 
*/
///@{
/**
	\brief \type{INT}\rw Perform capture immediately
	\deprecated This is the default unless one of the other trigger modes is used.

*/
#define TIME_TRIG_MODE_IMMEDIATE		PKEY_CONST('T','I','M','M')
/**
	\brief \type{INT}\rw Perform capture from trigger on expansion 1

	\note CRFS Technical note 19 "RFeye Expansion Port" describes required cabling
*/
#define TIME_TRIG_MODE_EXP_1			PKEY_CONST('T','E','X','1')
/**
	\brief \type{INT}\rw Perform capture from trigger on expansion 2

	\note CRFS Technical note 19 "RFeye Expansion Port" describes required cabling
*/
#define TIME_TRIG_MODE_EXP_2            PKEY_CONST('T','E','X','2')
/**
	\brief \type{INT}\rw Perform capture at an absolute time

	The capture occurs at the given time. See \ref TIME_TRIG_ABS_UNIX and \ref TIME_TRIG_ABS_NANO.
*/
#define TIME_TRIG_MODE_ABS_TIME         PKEY_CONST('T','A','B','S')
#define TIME_TRIG_ABS_UNIX              PKEY_CONST('T','U','N','X') ///< \type{INT}\rw Trigger time in unix time_t format
#define TIME_TRIG_ABS_NANO              PKEY_CONST('T','N','A','N') ///< \type{INT}\rw Trigger time in ns. Added to \ref TIME_TRIG_ABS_UNIX
/**
	\brief \type{INT}\rw Perform capture at the 1PPS of the GPS

	The capture should be triggered by the 1 pulse per second provided by the GPS. All nodes 1 PPS are synchronized.
*/
#define TIME_TRIG_MODE_GPS              PKEY_CONST('T','G','P','S')
#define TIME_TRIG_MODE_GSM_FRAME        PKEY_CONST('T','G','S','M') ///< \type{INT}\rw 
#define TIME_TRIG_MODE_POWER_THRESH     PKEY_CONST('T','P','T','H') ///< \type{INT}\rw 
#define TIME_TRIG_PT_THRESH             PKEY_CONST('T','P','P','T') ///< \type{INT}\rw
#define TIME_TRIG_PT_DWELL              PKEY_CONST('T','P','D','W') ///< \type{INT}\rw
#define TIME_TRIG_PT_PORT               PKEY_CONST('T','P','P','O') ///< \type{INT}\rw
/**
	\brief \type{INT}\rw Number of triggered captures to perform

	The number of captures to perform.
*/
#define TIME_TRIG_NUMBER_CAPTURES       PKEY_CONST('T','T','N','C')
/// \cond No_Documenation
#define TIME_TRIG_REPEAT_UNIX           PKEY_CONST('R','U','N','X') ///< trig repeat time unix seconds
#define TIME_TRIG_REPEAT_NANO           PKEY_CONST('R','N','A','N') ///< trig repeat time nanoseconds */
#define TIME_REPEAT_MISSED_TRIGGERS     PKEY_CONST('T','M','I','S') ///< time trigger repeat missed trigger count */
#define TIME_REPEAT_TRIGGER_MARGIN_UNIX PKEY_CONST('M','U','N','X') ///< time trigger repeat margin unix */
#define TIME_REPEAT_TRIGGER_MARGIN_NANO PKEY_CONST('M','N','A','N') ///< time trigger repeat margin nano */
/// \endcond
///@}

/**
	\brief \type{INT}\ro Radio gain
	
	Radio gain, used to scale IQ samples to absolute power. The value is in 1/16<SUP>th</SUP> of a dB


	See \ref label_faq_6 for using this to convert to absolute power.
*/
#define TIME_RADIO_GAIN                 PKEY_CONST('R','G','A','I')
#define TIME_AGC_ATTEN                  PKEY_CONST('R','A','T','N') ///< \type{INT}\ro RF attenuation
/**
	\brief \type{INT}\ro IQ Inversion

	Indicates a nominal IQ swap caused by frequency inversion in the radio
*/
#define TIME_FREQ_PLAN_INVERT           PKEY_CONST('F','I','N','V')
#define TIME_DC_OFFSET_I                PKEY_CONST('D','C','O','I') ///< \type{INT}\ro DC offset I
#define TIME_DC_OFFSET_Q                PKEY_CONST('D','C','O','Q') ///< \type{INT}\ro DC offset I
/**
	\brief \type{INT}\rw Request IQ data
	\deprecated This is the default
*/
#define TIME_GET_I_Q_DATA               PKEY_CONST('G','I','Q','T')
#define TIME_I_Q_DATA                   PKEY_CONST('D','I','Q','T') ///< \type{TimeIQ}\ro Returned IQ Data
#define TIME_NO_RETUNE                  PKEY_CONST('T','N','R','T') ///< \type{INT}\ro Do not retune radio. Faster captures if radio is already tuned.

/**
	@name Quantization options
	
	Time data can be quantized to reduce the bandwidth of returned data
*/
///@{
#define TIME_PACK_DATA                  PKEY_CONST('T','P','D','A') ///< \type{INT}\rw Quantize data to 1,2,4 or 8 bits 
#define TIME_PACK_SCALE                 PKEY_CONST('T','P','S','C') ///< \type{INT}\ro Scale used to pack data
#define TIME_PACK_MIN_I                 PKEY_CONST('M','I','N','I') ///< \type{INT}\ro Minimum value of I samples
#define TIME_PACK_MAX_I                 PKEY_CONST('M','A','X','I') ///< \type{INT}\ro Maximum value of I samples
#define TIME_PACK_MIN_Q                 PKEY_CONST('M','I','N','Q') ///< \type{INT}\ro Minimum value of Q samples
#define TIME_PACK_MAX_Q                 PKEY_CONST('M','A','X','Q') ///< \type{INT}\ro Maximum value of Q samples
///@}

/// \cond No_Documentation
#define TIME_TEST_SIGNAL                PKEY_CONST('T','S','I','G')/*[int,RO]           Test signal: 0 = off, 1 = ramp, 2 = sine wave */
#define TIME_TEST_MIN                   PKEY_CONST('T','M','I','N')/*[int,RO]           Minimum value of the test signal */
#define TIME_TEST_MAX                   PKEY_CONST('T','M','A','X')/*[int,RO]           Maximum value of the test signal */
#define TIME_STREAM_DATA                PKEY_CONST('T','S','T','R')/*[bool,RW,false]    Stream time data */
/// \endcond
/** @} */


#endif
