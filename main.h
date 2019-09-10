#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>

struct progArgs
{
    unsigned int srcIp;
    unsigned int dstIp;
    unsigned short dstPort;
    char filename[256];
} progArgs;

void parseArguments(int argc, char *argv[], struct progArgs *args);
void usage(char *name);

int clnt(struct progArgs *args);
int srvr(struct progArgs *args);

#endif