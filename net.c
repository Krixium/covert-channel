/*---------------------------------------------------------------------------------------
-- SOURCE FILE:             net.c - Networking stack related code.
--
-- PROGRAM:                 covert
--
-- FUNCTIONS:
--                          int getSockAddr(struct sockaddr_in *out, char *address, short port)
--
-- DATE:                    Sept 12, 2019
--
-- REVISIONS:               N/A
--
-- DESIGNERS:               Benny Wang
--
-- PROGRAMMERS:             Benny Wang
--
-- NOTES:
--                          Contains wrappers for the linux networking calls.
---------------------------------------------------------------------------------------*/
#include "net.h"

#include <netdb.h>
#include <string.h>

/*---------------------------------------------------------------------------------------
-- FUNCTION:                getSockAddr
--
-- DATE:                    Sept 12, 2019
--
-- REVISIONS:               N/A
--
-- DESIGNER:                Benny Wang
--
-- PROGRAMMER:              Benny Wang
--
-- INTERFACE:               int getSockAddr(struct sockaddr_in *out, char *address, short port)
--                              struct sockaddr_in *out: A pointer to the output struct.
--                              char *address: The address to reslove.
--                              short port: The port to resolve.
--
-- RETURNS:                 1 if successful, 0 otherwise.
--
-- NOTES:
--                          Fills a sockaddr_in struct with the given dotted decmial ip string
--                          and port combination.
---------------------------------------------------------------------------------------*/
int getSockAddr(struct sockaddr_in *out, char *address, short port)
{
    struct hostent *host;

    if ((host = gethostbyname(address)) == 0)
    {
        return 0;
    }

    bzero(out, sizeof(struct sockaddr_in));

    out->sin_family = AF_INET;
    out->sin_port = htons(port);
    bcopy(host->h_addr, (char *)&out->sin_addr, host->h_length);

    return 1;
}