/*-----------------------------------------------------------------------------
 * (c) CRFS Limited, 2011. All rights reserved.
 *
 * This software is the property of CRFS Limited and may not be copied or
 * reproduced otherwise than on to a single hard disk for backup or
 * archival purposes. The source code is confidential information and must
 * not be disclosed to third parties or used without the express written
 * permission of CRFS Limited.
 *
 * Filename : lights.h
 *---------------------------------------------------------------------------*/
#ifndef __LIGHTS_H
#define __LIGHTS_H

/** \file  lights.h

	Field and options to set the lights, retreive the status of the switch on a connected light box
*/

/** 
	\ingroup FIELD_GROUP

	Request/retrieve the status of the lights

	\note This command requires the CRFS light box

	See \ref LIGHTS_GROUP 
	\frw
*/
#define FIELD_LIGHTS                        	PKEY_CONST('L','I','G','H')

/**
	@defgroup LIGHTS_GROUP Lights options

	The field provides the option to alter the status of the LED's on a CRFS lightbox.

	\warning Incorrect use of LED_EXP* options can cause hardware plugged into the expansion port specified to 
			 stop working until a power cycle. All these options are deprecated, please use the newer commands
			 that will work on the configured LED expansion boards.

	\code
	    // Turn on green LED and all other off
	    T_PACKET* txpacket = packet_create("", 8192);
    	packet_write(txpacket, PACKET_TYPE_DSP_CONTROL, -1);
        // Add the LED field
        packet_add_field(txpacket, FIELD_LIGHTS, -1);
            packet_add_param_int(txpacket, LED_GREEN, 1);
            packet_add_param_int(txpacket, LED_YELLOW, 0);
            packet_add_param_int(txpacket, LED_RED, 0);
	    packet_write_complete(txpacket);
	\endcode

	@{
*/
#define LED_EXP1_YELLOW		PKEY_CONST('Y','E','L','1') ///< \type{INT}\rw Enable/Disable yellow on LED EXP1 \deprecated to be removed
#define LED_EXP1_GREEN      PKEY_CONST('G','R','E','1') ///< \type{INT}\rw Enable/Disable green on LED EXP1 \deprecated to be removed
#define LED_EXP1_RED        PKEY_CONST('R','E','D','1') ///< \type{INT}\rw Enable/Disable red on LED EXP1 \deprecated to be removed
#define LED_EXP1_SWITCH     PKEY_CONST('S','W','I','1') ///< \type{INT}\ro Switch status on EXP1 \deprecated to be removed
#define LED_EXP2_YELLOW     PKEY_CONST('Y','E','L','2') ///< \type{INT}\rw Enable/Disable yellow on LED EXP2 \deprecated to be removed
#define LED_EXP2_GREEN      PKEY_CONST('G','R','E','2') ///< \type{INT}\rw Enable/Disable green on LED EXP2 \deprecated to be removed
#define LED_EXP2_RED        PKEY_CONST('R','E','D','2') ///< \type{INT}\rw Enable/Disable green on LED EXP2 \deprecated to be removed
#define LED_EXP2_SWITCH     PKEY_CONST('S','W','I','2') ///< \type{INT}\ro Switch status on EXP2 \deprecated to be removed
#define LED_YELLOW          PKEY_CONST('Y','E','L',' ') ///< \type{INT}\rw Enable/Disable yellow LED
#define LED_GREEN           PKEY_CONST('G','R','E',' ') ///< \type{INT}\rw Enable/Disable green LED
#define LED_RED             PKEY_CONST('R','E','D',' ') ///< \type{INT}\rw Enable/Disable red LED
#define LED_SWITCH          PKEY_CONST('S','W','I',' ') ///< \type{INT}\ro Switch status
/** @} */

#endif
