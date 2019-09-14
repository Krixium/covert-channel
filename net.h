#ifndef NET_H
#define NET_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

int getSockAddr(struct sockaddr_in *out, char *address, short port);

#endif