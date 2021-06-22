#include <stdio.h> // perror
// #include <sys/types.h> // fork
// #include <unistd.h>    // fork
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <stdlib.h> // exit
#include <string.h> // strcmp
#include <strings.h> // bzero

#include "sntp.h"

int getBoundSock(char *outaddr) {
    struct sockaddr_in saddr;
    int sock, type, prot;

    type = SOCK_DGRAM ; 
	prot = 17;

    bzero(&saddr, sizeof(saddr));
    saddr.sin_family = AF_INET; 
    saddr.sin_addr.s_addr = inet_addr(outaddr); 
    saddr.sin_port = htons(NTP_PORT);

    if ((sock = socket(AF_INET, type, prot)) < 0) { perror("socket creation failed"); exit(EXIT_FAILURE); }

    struct timeval timeout;      
    timeout.tv_sec  = 1;
    timeout.tv_usec = 0;

    if (setsockopt (sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0) perror("setsockopt failed\n");
    if (connect(sock, (struct sockaddr *) &saddr, sizeof(saddr)) != 0) {  printf("connection with the server failed...\n"); exit(EXIT_FAILURE); } 
        
    return sock;
}
