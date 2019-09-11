#ifndef FORGE_H
#define FORGE_H

#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <sys/socket.h>

struct PseudoHeader
{
    unsigned int srcAddr;
    unsigned int dstAddr;
    unsigned char placeholder;
    unsigned char protocol;
    unsigned short len;
};

void constructHeader(unsigned char *buffer, char ch, struct sockaddr_in *srcAddr, struct sockaddr_in *dstAddr);
unsigned short in_cksum(unsigned short *, int);

#endif