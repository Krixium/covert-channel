#include "forge.h"

#include <stdlib.h>
#include <string.h>

void constructHeader(unsigned char *buffer, char ch, struct sockaddr_in *srcAddr, struct sockaddr_in *dstAddr)
{
    struct iphdr *ipHeader = (struct iphdr *)buffer;
    struct udphdr *udpHeader = (struct udphdr *)(buffer + sizeof(struct iphdr));

    char pBuffer[sizeof(struct PseudoHeader) + sizeof(struct udphdr)];
    struct PseudoHeader *pHeader = (struct PseudoHeader *)&pBuffer;

    // Create ip header
    ipHeader->ihl = 5;
    ipHeader->version = 4;
    ipHeader->tos = 0;
    ipHeader->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr);
    ipHeader->id = (int)(255.0 * rand() / (RAND_MAX + 1.0));
    ipHeader->frag_off = 0;
    ipHeader->ttl = 255;
    ipHeader->protocol = IPPROTO_UDP;
    ipHeader->check = 0;
    ipHeader->saddr = srcAddr->sin_addr.s_addr;
    ipHeader->check = in_cksum((unsigned short *)ipHeader, sizeof(struct iphdr));

    // Create udp header
    udpHeader->source = ch;
    udpHeader->dest = dstAddr->sin_port;
    udpHeader->len = htons(sizeof(struct udphdr));
    udpHeader->check = 0;

    // Create pseudo header
    pHeader->srcAddr = ipHeader->saddr;
    pHeader->dstAddr = ipHeader->daddr;
    pHeader->placeholder = 0;
    pHeader->protocol = IPPROTO_UDP;
    pHeader->len = sizeof(struct udphdr);

    memcpy(buffer + sizeof(struct PseudoHeader), udpHeader, sizeof(struct udphdr));

    udpHeader->check = in_cksum((unsigned short *)buffer, sizeof(struct iphdr) + sizeof(struct udphdr));
}

/* clipped from ping.c (this function is the whore of checksum routines */
/* as everyone seems to use it..I feel so dirty...) */

/* Copyright (c)1987 Regents of the University of California.
* All rights reserved.
*
* Redistribution and use in source and binary forms are permitted
* provided that the above copyright notice and this paragraph are
* dupliated in all such forms and that any documentation, advertising
* materials, and other materials related to such distribution and use
* acknowledge that the software was developed by the University of
* California, Berkeley. The name of the University may not be used
* to endorse or promote products derived from this software without
* specific prior written permission. THIS SOFTWARE IS PROVIDED ``AS
* IS'' AND WITHOUT ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
* WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF MERCHATIBILITY AND
* FITNESS FOR A PARTICULAR PURPOSE
*/
unsigned short in_cksum(unsigned short *ptr, int nbytes)
{
    register long sum;                              /* assumes long == 32 bits */
    unsigned short oddbyte;
    register unsigned short answer;                        /* assumes u_short == 16 bits */

    /*
    * Our algorithm is simple, using a 32-bit accumulator (sum),
    * we add sequential 16-bit words to it, and at the end, fold back
    * all the carry bits from the top 16 bits into the lower 16 bits.
    */

    sum = 0;
    while (nbytes > 1)
    {
        sum += *ptr++;
        nbytes -= 2;
    }

    /* mop up an odd byte, if necessary */
    if (nbytes == 1)
    {
        oddbyte = 0;                                /* make sure top half is zero */
        *((unsigned char *) &oddbyte) = *(unsigned char *)ptr;    /* one byte only */
        sum += oddbyte;
    }

    /*
    * Add back carry outs from top 16 bits to low 16 bits.
    */

    sum  = (sum >> 16) + (sum & 0xffff);            /* add high-16 to low-16 */
    sum += (sum >> 16);                             /* add carry */
    answer = ~sum;                                  /* ones-complement, then truncate to 16 bits */

    return(answer);
}