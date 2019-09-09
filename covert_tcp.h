/* Covert_TCP 1.0 - Covert channel file transfer for Linux
* Written by Craig H. Rowland (crowland@psionic.com)
* Copyright 1996 Craig H. Rowland (11-15-96)
* NOT FOR COMMERCIAL USE WITHOUT PERMISSION.
*
*
* This program manipulates the TCP/IP header to transfer a file one byte
* at a time to a destination host. This progam can act as a server and a client
* and can be used to conceal transmission of data inside the IP header.
* This is useful for bypassing firewalls from the inside, and for
* exporting data with innocuous looking packets that contain no data for
* sniffers to analyze. In other words, spy stuff... :)
*
* PLEASE see the enclosed paper for more information!!
*
* This software should be used at your own risk.
*
* compile: cc -o covert_tcp covert_tcp.c
*
*
*
* Portions of this code based on ping.c (c) 1987 Regents of the
* University of California. (See function in_cksm() for details)
*
* Small portions from various packet utilities by unknown authors
*/

#ifndef CONVERT_TCP_H
#define CONVERT_TCP_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <linux/ip.h>

#define VERSION "1.1"

/* Prototypes */
void forgepacket(unsigned int, unsigned int, unsigned short, unsigned short,char *,int,int,int,int);
unsigned short in_cksum(unsigned short *, int);
unsigned int host_convert(char *);
void usage(char *);

#endif