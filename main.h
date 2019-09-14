#ifndef MAIN_H
#define MAIN_H

#include "forge.h"

#define MODE_SERVER 1
#define MODE_CLIENT 2

struct progArgs
{
    int mode;
    char filename[256];
    char srcIp[16];
    char dstIp[16];
    unsigned short dstPort;
};

void parseArguments(int argc, char *argv[], struct progArgs *args);
void usage(char *name);
void printError(char *msg);
void printPacket(unsigned char *buffer, unsigned int size);

int clnt(struct progArgs *args);
int srvr(struct progArgs *args);

#endif