/*-----------------------------------------------------------------------------
 * (c) CRFS Limited, 2011. All rights reserved.
 *
 * This software is the property of CRFS Limited and may not be copied or
 * reproduced otherwise than on to a single hard disk for backup or
 * archival purposes. The source code is confidential information and must
 * not be disclosed to third parties or used without the express written
 * permission of CRFS Limited.
 *
 * Filename : ncp_example/main.c
 * Author : D Timson
 *
 * Purpose: Do various tests on NCP client.
 *
 * Description:
 *---------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __BORLANDC__
	/* just for struct timeval */
	#include <winsock2.h>
#else
	#include <sys/time.h>
#endif

#include "packets.h"
#include "ncp_client.h"
#include "ncp_packets.h"
#include "crfs_debug.h"

#define TIMEDIFF(X, Y) ((X).tv_sec - (Y).tv_sec + ((double)((X).tv_usec - (Y).tv_usec)) / 1000000)

/* static prototypes */
static void receive_data(T_NCP_CLIENT_CONNECTION *ncp_client);
static void check_for_bdc(T_NCP_CLIENT_CONNECTION *ncp_client, int* connected, int* exp_port, int* rf_input);

/* Main, setting up and reporting tests. */
#ifdef __BORLANDC__
#pragma hdrstop
#include <tchar.h>
#pragma argsused
int _tmain(int argc, _TCHAR* argv[])
#else
int main(int argc, char *argv[])
#endif
{
	char *hostname;
	T_NCP_CLIENT_CONNECTION *ncp_client;
	int sd; /*/ socket number */
	struct timeval tp1, tp2;
	T_PACKET *control_packet;
	int i, testno;

	hostname = (argc > 1) ? argv[1] : "192.168.1.90";

    testno = (argc > 2) ? atoi(argv[2]) : 0;

	/* create the control packet */
    control_packet = packet_create("", 8192);

    /*/ create the NCP client */
	ncp_client = ncp_client_create();
    /*/ try to connect */
    sd = ncp_client_connect(ncp_client,   /*/ client */
							hostname,
							9999,         /*/port number */
							0,            /*/non-blocking */
							3);          /*/no timeout */

    if (sd > 0) /*/ if socket is assigned then connection successful */
	{
        /* printf("-3-\n"); */
        gettimeofday(&tp1, NULL);

        /*/ set up packet with repeating loop */
        /*/ NO BDC: sweep from 2000 to 2050 MHz, peak data on input 1 or
            BDC: sweep from 10000 to 12000 MHz, peak data on connected port */
        /*/ time capture at 200MHz, 1024 samples on antenna 1 */

        packet_write(control_packet, PACKET_TYPE_DSP_LOOP, -1); /* create DSP single command packet */

		/* Is there a BDC connected */
		int bdc_connected = 0;
		int bdc_exp_port = 0;
		int bdc_rf_input = 0;
		check_for_bdc(ncp_client, &bdc_connected, &bdc_exp_port, &bdc_rf_input);
		if ((bdc_connected != -1) && (bdc_exp_port != -1) && (bdc_rf_input != -1))
		{
            packet_add_field(control_packet, FIELD_SWEEP, 0); /* add freq sweep field */
            packet_add_param_int(control_packet, SWEEP_START_FREQ_MHZ, 10000);
            packet_add_param_int(control_packet, SWEEP_STOP_FREQ_MHZ, 12000);
            packet_add_param_int(control_packet, SWEEP_GET_PEAK_DATA, 1);
            /* When using a BDC you need to add 3 peices of information.
                1) The expansion port the BDC is connected to.
                2) The RF input the BDC is connected to on the node.
                3) The RF input to use on the BDC */
            packet_add_param_int(control_packet, ANY_BDC_EXPANSION, bdc_exp_port);
            packet_add_param_int(control_packet, ANY_INPUT_NODE, 1);
            packet_add_param_int(control_packet, ANY_INPUT_BDC, 2);
		}
		else
		{
            packet_add_field(control_packet, FIELD_SWEEP, 0); /* add freq sweep field */
            packet_add_param_int(control_packet, SWEEP_START_FREQ_MHZ, 2000);
            packet_add_param_int(control_packet, SWEEP_STOP_FREQ_MHZ, 2050);
            packet_add_param_int(control_packet, SWEEP_GET_PEAK_DATA, 1);
            packet_add_param_int(control_packet, SWEEP_INPUT, 1);
		}

		packet_add_field(control_packet, FIELD_TIME, 1);/* add time capture field */
		packet_add_param_int(control_packet, TIME_CENTER_FREQ_MHZ, 2000);
		packet_add_param_int(control_packet, TIME_NUM_SAMPLES, 1024);

		packet_write_complete(control_packet);
		ncp_client_send(ncp_client, control_packet);

        /*/ loop to receive incoming data from unit */
        for (i=0; i<10; i++)
        {
            receive_data(ncp_client);
        }
    }

	gettimeofday(&tp2, NULL);
	printf("Duration = %f s\n", TIMEDIFF(tp2, tp1));

    packet_free(control_packet);
    ncp_client_disconnect(ncp_client);
    ncp_client_free(ncp_client);
#ifdef _WIN32
    printf("Press <return>: ");
    getchar();
#endif

    return 0;
}


/*-----------------------------------------------------------------------------
    Routine for receiving data from blocking socket.
    On reception of a complete packet, the data is decoded and displayed in the console
---------------------------------------------------------------------------*/
static void receive_data(T_NCP_CLIENT_CONNECTION *ncp_client)
{
    int i;
    int field_name;
    int datalength;
    static char databuf[65536];
    static int16_t IQbuffer[65536];
    char packet_type[5];
    T_PACKET *rx_packet;
    int field_identifier;

    /*/ receive an NCP packet */
    ncp_client_receive(ncp_client);

    /*/ get rx_packet from the client */
    rx_packet = ncp_client_get_rx_packet(ncp_client);

      /*/print the packet type */
    packet_key_to_str(packet_get_packet_type(rx_packet), packet_type);
    printf("<<<<<packet received type=\'%s\'>>>>>\n", packet_type);

      /*/reset packet for reading */
    packet_read(rx_packet);

    /*/loop until all fields in packet are processed */
    while (packet_get_next_field(rx_packet, &field_name, &field_identifier) > 0)
    {
        int ack_packet;
		int v;
		T_PARAM_DATA_TYPE data_type;
        switch (field_name)
        {
		case FIELD_SWEEP: /* sweep data field found */
			if (packet_get_param_int(rx_packet, ANY_ACKNOWLEDGE_PACKET, &ack_packet) > 0)
				printf("SWEEP ACK, packet %d\n", ack_packet);
			else
				printf("SWEEP field found  id:%d\n", field_identifier);
			packet_get_param_int(rx_packet, SWEEP_START_FREQ_MHZ, &v);
			printf("  start freq  %dMHz\n", v);
			packet_get_param_int(rx_packet, SWEEP_STOP_FREQ_MHZ, &v);
			printf("  stop freq  %dMHz\n", v);
			if ( (datalength = packet_get_param_data(rx_packet, SWEEP_PEAK_DATA, &data_type, databuf, 65536)) > 0) /*get the peak data */
			{
				for (i=0; i<20; i++)
				{
					printf("  %d, %gdBm\n", i, ((unsigned char) databuf[i]/2.0)-147.5);
				}
			}
			break;
		case FIELD_TIME: /* time data field found */
			if (packet_get_param_int(rx_packet, ANY_ACKNOWLEDGE_PACKET, &ack_packet) > 0)
				printf("TIME ACK, packet %d\n", ack_packet);
			else
				printf("TIME field found  id:%d\n", field_identifier);
			packet_get_param_int(rx_packet, TIME_CENTER_FREQ_MHZ, &v);
			printf("  center freq  %dMHz\n", v);
			if ( (datalength = packet_get_param_data(rx_packet, TIME_I_Q_DATA, &data_type, (char*) IQbuffer, 65536)) > 0) /*get the time data */
			{
				for (i=0; i<20; i++)
				{
					printf("  %d, I:%d  Q:%d\n", i, IQbuffer[i*2], IQbuffer[(i*2)+1]);
				}
			}
			break;
		}
	}
    packet_read_complete(rx_packet);
}

static void check_for_bdc(T_NCP_CLIENT_CONNECTION *ncp_client, int* connected, int* exp_port, int* rf_input)
{
    *connected = -1;
    *exp_port = -1;
    *rf_input = -1;

    T_PACKET *tx_packet;

    tx_packet = ncp_client_get_tx_packet(ncp_client);

    /* Send a stat message. This will respond with the connection status of the BDC */
    packet_write(tx_packet, PACKET_TYPE_NODE, 1);
    packet_add_field(tx_packet, FIELD_STATUS_INFORMATION, 1);
    packet_write_complete(tx_packet);
    ncp_client_send(ncp_client, tx_packet);

    /* Wait for the response */
    T_PACKET *rx_packet = NULL;
    while (ncp_client_receive(ncp_client))
    {
        rx_packet = ncp_client_get_rx_packet(ncp_client);
        if (packet_get_packet_type(rx_packet) == PACKET_TYPE_NODE)
            break;
    }

    packet_print(rx_packet);

    /* BDC connected to EXP_1 or EXP_2. Example only checks EXP_1 */
    int field_name = 0;
    int field_identifier = 0;
    packet_read(rx_packet);
    if (packet_get_next_field(rx_packet, &field_name, &field_identifier) > 0)
    {
        if (field_name == FIELD_STATUS_INFORMATION)
        {
            char exp_1[128];
            if (packet_get_param_string(rx_packet, STATUS_EXP_1, exp_1, 128) > 0)
            {
                /* Check to see if BDC connected */
                if (strncasecmp(exp_1, "CONNECTED:BDC18", 15) == 0)
                {
                    *exp_port = 1;

                    /* Check to see if it was detected */
                    if (strstr(exp_1, "STATUS:DETECTED") != NULL)
                        *connected = 1;

                    /* Which node RF input is it connected to? */
                    char *input = strstr(exp_1, "INPUT:");
                    if (input != NULL)
                    {
                        *rf_input = atoi(input + 6);
                    }
                }
            }
        }
    }
}
