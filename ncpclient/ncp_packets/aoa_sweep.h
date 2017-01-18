/*-----------------------------------------------------------------------------
 * (c) CRFS Limited, 2011. All rights reserved.
 *
 * This software is the property of CRFS Limited and may not be copied or
 * reproduced otherwise than on to a single hard disk for backup or
 * archival purposes. The source code is confidential information and must
 * not be disclosed to third parties or used without the express written
 * permission of CRFS Limited.
 *
 * Filename : aoa_sweep.h
 *---------------------------------------------------------------------------*/
#ifndef __AOA_SWEEP_H
#define __AOA_SWEEP_H

/** \file  aoa_sweep.h

	Fields and options for performing AOA sweeps
*/

/** 
	\ingroup FIELD_GROUP
	\brief Request/retrieve data for an AOA sweep

	This command requires CRFS AOA array equipment

	See \ref AOA_SWEEP_GROUP 
	\fro
*/
#define FIELD_AOA_SWEEP                         PKEY_CONST('A','O','A','S')


/**
	@defgroup AOA_SWEEP_GROUP  AOA sweep options

	Angle of Arrival (AOA) is a well-stablished technique for locating the source of a given signal. It is effective 
	with any RF transmission type as it responds directly to the received RF power. AOA direction finding provides a 
	bearing to received signals by rapidly switching between directional antenna elements in an antenna array. This 
	field provides a number of options for the AOA sweep function. This allows for configuring and retrieving AOA sweep 
	related parameters. The detailed description of each options are as follows.
	
	\code
	    // Request an AOA sweep returning vecor data
	    T_PACKET* txpacket = packet_create("", 8192);
    	packet_write(txpacket, PACKET_TYPE_DSP_CONTROL, -1);
        // Add the AOA sweep field
        packet_add_field(txpacket, FIELD_AOA_SWEEP, -1);
            // Start frequency 900 MHz
            packet_add_param_int(txpacket, AOA_SWEEP_START_FREQ_MHZ, 900);
			// Stop frequency 910 MHz
            packet_add_param_int(txpacket, AOA_SWEEP_STOP_FREQ_MHZ, 910);
            // To acheive better accuracy request the node to use 32 loops
            packet_add_param_int(txpacket, AOA_SWEEP_NUM_LOOPS, 32);
            // Request only vector data
            packet_add_param_int(txpacket, AOA_SWEEP_GET_VECTOR_DATA, 1);
	    packet_write_complete(txpacket);
	\endcode
	
	@{
*/
#define AOA_SWEEP_START_FREQ_MHZ        PKEY_CONST('F','S','T','A') ///< \type{INT}\rw Start frequency in MHz
#define AOA_SWEEP_STOP_FREQ_MHZ         PKEY_CONST('F','S','T','P') ///< \type{INT}\rw Stop frequency in MHz
#define AOA_SWEEP_START_FREQ_MILLIHZ    PKEY_CONST('F','S','A','M') ///< \type{INT}\rw Fractional start frequency in MHz
#define AOA_SWEEP_STOP_FREQ_MILLIHZ     PKEY_CONST('F','S','P','M') ///< \type{INT}\rw Fractional stop frequency in MHz
/**
	\brief \type{INT}\rw\auto Select the low noise Synth

	There are two quality methods \ref SWEEP_QUALITY_LOW_NOISE or \ref SWEEP_QUALITY_FAST_TUNE. The first is slower with
	better phase noise. The second is faster with with worse phase noise.

	If neither synth is selected then NCPd will select the most approite.

	Only one of \ref SWEEP_QUALITY_LOW_NOISE or \ref SWEEP_QUALITY_FAST_TUNE can be specified.
*/
#define AOA_SWEEP_QUALITY_LOW_NOISE     PKEY_CONST('L','N','O','I')
/**
	\brief \type{INT}\rw\auto Select the low noise Synth

	There are two quality methods \ref SWEEP_QUALITY_LOW_NOISE or \ref SWEEP_QUALITY_FAST_TUNE. The first is slower with
	better phase noise. The second is faster with with worse phase noise.

	The fast synth can not be used in certian situations, if this is the case then the slow synth will automatically 
	be selected. 

	If neither synth is selected then NCPd will select the most approite.

	Only one of \ref SWEEP_QUALITY_LOW_NOISE or \ref SWEEP_QUALITY_FAST_TUNE can be specified.
*/
#define AOA_SWEEP_QUALITY_FAST_TUNE     PKEY_CONST('F','T','U','N')
/**
	\brief \type{INT}\rw\auto Select the resolution bandwidth

	Specify the resolution bandwidth of the sweep. If a resolution bandwidth is not be supported a lower bandwith will
	automatically be selected. If this option is not specified NCPd will select an approite bandwidth.
*/
#define AOA_SWEEP_RES_BANDWIDTH_HZ      PKEY_CONST('R','E','S','B')
/**
	\brief \type{INT}\rw Number of loops

	This option sets the number of loops. The radio can select the peak or calculate the average of the captures.
	Loops can be set to 1, 2, 4, 8, 16 or 32
*/
#define AOA_SWEEP_NUM_LOOPS             PKEY_CONST('N','U','M','L')
#define AOA_SWEEP_INPUT                 PKEY_CONST('I','N','P','T') ///< \type{INT}\rw \deprecated See \ref ANY_INPUT_NODE
#define AOA_SWEEP_UID                   PKEY_CONST('S','U','I','D') ///< \type{INT}\rw \deprecated See \ref ANY_ANTENNA_UID
/**
	\brief \type{INT}\rw\auto Reference level

	Reference level in dBm the data's dynamic range extends 127.5dB below this level
*/
#define AOA_SWEEP_REF_LEVEL             PKEY_CONST('R','L','E','V')
/**
	\brief \type{INT}\rw Apply array calibration

	If the node has available array calibration then use it to improve the accuracy of the calculations.
*/
#define AOA_SWEEP_APPLY_CALIBRATION     PKEY_CONST('A','C','A','L')
/**
	\brief \type{INT}\rw Vector dB threshold

	Threshold level in DB for calculating the angle & confidence. The angle & confidence will only be calculated if 
	one of the antenna’s has a power equal to or over the threshold.
*/
#define AOA_SWEEP_DB_THRESHOLD          PKEY_CONST('D','T','H','E')
/**
	\brief \type{INT}\rw Vector confidence threshold

	Threshold level of the confidence (Number 0 to 255). The angle of the bin will not be calculated unless the 
	confidence has a value equal to or over this threshold
*/
#define AOA_SWEEP_CONF_THRESHOLD        PKEY_CONST('C','T','H','E')/*[int32, ro, 0]     confidence threshold to calculate vectors */
/**
	\brief \type{INT}\rw\auto Manual Attenuation

	Specify the level of attenuation to use during the capture. If not specified the capture will use Automatic Gain 
	Control.
*/
#define AOA_SWEEP_MANUAL_ATTEN			PKEY_CONST('A','T','E','N')
#define AOA_SWEEP_AGC                   PKEY_CONST('A','G','C',' ') ///< \type{INT}\ro AGC was used
#define AOA_SWEEP_AGC_TABLE_MODE        PKEY_CONST('A','G','C','T') ///< \type{INT}\rw Use AGC table mode

/// \cond No_Documentation
#define AOA_SWEEP_OFFSET                PKEY_CONST('O','F','S','E')/*[int32, ro, 0]     angular offset from north*/
#define AOA_SWEEP_ANTENNA_CONFIG        PKEY_CONST('A','C','O','N')/*[uint32,RW,0x00654321]   antenna configuration, each nibble indicates antenna number, starting from least significant */
#define AOA_SWEEP_NUM_ANTENNAS          PKEY_CONST('A','N','U','M')/*[int32,RW,6]       number of antennas in array */
#define AOA_SWEEP_EXPANSION_PORT        PKEY_CONST('E','X','P','O')/*[int32,RW,1]       expansion port used to drive switch (1,2) */
/// \endcond

#define AOA_SWEEP_GET_PEAK_DATA         PKEY_CONST('G','E','T','P') ///< \type{INT}\rw Return the peak data
#define AOA_SWEEP_GET_AVERAGE_DATA      PKEY_CONST('G','E','T','A') ///< \type{INT}\rw Return the average data
#define AOA_SWEEP_GET_STATUS_DATA       PKEY_CONST('G','E','T','S') ///< \type{INT}\rw Return the status records
#define AOA_SWEEP_GET_TIME_DATA         PKEY_CONST('G','E','T','T') ///< \type{INT}\rw Return the time data
/**
	\brief \type{INT}\rw Return the vector data

	The vector data can be calculated on the peak or average of the captured data.

	1 = Peak, 2 = Average
*/
#define AOA_SWEEP_GET_VECTOR_DATA       PKEY_CONST('G','E','T','V')
/**
	\brief \type{RAW}\ro Returned the vector data

	The returned vector data. It is packed into the following structure:

	\code{.c}
	#pragma pack(push, 1)
	typedef struct _Bearing
	{
	    uint16_t    angle;		// Angle in radians*1000
	    uint8_t     confidence; // Confidence of angle. 0 to 255, 255 is most confident
	    uint8_t     amplitude;  // Maximum amplitude across all antennas used in the AOA sweep
	} TBearing;
	#pragma pack(pop)
	\endcode
*/
#define AOA_SWEEP_VECTOR_DATA           PKEY_CONST('V','D','A','T')
#define AOA_SWEEP_DATA_SUB_ANT_1        PKEY_CONST('D','A','T','1') ///< \type{SPEC8}\ro Spectral data for antenna 1
#define AOA_SWEEP_DATA_SUB_ANT_2        PKEY_CONST('D','A','T','2') ///< \type{SPEC8}\ro Spectral data for antenna 2
#define AOA_SWEEP_DATA_SUB_ANT_3        PKEY_CONST('D','A','T','3') ///< \type{SPEC8}\ro Spectral data for antenna 3
#define AOA_SWEEP_DATA_SUB_ANT_4        PKEY_CONST('D','A','T','4') ///< \type{SPEC8}\ro Spectral data for antenna 4
#define AOA_SWEEP_DATA_SUB_ANT_5        PKEY_CONST('D','A','T','5') ///< \type{SPEC8}\ro Spectral data for antenna 5
#define AOA_SWEEP_DATA_SUB_ANT_6        PKEY_CONST('D','A','T','6') ///< \type{SPEC8}\ro Spectral data for antenna 6
#define AOA_SWEEP_DATA_SUB_ANT_7        PKEY_CONST('D','A','T','7') ///< \type{SPEC8}\ro Spectral data for antenna 7
#define AOA_SWEEP_DATA_SUB_ANT_8        PKEY_CONST('D','A','T','8') ///< \type{SPEC8}\ro Spectral data for antenna 8
#define AOA_SWEEP_DATA_SUB_ANT_9        PKEY_CONST('D','A','T','9') ///< \type{SPEC8}\ro Spectral data for antenna 9
#define AOA_SWEEP_DATA_SUB_ANT_10       PKEY_CONST('D','A','1','0') ///< \type{SPEC8}\ro Spectral data for antenna 10
#define AOA_SWEEP_DATA_SUB_ANT_11       PKEY_CONST('D','A','1','1') ///< \type{SPEC8}\ro Spectral data for antenna 11
#define AOA_SWEEP_DATA_SUB_ANT_12       PKEY_CONST('D','A','1','2') ///< \type{SPEC8}\ro Spectral data for antenna 12
#define AOA_SWEEP_DATA_SUB_ANT_13       PKEY_CONST('D','A','1','3') ///< \type{SPEC8}\ro Spectral data for antenna 13
#define AOA_SWEEP_DATA_SUB_ANT_14       PKEY_CONST('D','A','1','4') ///< \type{SPEC8}\ro Spectral data for antenna 14
#define AOA_SWEEP_DATA_SUB_ANT_15       PKEY_CONST('D','A','1','5') ///< \type{SPEC8}\ro Spectral data for antenna 15
#define AOA_SWEEP_DATA_SUB_ANT_16       PKEY_CONST('D','A','1','6') ///< \type{SPEC8}\ro Spectral data for antenna 16
#define AOA_SWEEP_AVG_SUB_ANT_1         PKEY_CONST('A','A','D','1') ///< \type{SPEC8}\ro Average spectral data for antenna 1
#define AOA_SWEEP_AVG_SUB_ANT_2         PKEY_CONST('A','A','D','2') ///< \type{SPEC8}\ro Average spectral data for antenna 2
#define AOA_SWEEP_AVG_SUB_ANT_3         PKEY_CONST('A','A','D','3') ///< \type{SPEC8}\ro Average spectral data for antenna 3
#define AOA_SWEEP_AVG_SUB_ANT_4         PKEY_CONST('A','A','D','4') ///< \type{SPEC8}\ro Average spectral data for antenna 4
#define AOA_SWEEP_AVG_SUB_ANT_5         PKEY_CONST('A','A','D','5') ///< \type{SPEC8}\ro Average spectral data for antenna 5
#define AOA_SWEEP_AVG_SUB_ANT_6         PKEY_CONST('A','A','D','6') ///< \type{SPEC8}\ro Average spectral data for antenna 6
#define AOA_SWEEP_AVG_SUB_ANT_7         PKEY_CONST('A','A','D','7') ///< \type{SPEC8}\ro Average spectral data for antenna 7
#define AOA_SWEEP_AVG_SUB_ANT_8         PKEY_CONST('A','A','D','8') ///< \type{SPEC8}\ro Average spectral data for antenna 8
#define AOA_SWEEP_AVG_SUB_ANT_9         PKEY_CONST('A','A','D','9') ///< \type{SPEC8}\ro Average spectral data for antenna 9
#define AOA_SWEEP_AVG_SUB_ANT_10        PKEY_CONST('A','A','1','0') ///< \type{SPEC8}\ro Average spectral data for antenna 10
#define AOA_SWEEP_AVG_SUB_ANT_11        PKEY_CONST('A','A','1','1') ///< \type{SPEC8}\ro Average spectral data for antenna 11
#define AOA_SWEEP_AVG_SUB_ANT_12        PKEY_CONST('A','A','1','2') ///< \type{SPEC8}\ro Average spectral data for antenna 12
#define AOA_SWEEP_AVG_SUB_ANT_13        PKEY_CONST('A','A','1','3') ///< \type{SPEC8}\ro Average spectral data for antenna 13
#define AOA_SWEEP_AVG_SUB_ANT_14        PKEY_CONST('A','A','1','4') ///< \type{SPEC8}\ro Average spectral data for antenna 14
#define AOA_SWEEP_AVG_SUB_ANT_15        PKEY_CONST('A','A','1','5') ///< \type{SPEC8}\ro Average spectral data for antenna 15
#define AOA_SWEEP_AVG_SUB_ANT_16        PKEY_CONST('A','A','1','6') ///< \type{SPEC8}\ro Average spectral data for antenna 16
#define AOA_SWEEP_STATUS_SUB_ANT_1      PKEY_CONST('A','S','D','1') ///< \type{RAW}\ro Status records for antenna 1
#define AOA_SWEEP_STATUS_SUB_ANT_2      PKEY_CONST('A','S','D','2') ///< \type{RAW}\ro Status records for antenna 2
#define AOA_SWEEP_STATUS_SUB_ANT_3      PKEY_CONST('A','S','D','3') ///< \type{RAW}\ro Status records for antenna 3
#define AOA_SWEEP_STATUS_SUB_ANT_4      PKEY_CONST('A','S','D','4') ///< \type{RAW}\ro Status records for antenna 4
#define AOA_SWEEP_STATUS_SUB_ANT_5      PKEY_CONST('A','S','D','5') ///< \type{RAW}\ro Status records for antenna 5
#define AOA_SWEEP_STATUS_SUB_ANT_6      PKEY_CONST('A','S','D','6') ///< \type{RAW}\ro Status records for antenna 6
#define AOA_SWEEP_STATUS_SUB_ANT_7      PKEY_CONST('A','S','D','7') ///< \type{RAW}\ro Status records for antenna 7
#define AOA_SWEEP_STATUS_SUB_ANT_8      PKEY_CONST('A','S','D','8') ///< \type{RAW}\ro Status records for antenna 8
#define AOA_SWEEP_STATUS_SUB_ANT_9      PKEY_CONST('A','S','D','9') ///< \type{RAW}\ro Status records for antenna 9
#define AOA_SWEEP_STATUS_SUB_ANT_10     PKEY_CONST('A','S','1','0') ///< \type{RAW}\ro Status records for antenna 10
#define AOA_SWEEP_STATUS_SUB_ANT_11     PKEY_CONST('A','S','1','1') ///< \type{RAW}\ro Status records for antenna 11
#define AOA_SWEEP_STATUS_SUB_ANT_12     PKEY_CONST('A','S','1','2') ///< \type{RAW}\ro Status records for antenna 12
#define AOA_SWEEP_STATUS_SUB_ANT_13     PKEY_CONST('A','S','1','3') ///< \type{RAW}\ro Status records for antenna 13
#define AOA_SWEEP_STATUS_SUB_ANT_14     PKEY_CONST('A','S','1','4') ///< \type{RAW}\ro Status records for antenna 14
#define AOA_SWEEP_STATUS_SUB_ANT_15     PKEY_CONST('A','S','1','5') ///< \type{RAW}\ro Status records for antenna 15
#define AOA_SWEEP_STATUS_SUB_ANT_16		PKEY_CONST('A','S','1','6') ///< \type{RAW}\ro Status records for antenna 16
/** @} */


#endif
