/*
    Example of requesting time data in a loop.
    Example will continue until ctrl-c is pressed.

    This example is very similar to single_time_example.

    Changes:
        Change the type of packet from PACKET_TYPE_DSP_CONTROL to PACKET_TYPE_DSP_LOOP
        Change the receiving code to multiple packets


    NOTE: No error checking is performed
*/

#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>

#include "packets.h"
#include "ncp_client.h"
#include "ncp_packets.h"

/* Function to connect to node */
T_NCP_CLIENT_CONNECTION* connect_to_node(const char* host, int port)
{
    /* Create a ncp client */
    T_NCP_CLIENT_CONNECTION* ncp_client = ncp_client_create();

    /* Connect to the client */
    ncp_client_connect(ncp_client, host, port, 0, -1);

    return ncp_client;
}

/* Function to disconnect from a node */
void disconnect_from_node(T_NCP_CLIENT_CONNECTION* ncp_client)
{
    /* Close the connection */
    ncp_client_disconnect(ncp_client);

    /* Free the NCP client */
    ncp_client_create();
}

/* Receive a packet discarding link packets */
void receive_packet(T_NCP_CLIENT_CONNECTION* client)
{
    int packet_found = 0;
    while (packet_found == 0)
    {
        ncp_client_receive(client);
        if (packet_get_packet_type(ncp_client_get_rx_packet(client)) != PACKET_TYPE_LINK)
            packet_found = 1;
    }
}

/* Print a packet to stdout. This function is for
   examples sake. A much better function exists in the
   client library packet_print. */
void print_packet(T_PACKET* packet)
{
	int fname, fid;

    packet_read(packet);
    /* Loop through all the fields */

    while (packet_get_next_field(packet, &fname, &fid) == 1)
    {
		void* pdata;
		int pname, plength;
		T_PARAM_DATA_TYPE ptype;
        printf("Field Name: %s, Id: %d\n", packet_key_to_str(fname, NULL), fid);

        while (packet_get_next_param(packet, &pname, &ptype, &pdata, &plength) == 1)
        {
			int i;
			int *iq;
            printf("    Param Name: %s, Value: ", packet_key_to_str(pname, NULL));
            switch (ptype)
            {
                case PARAM_INT:
                    printf("%d", *(int*)(pdata)); break;
                case PARAM_UNSIGNED_INT:
                    printf("%d", *(unsigned int*)(pdata)); break;
                case PARAM_STRING:
                    printf("%s", (char*)pdata); break;
                default:
                    printf("RAW_DATA %d: ", plength);
					iq = (int *)pdata;
					for(i = 0;i < plength/2; i++)
					{
						printf("%d %d\n", iq[i] >> 16, (iq[i] << 16) >> 16);
					}
            }
            printf("\n");
        }
    }
}

int main(int argc, char* argv[])
{
	T_NCP_CLIENT_CONNECTION* ncp_client;
	T_PACKET* txpacket;
	T_PACKET* rxpacket;
    /* Check argument is provided */
    //if (argc != 2)
    //{
    //    printf("%s <Node to connect>", argv[0]);
    //    exit(EXIT_FAILURE);
    //}

    /* Connect to the node */
    //ncp_client = connect_to_node(argv[1]);

	//ncp_client = connect_to_node("RFeye002637", 9999);
	ncp_client = connect_to_node("217.37.1.115", 1099);
    /* Request a time capture */
    txpacket = packet_create("", 8192);
    packet_write(txpacket, PACKET_TYPE_DSP_LOOP, -1);
        /* Add the time field */
        packet_add_field(txpacket, FIELD_TIME, -1);
            /* Add the parameters */
            /* Frequency to capture => 900Mhz*/
            packet_add_param_int(txpacket, TIME_CENTER_FREQ_MHZ, 1090);
            /* Fast capture */
            packet_add_param_int(txpacket, TIME_QUALITY_FAST_TUNE, 1);
            /* Number of samples => 100000 */
            packet_add_param_int(txpacket, TIME_NUM_SAMPLES, 1000);
			/* Decimation = 100 */
			packet_add_param_int(txpacket, TIME_DECIMATION, 100);
    packet_write_complete(txpacket);

    /* Send the request */
    ncp_client_send(ncp_client, txpacket);

    /* Receive the response */
    /* An acknowledgement will be sent first */
    rxpacket = ncp_client_get_rx_packet(ncp_client);
    receive_packet(ncp_client);
    print_packet(rxpacket);

    /* Receive the responses */
    /* The response may be in multiple packets */
    while (1)
    {
        int finished = 0;
		int seg = 0;
        int nseg = 0;
        while (!finished)
        {
            receive_packet(ncp_client);
            print_packet(rxpacket);

            /* If the response is over multiple packets the following information */
            /* will be available in the response packets. */

            packet_get_param_int(rxpacket, ANY_NUM_DATA_SEGMENTS, &nseg);
            packet_get_param_int(rxpacket, ANY_DATA_SEGMENT_NUMBER, &seg);
            if (seg >= nseg)
                finished = 1;
        }
    }

    /* Disconnect from the node */
    disconnect_from_node(ncp_client);

    exit(EXIT_SUCCESS);
}
