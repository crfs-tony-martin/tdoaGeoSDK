/*-----------------------------------------------------------------------------
 * (c) CRFS Limited, 2011. All rights reserved.
 *
 * This software is the property of CRFS Limited and may not be copied or
 * reproduced otherwise than on to a single hard disk for backup or
 * archival purposes. The source code is confidential information and must
 * not be disclosed to third parties or used without the express written
 * permission of CRFS Limited.
 *
 * Filename : ncp_packets.h
 * Author : Stewart Hyde
 *
 * Purpose: NCP packet key defines
 *
 * Description: Provides translation of field/parameter defines to
 *      32bit, 4 character NCP packet keys.
 *---------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
/**
    \defgroup   NCP_packets

    \brief


    \author     Stewart Hyde
    \author     CRFS Ltd
    \version    A06.3
    \date       3/12/10

    //  3/12/10
    //  Added sync sweep function to standard sweep command
    //  Added ref clock control
    //  Added sync trig config control

    //  6/12/10 A06.1
    //  Corrected errors in placement of 'extern "C"' declarations
    //  Commented out parameters/fields not currenly implemented on the server

    //  20/1/11 A06.3
    //  Added ANY_REPEAT_NUMBER
    //  Added SWEEP_REPEAT_DELAY_UNIX
    //  Added SWEEP_REPEAT_DELAY_NANO
    //  Added DEMOD_GAIN
    //  Added DEMOD_FM_MUTE_THD
    //  Added DEMOD_AM_BIAS_PERIOD
    //  Added DEMOD_POWER_LEVEL_TRIG_THD_LIN
    //  Added DEMOD_POWER_LEVEL_TRIG_THD_LOG
    //  Added more comprehensive range checking on incoming paramters
    //  Added version commands

*/
/*--------------------------------------------------------------------------*/

#ifndef NCP_PACKETS_H_INCLUDED
#define NCP_PACKETS_H_INCLUDED

#include "ncp_packet_types.h"

#ifdef __cplusplus
extern "C" {
#endif
#if 0
}
#endif

#define VERSION_NCP_PACKET_H  "Alpha A06.3  20/1/11 16:00"


/**-------------------------------------------------------------------------
                         PACKET PARAMETER FORMATS
--------------------------------------------------------------------------*/
/*
//  each field/parameter in this file has additional information in square brackets following the definition e.g. [int32,RW,-20]

//  the format of the information is [parameter_type, access_type, default_value]

/// parameter types:-

//  int32   -   signed 32 bit integer

//  uint32  -   unsigned 32 bit integer

//  bool    -   boolean (1=true,0=false) currently encoded as int32

//  spec8   -   array of unsigned 8 bit values representing spectrum in dB*2 (dynamic range is 127.5dB), reference level is provided in a separate parameter
//              dBm_spectrum_value = spec8/2-127.5+reference_level.  e.g. spec8=100, ref_level=-20, dBm_spectrum_value=100/2 -127.5 -20 =  -97.5dBm

//  timeIQ  -   array of 16bit 2s complement data representing the I and Q ADC sample values in an alternating I,Q,I,Q sequence

//  demod16 -   array of 16bit 2s complement demodulated data from the hardware demodulator output

//  GSMmag  -   array of 16bit linear magnitude values from the GSM AOA antenna array

//  timeHMSC:
//     byte3 (bits 31:24)  - hour       (in binary)
//     byte2 (bits 23:16)  - minute     (in binary)
//     byte1 (bits 15:8)   - second     (in binary)
//     byte0 (bits 7:0)    - centi sec  (in binary)

//  dateDMYC:
//      byte3 (bits 31:24)  - day       (in binary)
//      byte2 (bits 23:16)  - month     (in binary)
//      byte1 (bits 15:8)   - year      (in binary)
//      byte0 (bits 7:0)    - centuary  (in binary)


///  access_types:-

//  RO  -   Read Only
//  RW  -   Read or Write
//  WO  -   Write Only

/// default_values:-

//  only writable parameters have default values
*/

/**************************************************************//**
 *                         NCP RETURN CODES
 *****************************************************************/
// Codes returned in ANY_ERROR or ANY_WARNING
#define ERROR_UNKNOWN                       0xf000
#define ERROR_TIMEOUT                       0xf002
#define ERROR_NCP_INIT                      0xf003
#define ERROR_UNRECOGNISED_PACKET           0xf004
#define ERROR_MULTI_SYNC                    0xf005
#define ERROR_OUT_OF_MEMORY                 0xf006
#define ERROR_NO_LOCK                       0xf007

#define WARNING_UNKNOWN                     0x0f00
#define WARNING_UNRECOGNISED_FIELD          0x0f01
#define WARNING_UNRECOGNISED_PARAMETER      0x0f02
#define WARNING_CALIBRATION_NOT_SUPPORTED   0x0f03
#define WARNING_AGC_OVERFLOW                0x0f04
#define WARNING_UNCALIBRATED_SWEEP          0x0f05
#define WARNING_BDC_NOT_FOUND               0x0f06
#define WARNING_ADC_OVERFLOW                0x0f07
#define WARNING_DBM_MEAN_UNDERFLOW          0x0f08
#define WARNING_DBM_MEAN_OVERFLOW           0x0f09
#define WARNING_DBM_PEAK_UNDERFLOW          0x0f0A
#define WARNING_DBM_PEAK_OVERFLOW           0x0f0B


//    enum CMDS {MY_FIELD_ERR = PKEY_CONST('E','R','R','O')};
/**************************************************************//**
 *                         NCP GENERAL FIELDS
 *****************************************************************/
/*     These fields are provided to allow notification of problems
 *   not directly attributable to a particular instruction field
 *   (eg. over temperature warning) */
#define FIELD_ERROR                             PKEY_CONST('E','R','R','O')/* [field,RO]         general error field */
#define FIELD_WARN                              PKEY_CONST('W','A','R','N')/*[field,RO]         general warning field */

/**************************************************************//**
 *                         NCP 'ANY' PARAMETERS
 *****************************************************************/
/*      'ANY' PARAMETER KEYS CAN APPEAR IN ANY FIELD */
#define ANY_ACKNOWLEDGE_PACKET                  PKEY_CONST('A','C','K','N')/*[int32,RO]         acknowledge packet_id */
#define ANY_ERROR                               PKEY_CONST('E','R','R','O')/*[string,RO]        error information */
#define ANY_ERROR_CODE                          PKEY_CONST('E','R','R','C')/*[int32,RO]         error information */
#define ANY_WARNING                             PKEY_CONST('W','A','R','N')/*[string,RO]        warning information */
#define ANY_WARNING_CODE                        PKEY_CONST('W','A','R','C')/*[int32,RO]         warning information */
#define ANY_REPEAT_NUMBER                       PKEY_CONST('R','N','U','M')/*[int32,RO]         repeat number - the number of times times the operation has executed */
#define ANY_NUM_DATA_SEGMENTS                   PKEY_CONST('N','S','E','G')/*[int32,RO]         number of separate data segments to be used in returning the data */
#define ANY_DATA_SEGMENT_NUMBER                 PKEY_CONST('S','E','G','N')/*[int32,RO]         segment number of the current data */
#define ANY_LICENSE_ERROR                       PKEY_CONST('L','I','C','E')/*[int32,RO]         license not valid for this function */

/**************************************************************//**
 *                      NCP 'NODE' NODE PACKETS
 *****************************************************************/

  /*/     NCP VERSIONS - retrieve static information about the embedded linux system */
#define FIELD_NCP_VERSIONS                      PKEY_CONST('N','C','P','V')/*[field,RW]         NCP versions */
#define NCP_VERSIONS_SERVER_MAJOR               PKEY_CONST('N','V','M','A')/*[int,RO]           NCP versions major*/
#define NCP_VERSIONS_SERVER_MINOR               PKEY_CONST('N','V','M','I')/*[int,RO]           NCP versions minor*/
#define NCP_VERSIONS_SERVER_RELEASE             PKEY_CONST('R','E','L','V')/*[string,RO]        NCP server version */
#define NCP_VERSIONS_CLIENT                     PKEY_CONST('C','L','I','V')/*[string,RO]        NCP client version */
#define NCP_VERSIONS_NCP_PACKETS_H              PKEY_CONST('P','A','K','H')/*[string,RO]        NCP packets version */
#define NCP_VERSIONS_PACKETS                    PKEY_CONST('P','A','K','V')/*[string,RO]        Packets version */
#define NCP_VERSIONS_COM_INTERFACE              PKEY_CONST('C','O','M','V')/*[string,RO]        Packets version */

//#define FIELD_NCP_CONTROL                       PKEY_CONST('N','C','P','V')/*[field,RW]         NCP versions */
//#define NCP_RESTART                             PKEY_CONST('R','E','S','T')/*[string,RO]        Packets version */

/*     LINUX INFO - retrieve static information about the embedded linux system */
#define FIELD_LINUX_INFO                        PKEY_CONST('L','I','N','F')/*[field,RW]         linux system info */
    #define LINUX_INFO_VERSION                  PKEY_CONST('V','E','R','S')/*[string,RO]        linux version */
    #define LINUX_INFO_RELEASE                  PKEY_CONST('R','E','L','E')/*[string,RO]        linux release */
    #define LINUX_INFO_MACHINE                  PKEY_CONST('M','A','C','H')/*[string,RO]        machine information */
#define LINUX_INFO_SYSNAME                      PKEY_CONST('S','Y','S','N')/*[string,RO]        system name */
#define LINUX_INFO_NODENAME                     PKEY_CONST('N','O','D','N')/*[string,RO]        nodename (hostname) */
#define LINUX_INFO_CLOCK_TICKS                  PKEY_CONST('C','L','K','T')/*[int32,RO]         linux clock ticks */
#define LINUX_INFO_REALTIME                     PKEY_CONST('R','T','O','S')/*[bool,RO]          linux supports real-time extensions */
#define LINUX_INFO_REALTIME_THREADS             PKEY_CONST('R','T','T','H')/*[bool,RO]          linux supports real-time threads */
#define LINUX_INFO_THREAD_PRIORITY_SCHEDULING   PKEY_CONST('T','P','R','I')/*[int32,RO]       linux thread priority scheduling */
#define LINUX_INFO_MAC_ADDRESS                  PKEY_CONST('M','A','C','A')/*[int32,RO]       linux thread priority scheduling */

/*     LINUX MONITOR - retrieve dynamic status information about the embedded linux system*/
#define FIELD_LINUX_MONITOR                     PKEY_CONST('L','M','O','N')/*[field,RW]         linux monitor status */
#define LINUX_MONITOR_MEM_FREE                  PKEY_CONST('M','F','R','E')/*[int32,RO]         linux system free memory */
#define LINUX_MONITOR_MEM_USED                  PKEY_CONST('M','U','S','E')/*[int32,RO]         linux system used memory */
#define LINUX_MONITOR_USER_SEC                  PKEY_CONST('U','S','E','C')/*[int32,RO]         Time spent executing user instructions (seconds). */
#define LINUX_MONITOR_USER_USEC                 PKEY_CONST('U','U','S','E')/*[int32,RO]         Time spent executing user instructions (usec). */
#define LINUX_MONITOR_OS_SEC                    PKEY_CONST('O','S','E','C')/*[int32,RO]         Time spent in operating system code on behalf of processes(sec) */
#define LINUX_MONITOR_OS_USEC                   PKEY_CONST('O','U','S','E')/*[int32,RO]         Time spent in operating system code on behalf of processes(usec) */
#define LINUX_MONITOR_MAXRSS                    PKEY_CONST('M','R','S','S')/*[int32,RO]         The maximum resident set size used, in kilobytes. That is, the maximum number of kilobytes of physical memory that processes used simultaneously. */
#define LINUX_MONITOR_IXRSS                     PKEY_CONST('X','R','S','S')/*[int32,RO]         An integral value expressed in kilobytes times ticks of execution, which indicates the amount of memory used by text that was shared with other processes. */
#define LINUX_MONITOR_IDRSS                     PKEY_CONST('D','R','S','S')/*[int32,RO]         An integral value expressed the same way, which is the amount of unshared memory used for data. */
#define LINUX_MONITOR_ISRSS                     PKEY_CONST('I','R','S','S')/*[int32,RO]         An integral value expressed the same way, which is the amount of unshared memory used for stack space. */
#define LINUX_MONITOR_MINFLT                    PKEY_CONST('M','I','N','F')/*[int32,RO]         The number of page faults which were serviced without requiring any I/O. */
#define LINUX_MONITOR_MAJFLT                    PKEY_CONST('M','A','J','F')/*[int32,RO]         The number of page faults which were serviced by doing I/O. */
#define LINUX_MONITOR_NSWAP                     PKEY_CONST('N','S','W','A')/*[int32,RO]         The number of times processes was swapped entirely out of main memory. */
#define LINUX_MONITOR_INBLOCK                   PKEY_CONST('I','N','B','L')/*[int32,RO]         The number of times the file system had to read from the disk on behalf of processes. */
#define LINUX_MONITOR_OUBLOCK                   PKEY_CONST('O','U','B','L')/*[int32,RO]         The number of times the file system had to write to the disk on behalf of processes. */
#define LINUX_MONITOR_MSGSND                    PKEY_CONST('M','S','G','S')/*[int32,RO]         Number of IPC messages sent. */
#define LINUX_MONITOR_MSGRCV                    PKEY_CONST('M','S','G','R')/*[int32,RO]         Number of IPC messages received. */
#define LINUX_MONITOR_NSIGNALS                  PKEY_CONST('N','S','I','G')/*[int32,RO]         Number of signals received. */
#define LINUX_MONITOR_NVCSW                     PKEY_CONST('N','V','C','S')/*[int32,RO]         The number of times processes voluntarily invoked a context switch (usually to wait for some service). */
#define LINUX_MONITOR_NIVCSW                    PKEY_CONST('N','I','V','C')/*[int32,RO]         The number of times an involuntary context switch took place (because a time slice expired, or another process of higher priority was scheduled). */

/**      DEBUG - control node debug information
 *      (this information can currently only be seen if the ncp server is started in an SSH session)
 */
#define FIELD_DEBUG                             PKEY_CONST('D','B','U','G')/*[field,RW]         node debug */
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

/**     FILE - read/write files */
#define FIELD_FILE                              PKEY_CONST('F','I','L','E')/*[field,RW]         read/write files */
#define FILE_READ_FILENAME                      PKEY_CONST('F','R','E','A')/*[string,WO]        read file */
#define FILE_WRITE_FILENAME                     PKEY_CONST('F','W','R','I')/*[string,WO]        write file */
#define FILE_DATA                               PKEY_CONST('F','D','A','T')/*[string,WO]        file data */

/**     SHELL - execute shell commands on the linux processor */
#define FIELD_SHELL                             PKEY_CONST('S','H','E','L')/*[field,RW]         execute linux shell functions */
#define SHELL_EXECUTE                           PKEY_CONST('E','X','E','C')/*[string,WO]        write file */
#define SHELL_EXECUTE_RESULT                    PKEY_CONST('X','R','E','S')/*[string,WO]        shell execute result*/

 /**     RESTART - execute shell commands on the linux processor */
#define FIELD_RESTART                           PKEY_CONST('R','E','S','T')/*[field,RW]         execute linux shell functions */
#define RESTART_NCP                             PKEY_CONST('R','N','C','P')/*[string,WO]        write file */
#define RESTART_LINUX                           PKEY_CONST('R','L','I','N')/*[string,WO]        write file */

/**    UPGRADE - system upgrade */
#define FIELD_UPGRADE                           PKEY_CONST('U','P','G','R')/*[field,RW]         execute linux shell functions */
#define UPGRADE_NCPD                            PKEY_CONST('N','C','P','D')/*[RAW,WO]           write file */

/** CONFIGURE - node configuration */
#define FIELD_CONFIGURE                         PKEY_CONST('C','O','N','F')
#define CONFIGURE_GPS_AUTO                      PKEY_CONST('G','P','S','A') /*[int,RW] automatically send GPS: 0=off, 1=on */
#define CONFIGURE_MONITOR_AUTO                  PKEY_CONST('M','O','N','A') /*[int,RW] automatically send radio and baseband monitor: 0=off, 1=on */
#define CONFIGURE_GPS_ALL                       PKEY_CONST('G','P','S','L') /*[int,RW] automatically add GPS information to all packets: 0=off, 1=on */
#define CONFIGURE_EXP1                          PKEY_CONST('E','X','P','1') /*[string,rw] configure expansion port 1*/
#define CONFIGURE_EXP2                          PKEY_CONST('E','X','P','2') /*[string,rw] configure expansion port 2*/
#define CONFIGURE_STAT_PACKETS                  PKEY_CONST('C','S','T','A') /*[int,rw] 1 = stat packets on, 0 = stat packets off */

/* License information */
#define FIELD_LICENSE                       PKEY_CONST('F','L','I','C')/*[int32,RO]         License information*/
#define LICENSE_ISSUED_TO                   PKEY_CONST('L','I','T','O')/*[string,RO]        License issued to*/
#define LICENSE_VALID                       PKEY_CONST('L','I','V','A')/*[int32,RO]         License valid for node*/
#define LICENSE_ENABLED                     PKEY_CONST('L','I','E','N')/*[string,RO]        License enabled*/

/* Status information */
#define FIELD_STATUS_INFORMATION            PKEY_CONST('S','T','A','T')/*[field, RO]        Node Status information */
#define STATUS_OPEN_CONNECTIONS             PKEY_CONST('O','C','O','N')/*[int32, RO]        Number of open connections */
#define STATUS_CONNECTION_IP_ADDR           PKEY_CONST('C','A','D','D')/*[string, RO]       connection ip addresses csv */
#define STATUS_CLIENT_IDS                   PKEY_CONST('C','I','D','S')/*[string, RO]       connection client id words csv */
#define STATUS_GPS_RAW_STRING               PKEY_CONST('R','G','P','S')/*[string,RO]        GPS raw string */
#define STATUS_EXP_1                        PKEY_CONST('E','X','P','1')/*[string,RO]        Status information for EXP 1 */
#define STATUS_EXP_2                        PKEY_CONST('E','X','P','2')/*[string,RO]        Status information for EXP 2 */
#define STATUS_CONFIG_FILE_PRESENT          PKEY_CONST('C','O','N','P')/*[int32, RO]        Config file present */
#define STATUS_CALIBRATION_SUPPORTED        PKEY_CONST('C','A','L','S')/*[int32, RO]        Calibration supported */


/**************************************************************//**
 *                      NCP 'DSPC'/'DSPL'  PACKETS
 *****************************************************************/

/*-----------------------------------------------------------------
     'ANY_DSP' PARAMETER KEYS CAN APPEAR IN ANY DSP FIELD */
#define ANY_DSP_DEBUG_GET_RDB                   PKEY_CONST('g','R','D','B')/*[bool,WO]          return CDB as string */
#define ANY_DSP_DEBUG_GET_CDB                   PKEY_CONST('g','C','D','B')/*[bool,WO]          return RDB as string */
#define ANY_DSP_DEBUG_GET_BASIC                 PKEY_CONST('g','B','A','S')/*[bool,WO]          return basic debug info */
#define ANY_DSP_TIMEOUT                         PKEY_CONST('T','O','U','T')/*[int32_t,WO]       set command timeout */

#define ANY_DSP_DEBUG_RDB_STRING                PKEY_CONST('d','R','D','B')/*[string,RO]        return CDB as string */
#define ANY_DSP_DEBUG_CDB_STRING                PKEY_CONST('d','C','D','B')/*[string,RO]        return RDB as string */
#define ANY_DSP_DEBUG_BASIC_STRING              PKEY_CONST('d','B','A','S')/*[string,RO]        return basic debug info */

#define ANY_DSP_RDB_ERROR_MSG                   PKEY_CONST('D','R','E','R')/*[string,RO]        return error message */
#define ANY_DSP_RDB_ERROR_CODE                  PKEY_CONST('D','R','E','C')/*[int32,RO]         return error code */

#define ANY_DSP_TIME_US                         PKEY_CONST('D','S','P','T')/*[int32,RO]         time taken by radonblaze to execute command in usec */
#define ANY_DSP_WAIT_US                         PKEY_CONST('D','S','P','W')/*[int32,RO]         linux idle time waiting for radonblaze to finish a command in usec */
#define ANY_DSP_RTC_UNIX_TIME                   PKEY_CONST('U','T','I','M')/*[int32,RO]         DSP RTC in UNIX time_t format */
#define ANY_DSP_RTC_NANO                        PKEY_CONST('N','A','N','O')/*[int32,RO]         DSP RTC time stamp nanoseconds */
#define ANY_DSP_RTC_UTIM_STR                    PKEY_CONST('U','S','T','R')/*[int32,RO]         DSP UTIM time as string */
#define ANY_DSP_COM_MESSAGE                     PKEY_CONST('C','M','S','G')/*[int32,RO]         com command return message for debug */
#define ANY_DSP_DATA_TAG                        PKEY_CONST('D','T','A','G')/*[int32,RW]         tag your requests. If this is sent to NCP it will be sent back. NOTE: Currently only sweep/time */


/*----------------------------------------------------------------- */

/**     VERSIONS - retrive version information from the DSP subsystem */
#define FIELD_VERSIONS                          PKEY_CONST('V','E','R','S')/*[field,RW]         versions */
#define VERSIONS_RADIO_FPGA                     PKEY_CONST('R','F','P','G')/*[int32,RO]         radio FPGA version */
#define VERSIONS_RADIO_MICROCODE                PKEY_CONST('R','M','I','C')/*[int32,RO]         radio microcode version */
#define VERSIONS_DSP_FPGA                       PKEY_CONST('B','F','P','G')/*[int32,RO]         DSP FPGA version */
#define VERSIONS_DSP_MICROCODE                  PKEY_CONST('B','M','I','C')/*[int32,RO]         DSP microcode version */
#define VERSIONS_CPLD                           PKEY_CONST('C','P','L','D')/*[int32,RO]         CPLD version */
#define VERSIONS_RADIO_INIT_TABLE               PKEY_CONST('R','I','N','I')/*[int32,RO]         radio initialization table version */
#define VERSIONS_RADIO_TUNE_TABLE               PKEY_CONST('R','T','U','N')/*[int32,RO]         radio tune table version */
#define VERSIONS_RADIO_SERIAL                   PKEY_CONST('R','S','E','R')/*[string,RO]         radio tune table version */

/**     GPS - retrieve unit gps information  */
#define FIELD_GPS                               PKEY_CONST('S','G','P','S')/*[field,RW]         GPS information */
#define GPS_LATITUDE                            PKEY_CONST('L','A','T','I')/*[int32,RO]         GPS Latitude */
#define GPS_LONGITUDE                           PKEY_CONST('L','O','N','G')/*[int32,RO]         GPS Longitude */
#define GPS_SATELLITES                          PKEY_CONST('S','A','T','S')/*[int32,RO]         GPS sattelites */
#define GPS_FIX                                 PKEY_CONST('G','F','I','X')/*[int32,RO]         GPS fix */
#define GPS_STATUS                              PKEY_CONST('S','T','A','T')/*[int32,RO]         GPS status */
#define GPS_UTIM                                PKEY_CONST('U','T','I','M')/*[int32,RO]         GPS unix time */
#define GPS_SPEED                               PKEY_CONST('S','P','E','E')/*[int32,RO]         GPS speed */
#define GPS_HEADING                             PKEY_CONST('H','E','A','D')/*[int32,RO]         GPS heading */
#define GPS_ALTITUDE                            PKEY_CONST('A','L','T','I')/*[int32,RO]         GPS altitude */
#define GPS_DATETIME_STRING                     PKEY_CONST('T','S','T','R')/*[int32,RO]         GPS date/time string */

/* calibration */
#define FIELD_CAL_STATUS PKEY_CONST('C','A','L','S')
enum {
    CAL_STARTING_RECAL       = PKEY_CONST('C','S','T','A'), /*[bool,RO] a cal operation is about to start */
    CAL_RECAL_COMPLETE       = PKEY_CONST('C','C','O','M')  /*[bool,RO] a cal operation has completed, 1=>pass, 0=>fail */
};

/**     RADIO AUTO POWER/THERMAL CONTROL */
#define FIELD_RADIO_AUTO_MODE                   PKEY_CONST('R','A','U','T')/*[field,RW]         radio power commands */
#define RADIO_AUTO_POWER_MODE                   PKEY_CONST('R','P','O','W')/*[int32,WO]         radio power mgmt mode 0=off, 1=on, default on */
#define RADIO_AUTO_THERMAL_MODE                 PKEY_CONST('R','T','H','M')/*[int32,WO]         radio thermal mgmt mode 0=off, 1=on, default on */

#define FIELD_RADIO_AUTO_TIMEOUT                PKEY_CONST('R','A','T','O')/*[field,RW]         radio power commands */
#define RADIO_AUTO_POWER_TIMEOUT                PKEY_CONST('R','T','I','M')/*[int32,WO]         radio power mgmt timeout in secs, 0-65535, default 60s */

/**     RADIO POWER UP/DOWN */
#define FIELD_RADIO_POWER_UP                    PKEY_CONST('R','P','U','P')/*[field,RW]         radio power up */
#define FIELD_RADIO_POWER_DOWN                  PKEY_CONST('R','P','D','O')/*[field,RW]         radio power down */

/**     RADIO MONITOR - retrieve monitor information from the DSP radio subsystem */
#define FIELD_RADIO_MONITOR                     PKEY_CONST('R','M','O','N')/*[field,RW]         radio monitor information */
#define RADIO_MONITOR_TEMPERATURE               PKEY_CONST('T','E','M','P')/*[int32,RO]         radio temperature in deg C */
#define RADIO_MONITOR_VOLT_1                    PKEY_CONST('V','O','L','1')/*[int32,RO]         radio voltage 1 in millivolts */
#define RADIO_MONITOR_VOLT_2                    PKEY_CONST('V','O','L','2')/*[int32,RO]         radio voltage 2 in millivolts */

/**     BASEBAND MONITOR - retrieve monitor information from the DSP baseband subsystem */
#define FIELD_BASEBAND_MONITOR                  PKEY_CONST('B','M','O','N')/*[field,RW]         baseband monitor information */
#define BASEBAND_MONITOR_TEMPERATURE            PKEY_CONST('T','E','M','P')/*[int32,RO]         baseband temperature in deg C */
#define BASEBAND_MONITOR_VOLT_1                 PKEY_CONST('V','O','L','1')/*[int32,RO]         baseband voltage 1 in millivolts */
#define BASEBAND_MONITOR_VOLT_2                 PKEY_CONST('V','O','L','2')/*[int32,RO]         baseband voltage 2 in millivolts */
#define BASEBAND_MONITOR_VOLT_3                 PKEY_CONST('V','O','L','3')/*[int32,RO]         baseband voltage 3 in millivolts */
#define BASEBAND_MONITOR_VOLT_4                 PKEY_CONST('V','O','L','4')/*[int32,RO]         baseband voltage 4 in millivolts */
#define BASEBAND_MONITOR_INPUT_CURRENT          PKEY_CONST('I','I','N','P')/*[int32,RO]         baseband input current in milliamps */
#define BASEBAND_MONITOR_INPUT_VOLTAGE          PKEY_CONST('V','I','N','P')/*[int32,RO]         baseband input voltage in millivolts */

/**     REF CLOCK - control the radio reference clock source */
#define FIELD_REF_CLOCK                         PKEY_CONST('R','C','L','K')/*[field,RW]         radio ref clock control */
/* REF_CLOCK_SOURCE values are mutually exclusive */
#define REF_CLOCK_SOURCE_DAC                    PKEY_CONST('R','D','A','C')/*[bool,RW,true]     system ref AFC DAC */
#define REF_CLOCK_SOURCE_GPS                    PKEY_CONST('R','G','P','S')/*[bool,RW,false]    system ref 10MHz GPS */
#define REF_CLOCK_SOURCE_EXP1                   PKEY_CONST('R','E','X','1')/*[bool,RW,false]    system ref 10MHz EXP 1 */
#define REF_CLOCK_SOURCE_EXP2                   PKEY_CONST('R','E','X','2')/*[bool,RW,false]    system ref 10MHz EXP 2 */

  /**IMPORTANT NOTE: REF_CLOCK_DAC_SETTING - only change this setting if you wish to move away from the calibrated AFC DAC setting */
  /* The nominal value of the ref clock DAC is calibrated during production, this is the default state following system power cycle */
  /* Setting the ref clock DAC to a user value will override the production cal value until the next system power cycle */
#define REF_CLOCK_DAC_SETTING                   PKEY_CONST('S','D','A','C')/*[bool,RW,cal value] radio ref AFC DAC, value is 0..65536, nominal setting from calibration */
#define REF_CLOCK_OUT_EXP1                      PKEY_CONST('O','E','X','1')/*[int,RW,false]    output ref on EXP 1 (0 - None, 1 - GPS, 2 - 1PPS, 3- Radio Clk)*/
#define REF_CLOCK_OUT_EXP2                      PKEY_CONST('O','E','X','2')/*[int,RW,false]    output ref on EXP 2 (0 - None, 1 - GPS, 2 - 1PPS, 3- Radio Clk)*/
#define REF_CLOCK_DISABLE_TIME_TRANSMISSION     PKEY_CONST('R','D','T','T')/*[bool,RW,false]    disable time transmission */

  /*/     DSP RTC - read/write DSP real time clock */
#define FIELD_DSP_RTC                           PKEY_CONST('R','T','C','L')/*[field,RW]         DSP RTC */

  /* note: RTC_GET_TIME & RTC_SET_TIME commands are mutually exclusive */
#define DSP_RTC_GET_TIME                        PKEY_CONST('G','T','I','M')/*[bool,W0,true]     get time */
#define DSP_RTC_GET_TIME_FUTURE_NS              PKEY_CONST('G','T','F','U')/*[int32,RW,0]       future offset nanoseconds */
#define DSP_RTC_SET_TIME                        PKEY_CONST('S','T','I','M')/*[bool,W0,true]     set time */

  /*RTC_NOW values are used to specify the RTC_SET_TIME and also geturn the RTC_GET_TIME data */
#define DSP_RTC_NOW_DATE                        PKEY_CONST('N','D','A','T')/*[dateDMYC,RW]      now date */
#define DSP_RTC_NOW_TIME                        PKEY_CONST('N','T','I','M')/*[timeHMSC,RW]      now time */
#define DSP_RTC_NOW_UNIX_TIME                   PKEY_CONST('N','U','N','X')/*[int32,RW]         now time in unix time_t format */
#define DSP_RTC_NOW_NANO                        PKEY_CONST('N','N','A','N')/*[int32,RW]         now nano seconds */
#define DSP_RTC_NOW_STR                         PKEY_CONST('N','S','T','R')/*[string,RW]         now string */
#define DSP_RTC_FUTURE_UNIX_TIME                PKEY_CONST('F','U','N','X')/*[int32,RO]         future time in unix time_t format */
#define DSP_RTC_FUTURE_NANO                     PKEY_CONST('F','N','A','N')/*[int32,RO]         future nano seconds */
#define DSP_RTC_FUTURE_STRING                   PKEY_CONST('F','S','T','R')/*[string,RO]         future string */

  /**     DSP RTC_SYNC - configure  RTC synchronisation */
#define FIELD_DSP_RTC_SYNC                      PKEY_CONST('S','Y','N','C')/*[field,RW]         DSP RTC */
#define DSP_RTC_SYNC_DELAY_COMP_NS              PKEY_CONST('S','D','L','Y')/*[int32,RW,0]       set sync delay compensation: must be = measured path + rx_offset */
#define DSP_RTC_SYNC_DELAY_CYC10M_NS            PKEY_CONST('S','C','Y','C')/*[int32,RO,0]       actual cyc10m used */
#define DSP_RTC_SYNC_DELAY_TAP                  PKEY_CONST('S','T','A','P')/*[int32,RO,0]       actual tap used */

  /* RTC_CONFIG values are mutually exclusive */
#define DSP_RTC_CONFIG_SLAVE                    PKEY_CONST('S','C','F','G')/*[bool,RW,false]    configure as slave */
#define DSP_RTC_CONFIG_SLAVE_MEASURE_DELAY      PKEY_CONST('S','C','M','D')/*[bool,RW,false]    configure slave for delay measure */
#define DSP_RTC_CONFIG_MASTER                   PKEY_CONST('M','C','F','G')/*[bool,RW,false]    configure master */
#define DSP_RTC_CONFIG_MASTER_MEASURE_DELAY     PKEY_CONST('M','C','M','D')/*[bool,RW,false]    configure master */

#define DSP_RTC_MASTER_MEASURE_DELAY            PKEY_CONST('D','M','E','S')/*[bool,RW,false]    master measure round trip delay function */
#define DSP_RTC_MASTER_PATH_DELAY_NS            PKEY_CONST('M','P','D','Y')/*[bool,RW,false]    master measure round trip delay result */
#define DSP_RTC_MASTER_RX_OFFSET_NS             PKEY_CONST('M','O','F','F')/*[bool,RW,false]    master rx offset */

/* Antenna/Input selection */
#define ANY_INPUT_NODE                          PKEY_CONST('I','N','P','T')/*[int,RW,1]        antenna input on node */
#define ANY_INPUT_SWITCH                        PKEY_CONST('S','I','N','P')/*[int,RW]          antenna input on switch */
#define ANY_INPUT_BDC                           PKEY_CONST('B','I','N','P')/*[int,RW]          antenna input on BDC */
#define ANY_BDC_EXPANSION                       PKEY_CONST('B','D','E','X')/*[int,RW]          bdc expanspansion port */
#define ANY_SWITCH_EXPANSION                    PKEY_CONST('S','W','E','X')/*[int,RW]          switch expanspansion port */


  /*     FREQUENCY SWEEP - configure/retrieve data for frequency sweep */
#define FIELD_SWEEP                             PKEY_CONST('S','W','E','P')/*[field,RW]         frequency sweep */
#define SWEEP_START_FREQ_MHZ                    PKEY_CONST('F','S','T','A')/*[int32,RW,10]      start freq MHz */
#define SWEEP_STOP_FREQ_MHZ                     PKEY_CONST('F','S','T','P')/*[int32,RW,6000]    stop freq MHz */
#define SWEEP_START_FREQ_MILLIHZ                PKEY_CONST('F','S','A','M')/*[int32,RW]         Start fractional mHz */
#define SWEEP_STOP_FREQ_MILLIHZ                 PKEY_CONST('F','S','P','M')/*[int32,RW]         stop fractional mHz */


/*  SWEEP_QUALITY values are mutually exclusive - if one of these values is not present, then the unit defaults to auto an picks the most */
/*  appropriate value for the span */
#define SWEEP_QUALITY_LOW_NOISE                 PKEY_CONST('L','N','O','I')/*[bool,RW,auto]     sweep quality low noise */
#define SWEEP_QUALITY_FAST_TUNE                 PKEY_CONST('F','T','U','N')/*[bool,RW,auto]     sweep quality fast tune */
#define SWEEP_QUALITY_DUPLEX                    PKEY_CONST('Q','D','U','P')/*[bool,RW,auto]     sweep quality even faster tune */
#define SWEEP_HIGH_SPEED                        PKEY_CONST('H','S','P','E')/*[bool,RW,auto]     sweep using high speed method */
#define SWEEP_NORMAL_SPEED                      PKEY_CONST('N','S','P','E')/*[bool,RW,auto]     sweep using normal method */

  /* if SWEEP_RES_BANDWIDTH_HZ is not specified then the unit defaults to auto and picks the most appropriate resolution for the span */
#define SWEEP_RES_BANDWIDTH_HZ                  PKEY_CONST('R','E','S','B')/*[int32,RW,auto]    equivalent resolution bandwidth in Hz (taking account of windowing spread) */
#define SWEEP_FFT_SIZE                          PKEY_CONST('S','F','F','T')/*[int32,RO]         FFT size */
#define SWEEP_NUM_LOOPS                         PKEY_CONST('N','U','M','L')/*[int32,RW,1]       number of peak/average loops */
#define SWEEP_INPUT                             PKEY_CONST('I','N','P','T')/*[int32,RW,1]       input selection (1,2,3,4) */
#define SWEEP_REF_LEVEL                         PKEY_CONST('R','L','E','V')/*[int32,RW,-20]     reference level in dBm - data dynamic range extends 127.5dB below this level */

  /* if SWEEP_MANUAL_ATTEN is not specified then the unit defaults to auto and uses AGC */
#define SWEEP_MANUAL_ATTEN                      PKEY_CONST('A','T','E','N')/*[int32,RW,auto]    RF input attenuation in dB - if this parameter is omitted then AGC is used */
#define SWEEP_AGC                               PKEY_CONST('A','G','C',' ')/*[int32,RO,auto]    AGC is used */

#define SWEEP_DECIMATION                        PKEY_CONST('D','E','C','I')/*[int32,RW,0]       hardware decimation factor 0,1,5,10, 0=1=no decimation */
#define SWEEP_GET_PEAK_DATA                     PKEY_CONST('G','E','T','P')/*[bool,WO,true]     get peak spectral data */
#define SWEEP_GET_AVERAGE_DATA                  PKEY_CONST('G','E','T','A')/*[bool,WO,false]    get average spectral data */
#define SWEEP_GET_TIME_DATA                     PKEY_CONST('G','E','T','T')/*[int32,WO,false]   get time data */
#define SWEEP_GET_STATUS_DATA                   PKEY_CONST('G','E','T','S')/*[int32,WO,false]   get status records */

#define SWEEP_PEAK_DATA                         PKEY_CONST('P','D','A','T')/*[spec8,RO]         peak spectral data */
#define SWEEP_AVERAGE_DATA                      PKEY_CONST('A','D','A','T')/*[spec8,RO]         average spectral data */
#define SWEEP_TIME_I_Q_DATA                     PKEY_CONST('T','D','A','T')/*[timeIQ,RO]        IQ time data */
#define SWEEP_STATUS_DATA                       PKEY_CONST('S','D','A','T')/*[RO]               status data */

  /*  SWEEP_TRIG_MODE values are mutually exclusive */
#define SWEEP_TRIG_MODE_IMMEDIATE               PKEY_CONST('T','I','M','M')/*[bool,RW,true]     sweep trigger immediate */
#define SWEEP_TRIG_MODE_ABS_TIME                PKEY_CONST('T','A','B','S')/*[bool,RW,false]    sweep trigger ABS time */
#define SWEEP_TRIG_MODE_EXP_1                   PKEY_CONST('T','E','X','1')/*[bool,RW,false]    sweep trigger exp 1 */
#define SWEEP_TRIG_MODE_EXP_2                   PKEY_CONST('T','E','X','2')/*[bool,RW,false]    sweep trigger exp 1 */
#define SWEEP_TRIG_MODE_GPS                     PKEY_CONST('T','G','P','S')/*[bool,RW,false]    trig source GPS */

#define SWEEP_TRIG_TIME_UNIX                    PKEY_CONST('T','U','N','X')/*[int32,RW]         sweep trigger time unix time_t format */
#define SWEEP_TRIG_TIME_NANO                    PKEY_CONST('T','N','A','N')/*[int32,RW]         sweep trigger time nano seconds */
#define SWEEP_TRIG_TIME_STR                     PKEY_CONST('T','S','T','R')/*[int32,RW]         sweep trigger time string */
#define SWEEP_REPEAT_DELAY_UNIX                 PKEY_CONST('R','U','N','X')/*[int32,RW]         sweep trigger repeat time seconds */
#define SWEEP_REPEAT_DELAY_NANO                 PKEY_CONST('R','N','A','N')/*[int32,RW]         sweep trigger repeat time nano seconds */
#define SWEEP_REPEAT_MISSED_TRIGGERS            PKEY_CONST('T','M','I','S')/*[int32,RW]         sweep trigger repeat missed trigger count */
#define SWEEP_REPEAT_TRIGGER_MARGIN_UNIX        PKEY_CONST('M','U','N','X')/*[int32,RW]         sweep trigger repeat margin unix */
#define SWEEP_REPEAT_TRIGGER_MARGIN_NANO        PKEY_CONST('M','N','A','N')/*[int32,RW]         sweep trigger repeat margin nano */

  /*  SWEEP_SYNC_MODE - tune interval must be sufficient to allow all synchronised radios to tune and capture at each dwell freq */
#define SWEEP_SYNC_MODE                         PKEY_CONST('S','Y','N','C')/*[bool,RW,false]    use sync sweep mode */
#define SWEEP_ASYNC_MODE                        PKEY_CONST('A','S','Y','1')/*[int,RW,0]         specifies to use Asymmetric Multi-Sync Sweep.
                                                                                                Value indicates number of blocks */
#define SWEEP_ASYNC_TABLE                       PKEY_CONST('A','S','T','A')/* */
#define SWEEP_ASYNC_EXP_PORT                    PKEY_CONST('A','S','E','X')/*[int32, RW]        which expansion port to use 1 or 2*/
#define SWEEP_SYNC_TUNE_INTERVAL                PKEY_CONST('S','T','D','L')/*[int32,RW]         delay between tunes *25ns */
#define SWEEP_SYNC_MAX_RE_ARM_TIME_NS           PKEY_CONST('R','A','R','M')/*[int32,RO]         max re-arm time during sync sweep in ns */
#define SWEEP_SYNC_NUM_SWEEPS                   PKEY_CONST('N','S','W','P')/*[int32,RW]         number of buffered sweeps to carry out*/
#define SWEEP_SYNC_PEAK_TUNE_TIME_US            PKEY_CONST('S','N','A','N')/*[int32,RO]         peak radio tune time from sweep in us */
#define SWEEP_SYNC_DELAY_TUNE                   PKEY_CONST('S','S','D','T')/*[int32,RW]         increase sync-sweep delays 1-1000 percent. Value is 0.1%*/

#define SWEEP_COMMAND PKEY_CONST('C','M','D',' ')//[string,RO]

  /*/     AOA FREQUENCY SWEEP - configure/retrieve data for AOA sweep (requires CRFS AOA antenna array) */
#define FIELD_AOA_SWEEP                         PKEY_CONST('A','O','A','S')/*[field,RW]         AOA frequency sweep */
#define AOA_SWEEP_START_FREQ_MHZ                PKEY_CONST('F','S','T','A')/*[int32,RW,10]      start freq MHz */
#define AOA_SWEEP_STOP_FREQ_MHZ                 PKEY_CONST('F','S','T','P')/*[int32,RW,6000]    stop freq MHz */
#define AOA_SWEEP_START_FREQ_MILLIHZ            PKEY_CONST('F','S','A','M')/*[int32,RW]         Start fractional mHz */
#define AOA_SWEEP_STOP_FREQ_MILLIHZ             PKEY_CONST('F','S','P','M')/*[int32,RW]         stop fractional mHz */

  /*  AOA_SWEEP_QUALITY values are mutually exclusive - if one of these values is not present, then the unit defaults to auto an picks the most */
  /*  appropriate value for the span */
#define AOA_SWEEP_QUALITY_LOW_NOISE             PKEY_CONST('L','N','O','I')/*[bool,RW,auto]     sweep quality low noise */
#define AOA_SWEEP_QUALITY_FAST_TUNE             PKEY_CONST('F','T','U','N')/*[bool,RW,auto]     sweep quality fast tune */

  /* if SWEEP_RES_BANDWIDTH_HZ is not specified then the unit defaults to auto and picks the most appropriate resolution for the span */
#define AOA_SWEEP_RES_BANDWIDTH_HZ              PKEY_CONST('R','E','S','B')/*[int32,RW,auto]    equivalent resolution bandwidth in Hz (taking account of windowing spread) */

#define AOA_SWEEP_NUM_LOOPS                     PKEY_CONST('N','U','M','L')/*[int32,RW,1]       number of peak/average loops */
#define AOA_SWEEP_INPUT                         PKEY_CONST('I','N','P','T')/*[int32,RW,1]       input selection 1,2,3,4 */
#define AOA_SWEEP_REF_LEVEL                     PKEY_CONST('R','L','E','V')/*[int32,RW,-20]     reference level in dBm - data dynamic range extends 127.5dB below this level */

  /* if SWEEP_MANUAL_ATTEN is not specified then the unit defaults to auto and uses AGC */
#define AOA_SWEEP_MANUAL_ATTEN                  PKEY_CONST('A','T','E','N')/*[int32,RW,auto]    RF input attenuation in dB - if this parameter is omitted then AGC is used */
#define AOA_SWEEP_AGC                           PKEY_CONST('A','G','C',' ')/*[int32,RO,auto]    AGC is used */

#define AOA_SWEEP_ANTENNA_CONFIG                PKEY_CONST('A','C','O','N')/*[uint32,RW,0x00654321]   antenna configuration, each nibble indicates antenna number, starting from least significant */
#define AOA_SWEEP_NUM_ANTENNAS                  PKEY_CONST('A','N','U','M')/*[int32,RW,6]       number of antennas in array */
#define AOA_SWEEP_EXPANSION_PORT                PKEY_CONST('E','X','P','O')/*[int32,RW,1]       expansion port used to drive switch (1,2) */
#define AOA_SWEEP_GET_PEAK_DATA                 PKEY_CONST('G','E','T','P')/*[bool,RW,true]     get peak spectral data */
#define AOA_SWEEP_GET_AVERAGE_DATA              PKEY_CONST('G','E','T','A')/*[bool,RW,false]    get average spectral data */
#define AOA_SWEEP_GET_STATUS_DATA               PKEY_CONST('G','E','T','S')/*[bool,RW,false]    get status data */
#define AOA_SWEEP_GET_TIME_DATA                 PKEY_CONST('G','E','T','T')/*[bool,RW,false]    get time data */

#define AOA_SWEEP_DATA_SUB_ANT_1                PKEY_CONST('D','A','T','1')/*[spec8,RO]         spectral data sub antenna */
#define AOA_SWEEP_DATA_SUB_ANT_2                PKEY_CONST('D','A','T','2')/*[spec8,RO]         spectral data sub antenna */
#define AOA_SWEEP_DATA_SUB_ANT_3                PKEY_CONST('D','A','T','3')/*[spec8,RO]         spectral data sub antenna */
#define AOA_SWEEP_DATA_SUB_ANT_4                PKEY_CONST('D','A','T','4')/*[spec8,RO]         spectral data sub antenna */
#define AOA_SWEEP_DATA_SUB_ANT_5                PKEY_CONST('D','A','T','5')/*[spec8,RO]         spectral data sub antenna */
#define AOA_SWEEP_DATA_SUB_ANT_6                PKEY_CONST('D','A','T','6')/*[spec8,RO]         spectral data sub antenna */
#define AOA_SWEEP_DATA_SUB_ANT_7                PKEY_CONST('D','A','T','7')/*[spec8,RO]         spectral data sub antenna */
#define AOA_SWEEP_DATA_SUB_ANT_8                PKEY_CONST('D','A','T','8')/*[spec8,RO]         spectral data sub antenna */

#define AOA_SWEEP_TIME_SUB_ANT_1                PKEY_CONST('A','T','D','1')/*[spec8,RO]         time data sub antenna */
#define AOA_SWEEP_TIME_SUB_ANT_2                PKEY_CONST('A','T','D','2')/*[spec8,RO]         time data sub antenna */
#define AOA_SWEEP_TIME_SUB_ANT_3                PKEY_CONST('A','T','D','3')/*[spec8,RO]         time data sub antenna */
#define AOA_SWEEP_TIME_SUB_ANT_4                PKEY_CONST('A','T','D','4')/*[spec8,RO]         time data sub antenna */
#define AOA_SWEEP_TIME_SUB_ANT_5                PKEY_CONST('A','T','D','5')/*[spec8,RO]         time data sub antenna */
#define AOA_SWEEP_TIME_SUB_ANT_6                PKEY_CONST('A','T','D','6')/*[spec8,RO]         time data sub antenna */
#define AOA_SWEEP_TIME_SUB_ANT_7                PKEY_CONST('A','T','D','7')/*[spec8,RO]         time data sub antenna */
#define AOA_SWEEP_TIME_SUB_ANT_8                PKEY_CONST('A','T','D','8')/*[spec8,RO]         time data sub antenna */

#define AOA_SWEEP_AVG_SUB_ANT_1                 PKEY_CONST('A','A','D','1')/*[spec8,RO]         average data sub antenna */
#define AOA_SWEEP_AVG_SUB_ANT_2                 PKEY_CONST('A','A','D','2')/*[spec8,RO]         average data sub antenna */
#define AOA_SWEEP_AVG_SUB_ANT_3                 PKEY_CONST('A','A','D','3')/*[spec8,RO]         average data sub antenna */
#define AOA_SWEEP_AVG_SUB_ANT_4                 PKEY_CONST('A','A','D','4')/*[spec8,RO]         average data sub antenna */
#define AOA_SWEEP_AVG_SUB_ANT_5                 PKEY_CONST('A','A','D','5')/*[spec8,RO]         average data sub antenna */
#define AOA_SWEEP_AVG_SUB_ANT_6                 PKEY_CONST('A','A','D','6')/*[spec8,RO]         average data sub antenna */
#define AOA_SWEEP_AVG_SUB_ANT_7                 PKEY_CONST('A','A','D','7')/*[spec8,RO]         average data sub antenna */
#define AOA_SWEEP_AVG_SUB_ANT_8                 PKEY_CONST('A','A','D','8')/*[spec8,RO]         average data sub antenna */

#define AOA_SWEEP_STATUS_SUB_ANT_1                 PKEY_CONST('A','S','D','1')/*[spec8,RO]         status data sub antenna */
#define AOA_SWEEP_STATUS_SUB_ANT_2                 PKEY_CONST('A','S','D','2')/*[spec8,RO]         status data sub antenna */
#define AOA_SWEEP_STATUS_SUB_ANT_3                 PKEY_CONST('A','S','D','3')/*[spec8,RO]         status data sub antenna */
#define AOA_SWEEP_STATUS_SUB_ANT_4                 PKEY_CONST('A','S','D','4')/*[spec8,RO]         status data sub antenna */
#define AOA_SWEEP_STATUS_SUB_ANT_5                 PKEY_CONST('A','S','D','5')/*[spec8,RO]         status data sub antenna */
#define AOA_SWEEP_STATUS_SUB_ANT_6                 PKEY_CONST('A','S','D','6')/*[spec8,RO]         status data sub antenna */
#define AOA_SWEEP_STATUS_SUB_ANT_7                 PKEY_CONST('A','S','D','7')/*[spec8,RO]         status data sub antenna */
#define AOA_SWEEP_STATUS_SUB_ANT_8                 PKEY_CONST('A','S','D','8')/*[spec8,RO]         status data sub antenna */


  /*/     TIME CAPTURE - configure/retrieve data for time capture */
#define FIELD_TIME                              PKEY_CONST('T','I','M','E')/*[field,RW]         time capture */
#define TIME_CENTER_FREQ_MHZ                    PKEY_CONST('F','C','T','R')/*[int32,RW,2000]    center freq MHz */
#define TIME_DDS_OFFSET_HZ                      PKEY_CONST('D','D','S','O')

#define TIME_NUM_SAMPLES                        PKEY_CONST('S','A','M','P')/*[int32,RW,1024]    time capture number of samples */

  /* if TIME_MANUAL_ATTEN is not specified then the unit defaults to auto and uses AGC */
#define TIME_MANUAL_ATTEN                       PKEY_CONST('A','T','E','N')/*[int32,RW,auto]    RF input attenuation in dB - if this parameter is omitted then AGC is used */
#define TIME_AGC                                PKEY_CONST('A','G','C',' ')/*[int32,RW,auto]    AGC is used */
#define TIME_INPUT                              PKEY_CONST('I','N','P','T')/*[int32,RW,1]       input selection 1,2,3,4 */
#define TIME_DECIMATION                         PKEY_CONST('D','E','C','I')/*[int32,RW,0]       hardware decimation factor 0,2,4-8191, 0=1=no decimation */

/*  TIME_QUALITY values are mutually exclusive - if one of these values is not present, then the unit defaults low noise */
#define TIME_QUALITY_LOW_NOISE                  PKEY_CONST('L','N','O','I')/*[bool,RW,true]     sweep quality low noise */
#define TIME_QUALITY_FAST_TUNE                  PKEY_CONST('F','T','U','N')/*[bool,RW,false]    sweep quality fast tune */

#define TIME_SIMPLE_DC_NUM_CAP                  PKEY_CONST('T','S','D','C')/*[int,RW,0]  simple DC offset compensation, number of captures - development only */

/* TIME_TRIG_MODE values are mutually exclusive  - default is immediate trigger */
#define TIME_TRIG_MODE_IMMEDIATE                PKEY_CONST('T','I','M','M')/*[bool,RW,true]     trig immediately */
#define TIME_TRIG_MODE_EXP_1                    PKEY_CONST('T','E','X','1')/*[bool,RW,false]    trig source EXP1 */
#define TIME_TRIG_MODE_EXP_2                    PKEY_CONST('T','E','X','2')/*[bool,RW,false]    trig source EXP2 */
#define TIME_TRIG_MODE_GSM_FRAME                PKEY_CONST('T','G','S','M')/*[bool,RW,false]    trig source GSM frame */
#define TIME_TRIG_MODE_POWER_THRESH             PKEY_CONST('T','P','T','H')/*[bool,RW,false]    trig source power threshold */
#define TIME_TRIG_MODE_GPS                      PKEY_CONST('T','G','P','S')/*[bool,RW,false]    trig source GPS */
#define TIME_TRIG_PT_THRESH                     PKEY_CONST('T','P','P','T') /*[int,RW,0] log or linear (see AOA spinner) power threshold value */
#define TIME_TRIG_PT_DWELL                      PKEY_CONST('T','P','D','W') /*[int,RW,0] dwell time on each antenna (in samples) */
#define TIME_TRIG_PT_PORT                       PKEY_CONST('T','P','P','O') /*[int,RW,0] expansion port (1 or 2) connected to antenna switch */
#define TIME_TRIG_NUMBER_CAPTURES               PKEY_CONST('T','T','N','C') /*[int,RW,0]*/
#define TIME_TRIG_MODE_ABS_TIME                 PKEY_CONST('T','A','B','S')/*[bool,RW,false]    trig on absolute time */
#define TIME_TRIG_ABS_UNIX                      PKEY_CONST('T','U','N','X')/*[int,RW]    trig absolute time unix seconds */
#define TIME_TRIG_ABS_NANO                      PKEY_CONST('T','N','A','N')/*[int,RW]    trig absolute time nanoseconds */
#define TIME_TRIG_REPEAT_UNIX                   PKEY_CONST('R','U','N','X')/*[int,RW]    trig repeat time unix seconds */
#define TIME_TRIG_REPEAT_NANO                   PKEY_CONST('R','N','A','N')/*[int,RW]    trig repeat time nanoseconds */
#define TIME_REPEAT_MISSED_TRIGGERS             PKEY_CONST('T','M','I','S')/*[int32,RW]         time trigger repeat missed trigger count */
#define TIME_REPEAT_TRIGGER_MARGIN_UNIX         PKEY_CONST('M','U','N','X')/*[int32,RW]         time trigger repeat margin unix */
#define TIME_REPEAT_TRIGGER_MARGIN_NANO         PKEY_CONST('M','N','A','N')/*[int32,RW]         time trigger repeat margin nano */

#define TIME_RADIO_GAIN                         PKEY_CONST('R','G','A','I')/*[int32,RO]         radio gain, use this to scale the time IQ samples to absolute power, ADC calibrated as 0dBm  for full scale */
#define TIME_AGC_ATTEN                          PKEY_CONST('R','A','T','N')/*[int32,RO]         radio RF attenuation */
#define TIME_FREQ_PLAN_INVERT                   PKEY_CONST('F','I','N','V')/*[bool,RO]          radio freq plan invert, indicates a nominal IQ swap caused by freq inversion in the radio */
#define TIME_DC_OFFSET_I                        PKEY_CONST('D','C','O','I')/*[int,RO]           DC offset I */
#define TIME_DC_OFFSET_Q                        PKEY_CONST('D','C','O','Q')/*[int,RO]           DC offset Q */
#define TIME_GET_I_Q_DATA                       PKEY_CONST('G','I','Q','T')/*[bool,WO,true]     get I Q Time Data */
#define TIME_I_Q_DATA                           PKEY_CONST('D','I','Q','T')/*[timeIQ,RO]        I Q Time Data */
#define TIME_NO_RETUNE                          PKEY_CONST('T','N','R','T')/*[int,RW]           Dont retune the radio */
#define TIME_PACK_DATA                          PKEY_CONST('T','P','D','A')/*[int,RW]           Pack data into 2,4,8 bits */
#define TIME_PACK_SCALE                         PKEY_CONST('T','P','S','C')/*[int,RO]           Scale used to pack data */

#define TIME_STREAM_DATA                        PKEY_CONST('T','S','T','R')/*[bool,RW,false]    Stream time data */

/* AOA spinner */
#define FIELD_AOA_SPIN                 PKEY_CONST('A','O','S','P')

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
    AOA_SPIN_LOG_NLIN = PKEY_CONST('A','O','L','G') // log, not linear, threshold, default 0
};


/*     AM/FM DEMODULATION - configure/retrieve demodulated data */
#define FIELD_DEMODULATION                      PKEY_CONST('D','M','O','D')/*[field,RW]         AM/FM demod mode */
#define DEMOD_RADIO_TUNE_FREQ_MHZ           PKEY_CONST('F','C','T','R')/*[int32,RW,100]     center freq MHz */
#define DEMOD_DDS_FREQ_KHZ                  PKEY_CONST('F','D','D','S')/*[int32,RW,1000]    dds freq Hz */
#define DEMOD_INPUT                         PKEY_CONST('I','N','P','T')/*[int32,RW,1]       input selection 1,2,3,4 */
#define DEMOD_RF_DECIMATION                 PKEY_CONST('D','E','C','I')/*[int32,RW,100]     hardware RF decimation factor 0,1,5,10, 0=1=no decimation */
#define DEMOD_GAIN                          PKEY_CONST('D','G','A','I')/*[bool,RW,false]    demodulator gain  (0.255) */
#define DEMOD_FM_MUTE_THD                   PKEY_CONST('M','U','T','E')/*[bool,RW,false]    FM mute threshold */

  /* DEMOD_MODE values are mutually exclusive */
#define DEMOD_MODE_FM                       PKEY_CONST('F','M','D','E')/*[bool,RW,true]     FM demod selected */
#define DEMOD_MODE_AM                       PKEY_CONST('A','M','D','E')/*[bool,RW,false]    AM demod selected */
#define DEMOD_MODE_OFF                      PKEY_CONST('D','O','F','F')/*[bool,RW,false]    demod off */

#define DEMOD_DATA_DECIMATION               PKEY_CONST('D','D','E','C')/*[int32,RW,25]      hardware demod data decimation factor */
#define DEMOD_DATA                          PKEY_CONST('D','D','A','T')/*[demod16,RO]       16 bit demodulated data */

#define DEMOD_GET_SPECTRUM_DATA             PKEY_CONST('G','S','P','E')/*[bool,WO,false]    get spectrum data */
#define DEMOD_SPECTRUM_DATA                 PKEY_CONST('S','P','E','C')/*[spec8,RO]         demod spectrum data */

  /*/     GSM AOA - configure/retrieve data for GSM AOA function */
#define FIELD_GSM_AOA                           PKEY_CONST('A','G','S','M')/*[field,RW]         GSM AOA */
#define GSM_AOA_CHANNEL_NUMBER              PKEY_CONST('C','H','A','N')/*[int32,RW,776]     GSM Channel Number */
  /*  GSM_CHANNEL_DIR parameters are mutaully exclusive */
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

/* LED Expansion port */
#define FIELD_LIGHTS                        PKEY_CONST('L','I','G','H')
#define LED_EXP1_YELLOW                     PKEY_CONST('Y','E','L','1') /*[Bool,RW]         Enable/Disable Yellow LED EXP1 */
#define LED_EXP1_GREEN                      PKEY_CONST('G','R','E','1') /*[Bool,RW]         Enable/Disable Green LED EXP1 */
#define LED_EXP1_RED                        PKEY_CONST('R','E','D','1') /*[Bool,RW]         Enable/Disable Red LED EXP1 */
#define LED_EXP1_SWITCH                     PKEY_CONST('S','W','I','1') /*[Bool,RO]         Switch Status EXP1 */
#define LED_EXP2_YELLOW                     PKEY_CONST('Y','E','L','2') /*[Bool,RW]         Enable/Disable Yellow LED EXP2 */
#define LED_EXP2_GREEN                      PKEY_CONST('G','R','E','2') /*[Bool,RW]         Enable/Disable Green LED EXP2 */
#define LED_EXP2_RED                        PKEY_CONST('R','E','D','2') /*[Bool,RW]         Enable/Disable Red LED EXP2 */
#define LED_EXP2_SWITCH                     PKEY_CONST('S','W','I','2') /*[Bool,RO]         Switch Status EXP2 */

#ifdef __cplusplus
}
#endif

#endif /* NCP_PACKETS_H_INCLUDED */
