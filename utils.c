#include <time.h>   // timespec struct
#include <stdlib.h> // exit()
#include <stdio.h> // perror
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <string.h> // strcmp
#include <strings.h> // bzero

#include "config.h"

char *getAddr(int argc, char **argv) {

	if (argc < 2) return KW_DEFAULT_NTP_SERVER_IP6;

	char *arg;
	int argl;

	arg  = argv[1];
	argl = strlen(arg);

	
	if (argl < 7 || argl > 39) // "1.2.3.4" is 7 chars; IPv6 max 39 chars
		{ fprintf(stderr, "bad IP length of %d\n", argl); exit(EXIT_FAILURE);}

	if (strstr(arg, ".") == NULL) return arg;

	if (argl > 15) { fprintf(stderr, "bad IPv4 address - too long\n"); exit(EXIT_FAILURE);}

	const char *ip46p = "::FFFF:";
	const int bsz = strlen(ip46p) + 15 + 1;
	char *sbuf = (char *)malloc(bsz);
	sbuf = strcat(sbuf, ip46p);
	sbuf = strcat(sbuf, arg);

	return sbuf;
}

unsigned long nanotime() {
    struct timespec sts;
    if (clock_gettime(CLOCK_REALTIME, &sts) != 0) exit(8131);
    return sts.tv_sec * 1000000000 + sts.tv_nsec;
}

int getOutboundUDPSock(char *addrStr, int port) {

    int sock;
	struct sockaddr_in6 addro; // addr object, more of a PHP convention, but whatever

    bzero(&addro, sizeof(addro));

    addro.sin6_family = AF_INET6; 
	if (inet_pton(AF_INET6, addrStr, &addro.sin6_addr) != 1) { fprintf(stderr, "bad IP address\n"); exit(EXIT_FAILURE); }
    addro.sin6_port = htons(port);

    if ((sock = socket(AF_INET6, SOCK_DGRAM, 17)) < 0) { perror("socket creation failed"); exit(EXIT_FAILURE); }

    struct timeval timeout;      
    timeout.tv_sec  = 1;
    timeout.tv_usec = 0;

    if (setsockopt (sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0) perror("setsockopt failed\n");
    if (connect(sock, (struct sockaddr *) &addro, sizeof(addro)) != 0) {  printf("connection with the server failed...\n"); exit(EXIT_FAILURE); } 
        
    return sock;
}
