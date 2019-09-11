#include "main.h"
#include "net.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int main(int argc, char *argv[])
{
    struct progArgs args;
    bzero(&args, sizeof(struct progArgs));

    if (argc < 2 || argc > 16)
    {
        usage(argv[0]);
        return 0;
    }

    if (!strcmp("server", argv[1]))
    {
        args.mode = MODE_SERVER;
    }
    else if (!strcmp("client", argv[1]))
    {
        args.mode = MODE_CLIENT;
    }
    else
    {
        usage(argv[0]);
    }

    parseArguments(argc, argv, &args);

    if (args.mode == MODE_CLIENT)
    {
        clnt(&args);
    }
    else if (args.mode == MODE_SERVER)
    {
        srvr(&args);
    }
    else
    {
        printError("Invalid");
    }

    return 0;
}

void parseArguments(int argc, char *argv[], struct progArgs *args)
{
    int c;

    while ((c = getopt(argc, argv, "d:p:f:s:")) != -1)
    {
        switch ((c))
        {
            case 'd':
                strcpy(args->dstIp, optarg);
                break;
            case 'p':
                args->dstPort = atoi(optarg);
                break;
            case 'f':
                strcpy(args->filename, optarg);
                break;
            case 's':
                strcpy(args->srcIp, optarg);
                break;
            default:
                break;
        }
    }

    if (!strcmp(args->filename, ""))
    {
        printError("Missing filename of file to send.");
    }

    if (args->mode == MODE_CLIENT)
    {
        if (!args->dstIp)
        {
            printError("Missing destination address.");
        }

        if (!args->dstPort)
        {
            printError("Missing destination port.");
        }
    }
    else if (args->mode == MODE_SERVER)
    {
        if (!args->dstIp)
        {
            printError("Missing client address.");
        }
    }
    else
    {
        printError("Invalid mode");
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

void printError(char *msg)
{
    printf("Error: %s\n", msg);
    exit(1);
}

int clnt(struct progArgs *args)
{
    printf("Client Mode\n");

    char ch;
    unsigned char buffer[sizeof(struct iphdr) + sizeof(struct udphdr)];
    FILE *inputFile = fopen(args->filename, "rb");
    struct sockaddr_in dstAddr;
    struct sockaddr_in srcAddr;

    if (!getSockAddr(&dstAddr, args->dstIp, args->dstPort))
    {
        printError("Could not resolve destination address.");
    }

    if (!getSockAddr(&srcAddr, args->srcIp, 0))
    {
        printError("Could not resolve source address.");
    }

    while ((ch = fgetc(inputFile)) != EOF)
    {
        // TODO: Make this less slow
        sleep(1);

        constructHeader(buffer, ch, &srcAddr, &dstAddr);
    }

    fclose(inputFile);
    return 0;
}

int srvr(struct progArgs *args)
{
    printf("Server Mode\n");

    FILE *outputFile = fopen(args->filename, "wb");

    while (1)
    {

    }

    fclose(outputFile);
    return 0;
}