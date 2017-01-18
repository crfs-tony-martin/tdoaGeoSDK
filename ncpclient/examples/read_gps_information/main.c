/*
    Example shows reading a status packet from the node
    and then extracting the GPS Time

    NOTE: No error checking is performed
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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

    /* Create a ncp client */
    T_NCP_CLIENT_CONNECTION* ncp_client = ncp_client_create();

    /* Connect to the client */
    ncp_client_connect(ncp_client, argv[1], 9999, 0, -1);

    /* Build a simple request for the version information */
    T_PACKET* txpacket = packet_create("", 8192);
    /* Set the packet type to a node request */
    packet_write(txpacket, PACKET_TYPE_NODE, -1);
        /* Add the field we are requesting */
        packet_add_field(txpacket, FIELD_GPS, -1);
        /* This field request requires no paramters so non are added */
    /* Mark the packet as complete */
    packet_write_complete(txpacket);
    /* Send the packet */
    ncp_client_send(ncp_client, txpacket);


    /* Get the response packet */
    T_PACKET* rxpacket = ncp_client_get_rx_packet(ncp_client);

    do
    {
        ncp_client_receive(ncp_client);
    /* Server send LINK packets to ensure client is still connected */
    } while (packet_get_packet_type(rxpacket) == PACKET_TYPE_LINK);

    // Print the satelite information
    int fname, fid;
    packet_read(rxpacket);
    while (packet_get_next_field(rxpacket, &fname, &fid) == 1)
    {
        if (fname == FIELD_GPS)
        {
            // Fixed?
            int tmp;
            packet_get_param_int(rxpacket, GPS_FIX, &tmp);
            printf("Fix       : %d\n", tmp);
            packet_get_param_int(rxpacket, GPS_LATITUDE, &tmp);
            printf("Latitude  : %d\n", tmp);
            packet_get_param_int(rxpacket, GPS_LONGITUDE, &tmp);
            printf("Longitude : %d\n", tmp);
            packet_get_param_int(rxpacket, GPS_ALTITUDE, &tmp);
            printf("Altitude  : %d\n", tmp);
            packet_get_param_int(rxpacket, GPS_SATELLITES, &tmp);
            printf("In View   : %d\n", tmp);
            packet_get_param_int(rxpacket, GPS_SPEED, &tmp);
            printf("Speed     : %d\n", tmp);
            packet_get_param_int(rxpacket, GPS_HEADING, &tmp);
            printf("Heading   : %d\n", tmp);
            packet_get_param_int(rxpacket, GPS_UTIM, &tmp);
            printf("Unix Time : %d\n", tmp);
            // Time string
            char time[255];
            packet_get_param_string(rxpacket, GPS_DATETIME_STRING, time, 255);
            printf("Time      : %s\n", time);
        }
    }

    /* Close the connection */
    ncp_client_disconnect(ncp_client);

    /* Free the NCP client */
    ncp_client_create();

    exit(EXIT_SUCCESS);
}
