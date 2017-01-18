/*-----------------------------------------------------------------------------
 * (c) CRFS Limited, 2011. All rights reserved.
 *
 * This software is the property of CRFS Limited and may not be copied or
 * reproduced otherwise than on to a single hard disk for backup or
 * archival purposes. The source code is confidential information and must
 * not be disclosed to third parties or used without the express written
 * permission of CRFS Limited.
 *
 * Filename : internal.h
 *---------------------------------------------------------------------------*/
#ifndef __INTERNAL_H
#define __INTERNAL_H

/** \file  internal.h

	Field and options not currently supported. This could be because they are for debug purposes,
	WIP features, experimental hardware, features that are not enabled, etc.
*/

/// \cond No_Documentation
#define FIELD_CAL_STATUS 						PKEY_CONST('C','A','L','S')
#define FIELD_DEBUG                             PKEY_CONST('D','B','U','G')
#define FIELD_ATTENUATOR                    	PKEY_CONST('A','T','T','E')
#define FIELD_GSM_AOA                           PKEY_CONST('A','G','S','M')
#define FIELD_AOA_SPIN                 			PKEY_CONST('A','O','S','P')
#define FIELD_LICENSE                       	PKEY_CONST('F','L','I','C')

#define DEBUG_NETWORK_MONITOR                   PKEY_CONST('D','M','O','N')/*[bool,WO,false]    debug network monitor */
#define DEBUG_NETWORK_MONITOR_RX_TIME_SEC       PKEY_CONST('R','X','T','S')/*[int,WO,false]     debug network monitor packet RX time seconds*/
#define DEBUG_NETWORK_MONITOR_RX_TIME_NSEC      PKEY_CONST('R','X','T','N')/*[int,WO,false]     debug network monitor packet RX time nano seconds*/
#define DEBUG_NETWORK_MONITOR_TX_TIME_SEC       PKEY_CONST('T','X','T','S')/*[int,WO,false]     debug network monitor packet TX time seconds*/
#define DEBUG_NETWORK_MONITOR_TX_TIME_NSEC      PKEY_CONST('T','X','T','N')/*[int,WO,false]     debug network monitor packet TX time nano seconds*/
#define DEBUG_NETWORK_MONITOR_CLIENT_IP         PKEY_CONST('C','L','I','P')/*[string,RO,false]  client identifier*/
#define DEBUG_NETWORK_MONITOR_DIRECTION         PKEY_CONST('D','I','R','E')/*[int,RO,false]     direction 0 = out, 1 = in */
#define DEBUG_SHOW_RX_PACKETS                   PKEY_CONST('R','X','P','K')/*[bool,WO,false]    show rx packets */
#define DEBUG_SHOW_TX_PACKETS                   PKEY_CONST('T','X','P','K')/*[bool,WO,false]    show tx packets */
#define DEBUG_SOCKET_TX                         PKEY_CONST('S','K','T','X')/*[bool,WO,false]    show tx packet transmissions */
#define DEBUG_SOCKET_RX                         PKEY_CONST('S','K','R','X')/*[bool,WO,false]    show rx packet transmissions */
#define DEBUG_NODE_CONTROL_LOOP                 PKEY_CONST('N','C','T','L')/*[bool,WO,false]    show node control loop debug info */
#define DEBUG_RDB_WAIT                          PKEY_CONST('R','D','B','W')/*[bool,WO,false]    show rdb wait debug info */
#define DEBUG_RDB                               PKEY_CONST('R','D','B',' ')/*[bool,WO,false]    show rdb debug info */
#define DEBUG_CDB                               PKEY_CONST('C','D','B',' ')/*[bool,WO,false]    show cdb debug info */

#define LICENSE_ISSUED_TO                   PKEY_CONST('L','I','T','O')/*[string,RO]        License issued to*/
#define LICENSE_VALID                       PKEY_CONST('L','I','V','A')/*[int32,RO]         License valid for node*/
#define LICENSE_ENABLED                     PKEY_CONST('L','I','E','N')/*[string,RO]        License enabled*/

enum config_aoa_spin {
    AOA_SPIN_ENABLE   = PKEY_CONST('A','O','S','E'), // 1=enable, 0=disable, default is 1
    AOA_SPIN_NUM_ANT  = PKEY_CONST('A','O','N','A'), // number of antennas in spin, not including trigger antenna, def=1
    AOA_SPIN_ANT_TRIG = PKEY_CONST('A','O','A','T'), // antenna used in trigger (1..7)
    AOA_SPIN_ANT_1    = PKEY_CONST('A','O','A','1'), // antenna N (1..7) in spin
    AOA_SPIN_ANT_2    = PKEY_CONST('A','O','A','2'),
    AOA_SPIN_ANT_3    = PKEY_CONST('A','O','A','3'),
    AOA_SPIN_ANT_4    = PKEY_CONST('A','O','A','4'),
    AOA_SPIN_ANT_5    = PKEY_CONST('A','O','A','5'),
    AOA_SPIN_ANT_6    = PKEY_CONST('A','O','A','6'),
    AOA_SPIN_ANT_7    = PKEY_CONST('A','O','A','7'),
    AOA_SPIN_LOG_NLIN = PKEY_CONST('A','O','L','G'), // log, not linear, threshold, default 0
    AOA_SPIN_ARRAY_UID= PKEY_CONST('A','O','S','U'),  // UID of array to use
    AOA_SPIN_TRIGGER_UID = PKEY_CONST('A','S','T','U'),  // Trigger antenna to use
};


#define GSM_AOA_CHANNEL_NUMBER              PKEY_CONST('C','H','A','N')/*[int32,RW,776]     GSM Channel Number */
#define GSM_AOA_CHANNEL_DIR_UPLINK          PKEY_CONST('U','L','N','K')/*[bool,RW,false]    GSM uplink */
#define GSM_AOA_CHANNEL_DIR_DOWNLINK        PKEY_CONST('D','L','N','K')/*[bool,RW,true]     GSM downlink */
#define GSM_AOA_PCS1900_MODE                PKEY_CONST('P','C','1','9')/*[bool,RW,false]    PCS 1900 mode */
#define GSM_AOA_MANUAL_ATTEN                PKEY_CONST('A','T','E','N')/*[int32,RW,auto]    input attenuation */
#define GSM_AOA_NUM_SAMPLES                 PKEY_CONST('S','A','M','P')/*[int32,RW,5000]    GSM capture number of samples */
#define GSM_AOA_INPUT                       PKEY_CONST('I','N','P','T')/*[int32,RW,1]       input selection 1,2,3,4 */
#define GSM_AOA_SUB_ANTENNA                 PKEY_CONST('S','U','B','A')/*[int32,RW,0]       sub antenna selection */
#define GSM_AOA_EXPANSION_PORT              PKEY_CONST('E','X','P','O')/*[int32,RW,1]       expansion port used to drive switch */
#define GSM_AOA_REF_LEVEL                   PKEY_CONST('R','L','E','V')/*[int32,RW,-20]     reference level */
#define GSM_AOA_DIGITAL_ATTEN               PKEY_CONST('D','A','T','N')/*[int32,RW,0]       digital atten */
#define GSM_AOA_GSM_CONTROL_REG             PKEY_CONST('G','C','T','L')/*[uint32,RW]      gsm control reg */
#define GSM_AOA_ANTENNA_CONFIG              PKEY_CONST('A','C','O','N')/*[uint32,RW,0x6540321]antenna configuration */
#define GSM_AOA_FRAME_NULL_THD              PKEY_CONST('F','C','N','U')/*[int32,RW,30]      frame correlator null threshold */
#define GSM_AOA_FRAME_POWER_THD             PKEY_CONST('F','C','P','T')/*[int32,RW,1418]    frame correlator power threshold */
#define GSM_AOA_FRAME_LOCK_TIMEOUT          PKEY_CONST('F','C','L','T')/*[int32,RW,100]     frame correlator lock timeout */
#define GSM_AOA_DDS_FREQ                    PKEY_CONST('F','D','D','S')/*[int32,RO]         DDS freq kHz */
#define GSM_AOA_RADIO_FREQ                  PKEY_CONST('F','R','A','D')/*[int32,RO]         radio tuned freq MHz */
#define GSM_AOA_DC_I                        PKEY_CONST('D','C','O','I')/*[int32,RO]         dc offset I */
#define GSM_AOA_DC_Q                        PKEY_CONST('D','C','O','Q')/*[int32,RO]         dc offset Q */
#define GSM_AOA_RADIO_ERROR                 PKEY_CONST('E','R','A','D')/*[int32,RO]         radio error */
#define GSM_AOA_PEAK_POWER                  PKEY_CONST('P','K','P','O')/*[int32,RO]         peak power value */
#define GSM_AOA_SPIN_ANTENNAS               PKEY_CONST('S','P','I','N')/*[bool,RW,true]     spin antennas */
#define GSM_AOA_GET_I_Q_DATA                PKEY_CONST('G','I','Q','T')/*[bool,WO,true]     get I Q Time Data */
#define GSM_AOA_GET_POWER_DATA              PKEY_CONST('G','P','W','R')/*[boole,WO,true]    get power data */
#define GSM_AOA_IQ_TIME_DATA                PKEY_CONST('D','I','Q','T')/*[IQtime,RO]        IQ time data */
#define GSM_AOA_POWER_DATA                  PKEY_CONST('D','P','W','R')/*[GSMmag,RO]        GSM power data */


enum {
    CAL_STARTING_RECAL       = PKEY_CONST('C','S','T','A'), /*[bool,RO] a cal operation is about to start */
    CAL_RECAL_COMPLETE       = PKEY_CONST('C','C','O','M')  /*[bool,RO] a cal operation has completed, 1=>pass, 0=>fail */
};

/* Attenuator stuff */
#define ATTENUATOR_DEVICE_ID                PKEY_CONST('D','E','V','I')
#define ATTENUATOR_0                        PKEY_CONST('A','T','T','0')
#define ATTENUATOR_1                        PKEY_CONST('A','T','T','1')
#define ATTENUATOR_2                        PKEY_CONST('A','T','T','2')
#define ATTENUATOR_3                        PKEY_CONST('A','T','T','3')
#define ATTENUATOR_4                        PKEY_CONST('A','T','T','4')
#define ATTENUATOR_5                        PKEY_CONST('A','T','T','5')
#define ATTENUATOR_6                        PKEY_CONST('A','T','T','6')
#define ATTENUATOR_7                        PKEY_CONST('A','T','T','7')
/// \endcond

#endif
