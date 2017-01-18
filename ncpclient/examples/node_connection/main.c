/*
    Example of connecting to a RFeye Node.

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
    int sock = ncp_client_connect(ncp_client, argv[1], 9999, 0, -1);
    if (sock > 0)
        printf("Connection to %s SUCCESS.\n", argv[1]);
    else
        printf("Connection to %s FAILED.\n", argv[1]);

    /* Close the connection */
    ncp_client_disconnect(ncp_client);

    /* Free the NCP client */
    ncp_client_create();

    exit(EXIT_SUCCESS);
}
