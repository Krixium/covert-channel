#include "main.h"

#include <getopt.h>
#include <string.h>

int main(int argc, char *argv[])
{
    struct progArgs args;

    parseArguments(argc, argv, &args);

    if (!strcmp("server", argv[1]))
    {
        return srvr(&args);
    }
    else if (!srtcmp("client", argv[1]))
    {
        return clnt(&args);
    }
    else
    {
        usage(argv[0]);
    }

    return 0;
}

void parseArguments(int argc, char *argv[], struct progArgs *args)
{
    int c;

    while ((c = getopt(argc, argv, "d:p:f:s")) != -1)
    {
        switch ((c))
        {
            case 'd':
                args->dstIp = atoi(optarg);
                break;
            case 'p':
                args->dstPort = atoi(optarg);
                break;
            case 'f':
                strcpy(args->filename, optarg);
                break;
            case 's':
                args->srcIp = atoi(optarg);
                break;
            default:
                break;
        }
    }
}

void usage(char *name)
{
    printf("Usage: %s [server|client]\n", name);
    printf("\n");
    printf("\tClient Mode Required:\n");
    printf("\t\t-d - The desired destination ip of the packet.\n");
    printf("\t\t-p - The desired destination port of the packet.\n");
    printf("\t\t-f - The file to send\n");
    printf("\tClient Mode Optional:\n");
    printf("\t\t-s - source ip, the source ip to put into the packet(this can be any valid ip)\n");
    printf("\n");
    printf("\tServer Mode Required:\n");
    printf("\t\t-s - The ip of the client that is sending covert messages.\n");
    printf("\t\t-f - The file to write incoming data into.\n");
}


int clnt(struct progArgs *args)
{

}

int srvr(struct progArgs *args)
{

}