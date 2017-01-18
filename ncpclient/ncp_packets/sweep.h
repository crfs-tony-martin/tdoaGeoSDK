/*-----------------------------------------------------------------------------
 * (c) CRFS Limited, 2011. All rights reserved.
 *
 * This software is the property of CRFS Limited and may not be copied or
 * reproduced otherwise than on to a single hard disk for backup or
 * archival purposes. The source code is confidential information and must
 * not be disclosed to third parties or used without the express written
 * permission of CRFS Limited.
 *
 * Filename : sweep.h
 *---------------------------------------------------------------------------*/
#ifndef __SWEEP_H
#define __SWEEP_H
/** \file sweep.h

	Request/retrieve spectral data from the node.
*/

/** 
	\ingroup FIELD_GROUP
	\brief Request/retrieve data for a frequency sweep

	See \ref SWEEP_GROUP 
	\frw
*/
#define FIELD_SWEEP                             PKEY_CONST('S','W','E','P')

/**
	@defgroup SWEEP_GROUP  Sweep options

	The RFeye Node captures frequency domain sweep data by outputting the frequency bin values resulting from FFT 
	analysis on captures in tuned steps (sweeps). This field provides the options to set and retrieve sweep related 
	options required for a sweep task.

	Building a simple sweep request:

	\code{.c}
		// Request a sweep from the node
		packet_write(packet, PACKET_TYPE_DSPC, -1);
		// Add the sweep field
		packet_add_field(packet, FIELD_SWEEP, -1)
			// Start at 450Mhz
			packet_add_param_int(packet, SWEEP_START_FREQ_MHZ, 450);
			// End at 550Mhz
			packet_add_param_int(packet, SWEEP_STOP_FREQ_MHZ, 550);
			// Request the peak data => This is the default
			packet_add_param_int(packet, SWEEP_GET_PEAK_DATA, 1);
		// Finish packet
		packet_write_complete();
	\endcode

	@{
*/
#define SWEEP_START_FREQ_MHZ        PKEY_CONST('F','S','T','A') ///< \type{INT}\rw Start frequency in MHz
#define SWEEP_STOP_FREQ_MHZ         PKEY_CONST('F','S','T','P') ///< \type{INT}\rw Stop frequency in MHz
#define SWEEP_START_FREQ_MILLIHZ    PKEY_CONST('F','S','A','M') ///< \type{INT}\rw Fractional start frequency in MHz
#define SWEEP_STOP_FREQ_MILLIHZ     PKEY_CONST('F','S','P','M') ///< \type{INT}\rw Fractional stop frequency in MHz
#define SWEEP_CHANNEL               PKEY_CONST('C','H','A','N') ///< \type{INT}\rw Channel number to start scanning. See \ref Channels
#define SWEEP_CHANNEL_END           PKEY_CONST('C','H','A','E') ///< \type{INT}\rw Channel number to stop scanning. See \ref Channels
/**
	\brief \type{INT}\rw\auto Select the low noise Synth

	There are two quality methods \ref SWEEP_QUALITY_LOW_NOISE or \ref SWEEP_QUALITY_FAST_TUNE. The first is slower with
	better phase noise. The second is faster with with worse phase noise.

	If neither synth is selected then NCPd will select the most approite.

	Only one of \ref SWEEP_QUALITY_LOW_NOISE or \ref SWEEP_QUALITY_FAST_TUNE can be specified.
*/
#define SWEEP_QUALITY_LOW_NOISE     PKEY_CONST('L','N','O','I')
/**
	\brief \type{INT}\rw\auto Select the low noise Synth

	There are two quality methods \ref SWEEP_QUALITY_LOW_NOISE or \ref SWEEP_QUALITY_FAST_TUNE. The first is slower with
	better phase noise. The second is faster with with worse phase noise.

	The fast synth can not be used in certian situations, if this is the case then the slow synth will automatically 
	be selected. 

	If neither synth is selected then NCPd will select the most approite.

	Only one of \ref SWEEP_QUALITY_LOW_NOISE or \ref SWEEP_QUALITY_FAST_TUNE can be specified.
*/
#define SWEEP_QUALITY_FAST_TUNE     PKEY_CONST('F','T','U','N')
#define SWEEP_QUALITY_DUPLEX        PKEY_CONST('Q','D','U','P') ///< \type{INT}\rw Experimental
/**
	\brief \type{INT}\rw\auto Select high speed capture

	\deprecated NCPd automatically selects the correct speed of capture. If the high speed capture method can be used
				for the given command then it will.

	In certian sweep commands it is possible to use a faster capture method with no effect on quality. NCPd will 
	automatically override this	parameter if high speed capture can not be used. If neither speed commands are specified
	then NCPd will select appriotely.

	Only one of \ref SWEEP_HIGH_SPEED or \ref SWEEP_NORMAL_SPEED can be specified.
*/
#define SWEEP_HIGH_SPEED            PKEY_CONST('H','S','P','E')
/**
	\brief \type{INT}\rw\auto Select slow speed capture

	\deprecated NCPd automatically selects the correct speed of capture. If the high speed capture method can be used
				for the given command then it will select it.

	In certian sweep commands it is possible to use a faster capture method with no effect on quality. NCPd will 
	automatically override this	parameter if high speed capture can not be used. If neither speed commands are specified
	then NCPd will select appriotely.

	Only one of \ref SWEEP_HIGH_SPEED or \ref SWEEP_NORMAL_SPEED can be specified.
*/
#define SWEEP_NORMAL_SPEED          PKEY_CONST('N','S','P','E')

  /* if SWEEP_RES_BANDWIDTH_HZ is not specified then the unit defaults to auto and picks the most appropriate resolution for the span */

/**
	\brief \type{INT}\rw\auto Select the resolution bandwidth

	Specify the resolution bandwidth of the sweep. If a resolution bandwidth is not be supported a lower bandwith will
	automatically be selected. If this parameter is not specified NCPd will select an approite bandwidth.
*/
#define SWEEP_RES_BANDWIDTH_HZ      PKEY_CONST('R','E','S','B')
#define SWEEP_FFT_SIZE              PKEY_CONST('S','F','F','T') ///< \type{INT}\ro FFT size used \deprecated To be removed
/**
	\brief \type{INT}\rw Number of loops

	This parameter sets the number of loops. The radio can select the peak or calculate the average of the captures.
	Loops can be set to 1, 2, 4, 8, 16 or 32
*/
#define SWEEP_NUM_LOOPS             PKEY_CONST('N','U','M','L')
#define SWEEP_INPUT                 PKEY_CONST('I','N','P','T') ///< \type{INT}\rw \deprecated See \ref ANY_INPUT_NODE
#define SWEEP_ANTENNA_UID           PKEY_CONST('A','U','I','D') ///< \type{INT}\rw \deprecated See \ref ANY_ANTENNA_UID
/**
	\brief \type{INT}\rw\auto Reference level

	Reference level in dBm the data's dynamic range extends 127.5dB below this level
*/
#define SWEEP_REF_LEVEL             PKEY_CONST('R','L','E','V')
/*[int32,RW, -1]     If this is set it will repeatedly sweep for the time specified in ms */

/**
	\brief \type{INT}\rw Dwell time

	Dwell time instructs NCPd to perform the sweep for _atleast_ the specified ms. It will calculate the average and the
	peak of the data off all captures performed during this time.
*/
#define SWEEP_DWELL_TIME            PKEY_CONST('D','W','E','L')
/**
	\brief \type{INT}\rw\auto Manual Attenuation

	Specify the level of attenuation to use during the capture. If not specified the capture will use Automatic Gain 
	Control.
*/
#define SWEEP_MANUAL_ATTEN          PKEY_CONST('A','T','E','N')
#define SWEEP_AGC                   PKEY_CONST('A','G','C',' ') ///< \type{INT}\ro AGC was used
#define SWEEP_AGC_TABLE_MODE        PKEY_CONST('A','G','C','T') ///< \type{INT}\rw Use AGC table mode
#define SWEEP_COMMAND 				PKEY_CONST('C','M','D',' ') ///< \type{STRING}\ro "HS" = High speed sweep, "NORM" = Normal speed sweep
/**
	\brief \type{INT}\rw\auto Decimation level

	\deprecated NCPd automatically selects the correct decimation level based on the resolution bandwidth

	Specify the hardware decimation factor to use. 
*/
#define SWEEP_DECIMATION            PKEY_CONST('D','E','C','I')
#define SWEEP_GET_PEAK_DATA         PKEY_CONST('G','E','T','P') ///< \type{INT}\rw Return the peak data
#define SWEEP_GET_AVERAGE_DATA      PKEY_CONST('G','E','T','A') ///< \type{INT}\rw Return the average data
#define SWEEP_GET_TIME_DATA         PKEY_CONST('G','E','T','T') ///< \type{INT}\rw Return the time data. One per capture
#define SWEEP_GET_STATUS_DATA       PKEY_CONST('G','E','T','S') ///< \type{INT}\rw Return the status records. One per capture

/**
	\brief \type{INT}\rw Return values as dBuV/m

	Return the data as dBuV/m as opposed to the default of dB. This is only possible if \ref K-Factor files have been
	provided, and configured, for the antenna selected to perform the sweep.

*/
#define SWEEP_GET_DBUVM             PKEY_CONST('D','B','U','V')
#define SWEEP_PEAK_DATA             PKEY_CONST('P','D','A','T') ///< \type{SPEC8}\ro Returned peak data. See \ref SWEEP_GET_PEAK_DATA
#define SWEEP_AVERAGE_DATA          PKEY_CONST('A','D','A','T') ///< \type{SPEC8}\ro Returned average data. See \ref SWEEP_GET_AVERAGE_DATA
#define SWEEP_TIME_I_Q_DATA         PKEY_CONST('T','D','A','T') ///< \type{IQTIME}\ro Returned time data.  See \ref SWEEP_GET_TIME_DATA
/**
	\ro Returned status records

	The status records contain information about each individual capture that builds up the sweep data. The format is
	as follows:

	\code{.c}
		#pragma pack(push, 1)
		typedef struct _StatusRecords
		{
		    int32_t     Frequency; // Frequency of the the capture
		    int32_t     SpectrumResolution; // Resolution of the capture. All identical.
		    int32_t     NumberOfLoops; // Number of loops. All identical.
			int32_t     AGCGain; // AGC gain in dB 
			int32_t     RadioGain; // Radio gain in 1/16th dB
			int32_t     ErrorCode; // Error code for the individual capture 
			int32_t     Deprecated;
			int32_t     Flags;  // Clip/Overflow flags.
		} TStatusRecords;
		#pragma pack(pop)
	\endcode

	The flags are as follows:

	Bit  		  	| Flag
	:--------------:|-------------:
	31 - 26		   	| Reserved
	25 - 23		   	| HLLS mode
	22 - 20		   	| Reserved
	19				| Peak dBm conversion overflow clip
	18				| Peak dBm conversion underflow clip
	17				| Mean dBm conversion overflow clip
	16				| Mean dBm conversion overflow clip
	15 - 1			| Reserved
	0				| Radio ADC Overflow
*/

#define SWEEP_STATUS_DATA           PKEY_CONST('S','D','A','T')

/**
	@name Sweep trigger options

	A sweep can be triggered by an external event.

	\note Only one of \ref SWEEP_TRIG_MODE_IMMEDIATE, \ref SWEEP_TRIG_MODE_ABS_TIME, \ref SWEEP_TRIG_MODE_EXP_1, 
		  \ref SWEEP_TRIG_MODE_EXP_2 or \ref SWEEP_TRIG_MODE_GPS can be specified at one time. 
	\note \ref SWEEP_SYNC_MODE must be set triggering to work
*/
///@{
/**
	\brief \type{INT}\rw Perform sweep immediately
	\deprecated This is the default unless one of the other four trigger modes is used.

*/
#define SWEEP_TRIG_MODE_IMMEDIATE   PKEY_CONST('T','I','M','M')/*[bool,RW,true]     sweep trigger immediate */
/**
	\brief \type{INT}\rw Perform sweep at an absolute time

	The sweep occurs at the given time. See \ref SWEEP_TRIG_TIME_UNIX and \ref SWEEP_TRIG_TIME_NANO.
*/
#define SWEEP_TRIG_MODE_ABS_TIME    PKEY_CONST('T','A','B','S')
#define SWEEP_TRIG_TIME_UNIX        PKEY_CONST('T','U','N','X') ///< \type{INT}\rw Sweep trigger time in unix time_t format
#define SWEEP_TRIG_TIME_NANO        PKEY_CONST('T','N','A','N') ///< \type{INT}\rw Sweep trigger time in ns. Added to \ref SWEEP_TRIG_TIME_UNIX

/**
	\brief \type{INT}\rw Perform sweep from input on expansion 1

	\note CRFS Technical note 19 "RFeye Expansion Port" describes required cabling
*/
#define SWEEP_TRIG_MODE_EXP_1       PKEY_CONST('T','E','X','1')
/**
	\brief \type{INT}\rw Perform sweep from input on expansion 2

	\note CRFS Technical note 19 "RFeye Expansion Port" describes required cabling
*/
#define SWEEP_TRIG_MODE_EXP_2       PKEY_CONST('T','E','X','2')
/**
	\brief \type{INT}\rw Perform sweep at the 1PPS of the GPS

	The captured should be triggered by the 1 pulse per second provided by the GPS. All nodes 1 PPS are synchronized.
*/
#define SWEEP_TRIG_MODE_GPS         PKEY_CONST('T','G','P','S')
///@}

/**
	\brief \type{INT}\rw Perform a synchronous sweep

	By default the if sync mode is enabled it will perform a sweep using the SyncLinc<SUP>TM</SUP> system. This can be
	overridden using one of \ref SWEEP_TRIG_MODE_IMMEDIATE, \ref SWEEP_TRIG_MODE_ABS_TIME, \ref SWEEP_TRIG_MODE_EXP_1, 
    \ref SWEEP_TRIG_MODE_EXP_2 or \ref SWEEP_TRIG_MODE_GPS. 
*/
#define SWEEP_SYNC_MODE                         PKEY_CONST('S','Y','N','C')
/**
	\brief \type{INT}\rw Tune interval

	The delay to use between tunes, 25ns resolution. This value \b must be set to the same value for all units carrying
	out the synchronous sweep.

	\note Only applies if using the SyncLinc<SUP>TM</SUP> system.

*/
#define SWEEP_SYNC_TUNE_INTERVAL                PKEY_CONST('S','T','D','L')
/**
	\brief \type{INT}\rw Max re-arm time

	Peak tune time for sweep in us.  By comparing this return value from all units running a synchronous sweep, it is 
	possible to calculate the optimum value of SWEEP_SYNC_TUNE_INTERVAL to give the fastest possible syncronous sweep.

	\note Only applies if using the SyncLinc<SUP>TM</SUP> system.

*/
#define SWEEP_SYNC_MAX_RE_ARM_TIME_NS           PKEY_CONST('R','A','R','M')
/**
	\brief \type{INT}\rw The number of sweeps to perform
	
	If specified then the number of sweeps to perform synchronously back-to-back.

	
	\note Only applies if using the SyncLinc<SUP>TM</SUP> system.

*/
#define SWEEP_SYNC_NUM_SWEEPS                   PKEY_CONST('N','S','W','P')
/**
	\brief \type{INT}\rw Increase in the delay between syncs
	
	If you are having timing issues whilst performing synchrounous sweeps then you can increase the delays between tunes.
	The value represents 0.1% (i.e. 10 = 1%) increase.

	\note Only applies if using the SyncLinc<SUP>TM</SUP> system.

*/
#define SWEEP_SYNC_DELAY_TUNE                   PKEY_CONST('S','S','D','T')

/// \cond No_Documentation
#define SWEEP_TRIG_TIME_STR         PKEY_CONST('T','S','T','R')/*[int32,RW]         sweep trigger time string */
#define SWEEP_REPEAT_DELAY_UNIX     PKEY_CONST('R','U','N','X')/*[int32,RW]         sweep trigger repeat time seconds */
#define SWEEP_REPEAT_DELAY_NANO		PKEY_CONST('R','N','A','N')/*[int32,RW]         sweep trigger repeat time nano seconds */
#define SWEEP_REPEAT_MISSED_TRIGGERS        PKEY_CONST('T','M','I','S')/*[int32,RW]         sweep trigger repeat missed trigger count */
#define SWEEP_REPEAT_TRIGGER_MARGIN_UNIX	PKEY_CONST('M','U','N','X')/*[int32,RW]         sweep trigger repeat margin unix */
#define SWEEP_REPEAT_TRIGGER_MARGIN_NANO    PKEY_CONST('M','N','A','N')/*[int32,RW]         sweep trigger repeat margin nano */
#define SWEEP_ASYNC_MODE                        PKEY_CONST('A','S','Y','1')/*[int,RW,0]         specifies to use Asymmetric Multi-Sync Sweep.
                                                                                                Value indicates number of blocks */
#define SWEEP_ASYNC_TABLE                       PKEY_CONST('A','S','T','A')/* */
#define SWEEP_ASYNC_EXP_PORT                    PKEY_CONST('A','S','E','X')/*[int32, RW]        which expansion port to use 1 or 2*/
/// \endcond
/** @} */


#endif
