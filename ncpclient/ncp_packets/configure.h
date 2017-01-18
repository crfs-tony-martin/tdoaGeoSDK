/*-----------------------------------------------------------------------------
 * (c) CRFS Limited, 2011. All rights reserved.
 *
 * This software is the property of CRFS Limited and may not be copied or
 * reproduced otherwise than on to a single hard disk for backup or
 * archival purposes. The source code is confidential information and must
 * not be disclosed to third parties or used without the express written
 * permission of CRFS Limited.
 *
 * Filename : configure.h
 *---------------------------------------------------------------------------*/
#ifndef __CONFIGURE_H
#define __CONFIGURE_H

/** \file  configure.h

	Field and options to retreive configuration information
*/


/** 
	\ingroup FIELD_GROUP
	\brief Update/retrieve configuration options of NCPd

	See \ref CONFIGURE_GROUP 
	\frw
*/
#define FIELD_CONFIGURE                         PKEY_CONST('C','O','N','F')

/**
	@defgroup CONFIGURE_GROUP Configure options

	This field provides the options to configure and monitor a number of NCP parameters including expansion ports, GPS 
	and radio. The detailed descriptions of each option are as follows.

	\code
	    // Alter to configuration to send stat packets once a second
	    T_PACKET* txpacket = packet_create("", 8192);
    	packet_write(txpacket, PACKET_TYPE_DSP_NODE, -1);
        // Add the configuration sweep field
        packet_add_field(txpacket, FIELD_CONFIGURE, -1);
            // Enable stat packets
            packet_add_param_int(txpacket, CONFIGURE_STAT_PACKETS, 1);
	    packet_write_complete(txpacket);
	\endcode

	@{
 */
#define CONFIGURE_GPS_AUTO              PKEY_CONST('G','P','S','A') ///< \type{INT}\rw Send GPS information in Status packets. 0 = off, 1 = on
#define CONFIGURE_MONITOR_AUTO          PKEY_CONST('M','O','N','A') ///< \type{INT}\rw Send Radio/Baseband monitor in Status packets. 0 = off, 1 = on
#define CONFIGURE_STAT_PACKETS          PKEY_CONST('C','S','T','A') ///< \type{INT}\rw Automatically send Status packets. 0 = off, 1 = on
#define CONFIGURE_JSON_CONFIGURATION	PKEY_CONST('J','C','O','N') ///< \type{STRING}\ro Nodes configuration in JSON format
/**
	\brief \type{INT}\rw Configure exclusive lock

	This option allows the connected client to take "Exclusive" access to the radio. Once a client
	has the lock they are the only client that can perform radio commands. Use this if you require your commands to 
	run without interuption.

	0 = Unlock, 1 = Attempt to lock


	\retval 2 Locked to you
	\retval 1 Locked, but not to you
	\retval 0 Unlocked

*/
#define CONFIGURE_EXCLUSIVE_LOCK        PKEY_CONST('E','L','O','C')
/**
	\brief \type{INT}\rw Automatically add GPS information to all returned packets

	This option enables\\disables the returning of GPS information on all packets. If enabled each packet that is 
	returned will contain a \ref FIELD_GPS field with the latest currently available GPS information. This does add a 
	certain overhead to communications.

	0 = Enable, 1 = Disable

*/
#define CONFIGURE_GPS_ALL               PKEY_CONST('G','P','S','L') /*[int,RW] automatically add GPS information to all packets: 0=off, 1=on */
/**
	\brief \type{STRING}\rw Configure expansion port 1

	This allows the configuration of BDC hardware on a give expansion port. The RF connection between the BDC &
	node needs to be specified in the string. The string should contain the word INPUT:\<rf\> followed by the RF number.
	The following would specify the BDC is connected to RF 1 of the node.
	
	\code INPUT:1 \endcode
*/
#define CONFIGURE_EXP1                  PKEY_CONST('E','X','P','1')
#define CONFIGURE_EXP2                  PKEY_CONST('E','X','P','2') ///< \type{STRING}\rw Configure expansion port 2. See \ref CONFIGURE_EXP1
/** @} */

#endif
