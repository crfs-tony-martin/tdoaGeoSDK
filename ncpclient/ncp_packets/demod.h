/*-----------------------------------------------------------------------------
 * (c) CRFS Limited, 2011. All rights reserved.
 *
 * This software is the property of CRFS Limited and may not be copied or
 * reproduced otherwise than on to a single hard disk for backup or
 * archival purposes. The source code is confidential information and must
 * not be disclosed to third parties or used without the express written
 * permission of CRFS Limited.
 *
 * Filename : demod.h
 *---------------------------------------------------------------------------*/
#ifndef __DEMOD_H
#define __DEMOD_H

/** \file  demod.h

	Field and options to request\\retreive audio data
*/

/** 
	\ingroup FIELD_GROUP
	\brief Request/retrieve demodulated data

	The command can return AM/FM demodulated data as PCM data. The specturm for each of the captures can also be 
	retrieved.

	See \ref DEMOD_GROUP 
	\frw
*/
#define FIELD_DEMODULATION                  	PKEY_CONST('D','M','O','D')

/**
	@defgroup DEMOD_GROUP  Audio demodulation options

	Audio demodulation involves changing the frequency (FM) or amplitude (AM) variations in a RF signal into a baseband 
	audio signal. This field provides the options for both AM and FM audio demodulation. This allows for configuring 
	audio demodulation related parameters and retrieving the received audio data. The detailed description of each 
	options are as follows.

	\note  If the DDS freq is less than the decimated RF bandwidth then the demodulator can be blocked by the DC offsets 
		   from the IQ demod for small signals. Thus, it is suggested that, even if it is possible to tune the radio 
		   precicely to the desired demod freq, the radio is tuned off by at least the decimated RF bandwidth. The DDS 
		   can then be used to bring the offset radio freq back to the desired demod freq.

	\code
	    // Request FM demodulation retreiving 
	    T_PACKET* txpacket = packet_create("", 8192);
    	packet_write(txpacket, PACKET_TYPE_DSP_CONTROL, -1);
        // Add the AOA sweep field
        packet_add_field(txpacket, FIELD_DEMODULATION, -1);
            // Tune to FM station at 96.6 Mhz
            packet_add_param_int(txpacket, DEMOD_RADIO_TUNE_FREQ_MHZ, 96);
            packet_add_param_int(txpacket, DEMOD_DDS_FREQ_KHZ, 600);
			// FM Demodulation
			packet_add_param_int(txpacket, DEMOD_MODE_FM, 1);
			// Request the spectrum data
			packet_add_param_int(txpacket, DEMOD_GET_SPECTRUM_DATA, 1)
			// Only request specturm data returned twice a second
			packet_add_param_int(txpacket, DEMOD_SPECTRUM_DELAY, 500);
	    packet_write_complete(txpacket);
	\endcode
	@{
*/
#define DEMOD_RADIO_TUNE_FREQ_MHZ           PKEY_CONST('F','C','T','R') ///< \type{INT}\rw Center Frequency (MHz)
/**
	\brief \type{INT}\rw DDS Offset

	This option controlls the frequency of the Direct Digital Synthesizer used for off setting the demodulated tuned 
	frequency from the radio tuned frequency. If this option is set to zero then the demodulated data is centered on 
	the tuned freq of the radio.
*/
#define DEMOD_DDS_FREQ_KHZ                  PKEY_CONST('F','D','D','S')
#define DEMOD_INPUT                         PKEY_CONST('I','N','P','T') ///< \type{INT}\rw \deprecated See \ref ANY_INPUT_NODE
#define DEMOD_RF_DECIMATION                 PKEY_CONST('D','E','C','I') ///< \type{INT}\rw Hardware RF decimation
#define DEMOD_GAIN                          PKEY_CONST('D','G','A','I') ///< \type{INT}\rw Radio gain applied to capture
/**
	\brief \type{INT}\rw FM Mute threshold

	Threshold level at which data returned from the demodulator will be zero'd. Use this in conjunction 
	\ref DEMOD_SILENCE_THRESHOLD to stop "silent" data from being returned.
*/
#define DEMOD_FM_MUTE_THD                   PKEY_CONST('M','U','T','E')
/**
	\brief \type{INT}\rw Silence threshold

	Threshold level at which data returned from the demodulator will be zero'd. Use this in conjunction 
	\ref DEMOD_FM_MUTE_THD to stop "silent" data from being returned.
*/
#define DEMOD_SILENCE_THRESHOLD             PKEY_CONST('S','I','L','E')
/**
	\brief \type{INT}\rw Audio compression of data. 1 - 4 (Maximum)

	The node can compress the data using the G726 codec.

*/
#define DEMOD_COMPRESSION_LEVEL             PKEY_CONST('C','O','M','P')/*[int32,RW,false]   Level of compression to apply to data. 1 to 4(Maximum). */

/**
	@name Demodulation options
	
	The demodulator supports two modulation schemes. Options are mutual exclusive.
*/
///@{
#define DEMOD_MODE_FM                       PKEY_CONST('F','M','D','E') ///< \type{INT}\rw FM demodulation
#define DEMOD_MODE_AM                       PKEY_CONST('A','M','D','E') ///< \type{INT}\rw AM demodulation
#define DEMOD_MODE_OFF                      PKEY_CONST('D','O','F','F') ///< \type{INT}\rw Disable demodulation \deprecated Removed
///@}
/**
	\brief \type{INT}\rw Data decimation

	This option controls the sample rate for the data entering the detector. The RF decimation filter defines the 
	decimation RF channel selectivity. 

	\note A decimation factor of 100 on 40MHz sample rate with 20MHz RTBW, equates to 
		  a decimated sample rate of 400kHz with 200kHz RTBW.
*/
#define DEMOD_DATA_DECIMATION               PKEY_CONST('D','D','E','C')
#define DEMOD_DATA                          PKEY_CONST('D','D','A','T')	///< \type{Demod16}\ro Returned audio data
/**
	\brief \type{INT}\wo Request associated RF spectrum data
	
	Include the RF spectrum data with the audio data. See \ref DEMOD_SPECTRUM_DELAY to manage the return rate.
*/
#define DEMOD_GET_SPECTRUM_DATA             PKEY_CONST('G','S','P','E')
/**
	\brief \type{INT}\rw RF Spectrum delay (ms)

	NCPd will send the RF spectrum for each capture that makes up the audio data, this can be many times a second. Use
	this option to only send the RF spectrum every x ms.
*/
#define DEMOD_SPECTRUM_DELAY                PKEY_CONST('S','D','E','L') ///< \type{INT}\rw 
#define DEMOD_SPECTRUM_DATA                 PKEY_CONST('S','P','E','C') ///< \type{Spec8}\ro Returned RF spectrum data
/** @} */

#endif
