# Covert UDP Channel
Covert channel application that sends a file using the UDP header. The contents of the file are sent over the network byte by byte using the lower byte of the UDP srouce field.

```
Usage: covert [server|client]

Client Mode Required:
    -s - source ip, the source ip to put into the packet(this can be any valid ip)
    -d - The desired destination ip of the packet.
    -p - The desired destination port of the packet.
    -f - The file to send.

Server Mode Required:
    -s - The ip of the client that is sending covert messages.
    -p - The desired destination port of the packet.
    -f - The file to write incoming data into.
```