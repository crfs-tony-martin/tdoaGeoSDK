/*
    Example of setting the time on a RFeye Node.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <windows.h>

#include "packets.h"
#include "ncp_client.h"
#include "ncp_packets.h"

int main(int argc, char* argv[])
{
    /* Check argument is provided */
    if (argc != 2)
    {
        printf("%s <Node to connect>", argv[0]);
        exit(EXIT_FAILURE);
    }

    int exitcode = EXIT_SUCCESS;

    /* Create a ncp client */
    T_NCP_CLIENT_CONNECTION* ncp_client = ncp_client_create();

    /* Connect to the client */
    int sock = ncp_client_connect(ncp_client, argv[1], 9999, 0, -1);
    if (sock > 0)
    {
        /* Set the time to the current PC time */
        /* Get the current PC time */
        time_t seconds = time(NULL);

        /* Send the packet to set the time */
        T_PACKET* txpacket = packet_create("", 8192);
        packet_write(txpacket, PACKET_TYPE_DSP_CONTROL, -1);
            packet_add_field(txpacket, FIELD_DSP_RTC, 1);
            packet_add_param_int(txpacket, DSP_RTC_SET_TIME, 1);
            packet_add_param_int(txpacket, DSP_RTC_NOW_UNIX_TIME, seconds);
        /* Mark the packet as complete */
        packet_write_complete(txpacket);
        /* Send the packet */
        ncp_client_send(ncp_client, txpacket);

        //Sleep(1000);

        /* Receive ACK & Response */
        T_PACKET* rxpacket = ncp_client_get_rx_packet(ncp_client);
        packet_read(rxpacket);
        packet_read(rxpacket);

        Sleep(1000);

        printf("SUCCESS.\n");
        exitcode = EXIT_SUCCESS;

        /* Close the connection */
        ncp_client_disconnect(ncp_client);
    }
    else
    {
        printf("FAILED: Could not connect to %s.\n", argv[1]);
        exitcode = EXIT_FAILURE;
    }

    /* Free the NCP client */
    ncp_client_create();

    exit(exitcode);
}
