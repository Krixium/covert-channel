/*---------------------------------------------------------------------------------------
-- SOURCE FILE:             main.c - Main logic for the covert channel.
--
-- PROGRAM:                 covert
--
-- FUNCTIONS:
--                          void parseArguments(int argc, char *argv[], struct progArgs *args)
--                          void usage(char *name)
--                          void printError(char *msg)
--                          void printPacket(unsigned char *buffer, unsigned int size)
--                          int clnt(struct progArgs *args)
--                          int srvr(struct progArgs *args)
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
--                          A covert channel program that sends or recieves a file character by
--                          character. The character is transfered in the lower byte of the UDP
--                          source port field.
---------------------------------------------------------------------------------------*/
#include "main.h"
#include "net.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*---------------------------------------------------------------------------------------
-- FUNCTION:                main
--
-- DATE:                    Sept 12, 2019
--
-- REVISIONS:               N/A
--
-- DESIGNER:                Benny Wang
--
-- PROGRAMMER:              Benny Wang
--
-- INTERFACE:               int main(int argc, char *argv[])
--                              int argc: The number of command line arguments.
--                              int argv: A list of the command line arguments.
--
-- RETURNS:                 The program return code.
--
-- NOTES:
--                          The entry point of the program.
---------------------------------------------------------------------------------------*/
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

/*---------------------------------------------------------------------------------------
-- FUNCTION:                parseArguments
--
-- DATE:                    Sept 12, 2019
--
-- REVISIONS:               N/A
--
-- DESIGNER:                Benny Wang
--
-- PROGRAMMER:              Benny Wang
--
-- INTERFACE:               void parseArguments(int argc, char *argv[], struct progArgs *args)
--                              int argc: The number of command line arguments.
--                              char *argv[]: The command line arguments.
--                              struct progArgs *args: A struct where the partsed command line arguments will be stored.
--
-- NOTES:
--                          Parses the command line arguments for the required information to run the program.
---------------------------------------------------------------------------------------*/
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

/*---------------------------------------------------------------------------------------
-- FUNCTION:                usage
--
-- DATE:                    Sept 12, 2019
--
-- REVISIONS:               N/A
--
-- DESIGNER:                Benny Wang
--
-- PROGRAMMER:              Benny Wang
--
-- INTERFACE:               void usage(char *name)
--                              char *name: The name of the program(argv[0]).
--
-- NOTES:
--                          Prints the usage help menu of the program.
---------------------------------------------------------------------------------------*/
void usage(char *name)
{
    printf("Usage: %s [server|client]\n", name);
    printf("\n");
    printf("\tClient Mode Required:\n");
    printf("\t\t-s - source ip, the source ip to put into the packet(this can be any valid ip)\n");
    printf("\t\t-d - The desired destination ip of the packet.\n");
    printf("\t\t-p - The desired destination port of the packet.\n");
    printf("\t\t-f - The file to send\n");
    printf("\tClient Mode Optional:\n");
    printf("\t\t-t - The amount of time in seconds to wait between each send. Default is 1 second.\n");
    printf("\n");
    printf("\tServer Mode Required:\n");
    printf("\t\t-s - The ip of the client that is sending covert messages.\n");
    printf("\t\t-p - The desired destination port of the packet.\n");
    printf("\t\t-f - The file to write incoming data into.\n");
}

/*---------------------------------------------------------------------------------------
-- FUNCTION:                printError
--
-- DATE:                    Sept 12, 2019
--
-- REVISIONS:               N/A
--
-- DESIGNER:                Benny Wang
--
-- PROGRAMMER:              Benny Wang
--
-- INTERFACE:               void pringError(char *msg)
--                              char *msg: The message to display as an error.
--
-- NOTES:
--                          Prints an error message and then exits the program with exit code 1.
---------------------------------------------------------------------------------------*/
void printError(char *msg)
{
    printf("Error: %s\n", msg);
    exit(1);
}

/*---------------------------------------------------------------------------------------
-- FUNCTION:                clnt
--
-- DATE:                    Sept 12, 2019
--
-- REVISIONS:               N/A
--
-- DESIGNER:                Benny Wang
--
-- PROGRAMMER:              Benny Wang
--
-- INTERFACE:               int clnt(struct progArgs *args)
--                              struct progArgs *args: The parsed command line arguments.
--
-- RETURNS:                 0 if successful, otherwise a positive integer.
--
-- NOTES:
--                          The main client code. Opens a file and sends it over the network.
--                          The file is sent using the lower byte of the UDP header.
---------------------------------------------------------------------------------------*/
int clnt(struct progArgs *args)
{
    printf("Client Mode\n");

    int sfd;
    char ch;
    unsigned char buffer[sizeof(struct iphdr) + sizeof(struct udphdr)];
    FILE *inputFile = fopen(args->filename, "rb");
    struct sockaddr_in dstAddr;
    struct sockaddr_in srcAddr;

    printf("Resolving addresses ...\n");

    if (!getSockAddr(&dstAddr, args->dstIp, args->dstPort))
    {
        printError("Could not resolve destination address.");
    }

    if (!getSockAddr(&srcAddr, args->srcIp, 0))
    {
        bcopy(&dstAddr, &srcAddr, sizeof(struct sockaddr_in));
    }

    printf("Creating socket ...\n");

    if ((sfd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0)
    {
        printError("Could not create a socket.");
    }


    printf("Starting ...\n");

    while ((ch = fgetc(inputFile)) != EOF)
    {
        printf("Sending [%c] ...\n", ch);
        constructHeader(buffer, ch, &srcAddr, &dstAddr);
        sendto(sfd, buffer, sizeof(struct iphdr) + sizeof(struct udphdr), 0, (struct sockaddr *)&dstAddr, sizeof(struct sockaddr_in));

        sleep(1);
    }

    close(sfd);
    fclose(inputFile);

    printf("Finished\n");

    return 0;
}

/*---------------------------------------------------------------------------------------
-- FUNCTION:                srvr
--
-- DATE:                    Sept 12, 2019
--
-- REVISIONS:               N/A
--
-- DESIGNER:                Benny Wang
--
-- PROGRAMMER:              Benny Wang
--
-- INTERFACE:               int srvr(struct progArgs *args)
--
-- RETURNS:                 0 if successful, otherwise a positive integer.
--
-- NOTES:
--                          The main server code. Listens for UDP packets sent from the
--                          specified client on the specified port and extracts the
--                          character from the source field in the UDP header than writes
--                          it to a file.
---------------------------------------------------------------------------------------*/
int srvr(struct progArgs *args)
{
    printf("Server Mode\n");

    int sfd;
    FILE *outputFile = fopen(args->filename, "wb");
    unsigned char buffer[1024];
    struct udphdr *udpHeader = (struct udphdr *)(buffer + sizeof(struct iphdr));
    struct sockaddr_in anyAddr;
    struct sockaddr_in targetAddr;
    struct sockaddr_in srcAddr;
    unsigned int addrLen = sizeof(struct sockaddr_in);

    bzero(&targetAddr, sizeof(struct sockaddr_in));
    if (!getSockAddr(&targetAddr, args->srcIp, args->dstPort))
    {
        printError("Could not resolve client ip");
    }

    if ((sfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) < 0)
    {
        printError("Could not create a socket.");
    }

    bzero(&anyAddr, sizeof(struct sockaddr_in));
    anyAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    anyAddr.sin_port = htons(args->dstPort);
    anyAddr.sin_family = AF_INET;

    if ((bind(sfd, (struct sockaddr *)&anyAddr, addrLen)) == -1)
    {
        printError("Could not bind socket.");
    }

    while (1)
    {
        bzero(buffer, 1024);
        recvfrom(sfd, &buffer, 1024, 0, (struct sockaddr *)&srcAddr, &addrLen);

        if (targetAddr.sin_addr.s_addr == srcAddr.sin_addr.s_addr)
        {
            printf("received character [%c]\n", udpHeader->source >> 8);
            fprintf(outputFile, "%c", udpHeader->source >> 8);
        }
    }

    fclose(outputFile);
    return 0;
}