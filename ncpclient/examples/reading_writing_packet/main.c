/*
    Example to show:
        1) connecting to a node
        2) creating/sending a request packet
        3) Waiting for the result packet
        4) Decode the result packet
        5) Cleaning up

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

    /* 1 */
    /* Create a ncp client */
    T_NCP_CLIENT_CONNECTION* ncp_client = ncp_client_create();

    /* Connect to the client */
    ncp_client_connect(ncp_client, argv[1], 9999, 0, -1);

    /* 2 */
    /* Build a simple request for the version information */
    T_PACKET* txpacket = packet_create("", 8192);
    /* Set the packet type to a node request */
    packet_write(txpacket, PACKET_TYPE_NODE, -1);
        /* Add the field we are requesting */
        packet_add_field(txpacket, FIELD_NCP_VERSIONS, -1);
        /* This field request requires no paramters so non are added */
    /* Mark the packet as complete */
    packet_write_complete(txpacket);
    /* Send the packet */
    ncp_client_send(ncp_client, txpacket);

    /* 3 */
    /* Once a second the node sends a packet to check if the connection is still active.
       These LINK packets can be ignored */
    int packet_found = 0;
    while (packet_found == 0)
    {
        ncp_client_receive(ncp_client);
        if (packet_get_packet_type(ncp_client_get_rx_packet(ncp_client)) != PACKET_TYPE_LINK)
            packet_found = 1;
    }

    /* 4 */
    /* Set the state of the packet to reading */
    T_PACKET* rxpacket = ncp_client_get_rx_packet(ncp_client);
    packet_read(rxpacket);
    /* Loop through all the fields */
    int fname, fid;
    while (packet_get_next_field(rxpacket, &fname, &fid) == 1)
    {
        printf("Field Name: %s, Id: %d", packet_key_to_str(fname, NULL), fid);
        int pname, plength;
        T_PARAM_DATA_TYPE ptype;
        void* pdata;
        while (packet_get_next_param(rxpacket, &pname, &ptype, &pdata, &plength) == 1)
        {
            printf("Param Name: %s, Value: ", packet_key_to_str(pname, NULL));
            switch (ptype)
            {
                case PARAM_INT:
                    printf("%d", (int)pdata); break;
                case PARAM_UNSIGNED_INT:
                    printf("%d", (unsigned int)pdata); break;
                case PARAM_STRING:
                    printf("%s", (char*)pdata); break;
                default:
                    printf("RAW_DATA");
            }
            printf("\n");
        }
    }


    /* 5*/
    /* Close the connection */
    ncp_client_disconnect(ncp_client);

    /* Free the NCP client */
    ncp_client_create();

    exit(EXIT_SUCCESS);
}
