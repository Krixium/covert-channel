#include "net.h"

#include <netdb.h>
#include <string.h>

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