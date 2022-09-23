#include <stdlib.h> // exit()
#include <stdio.h> // perror
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <string.h> // strcmp
#include <strings.h> // bzero

void setOBPack(char *pack) {
    memcpy(pack    , "#",  1); // SNTP packet header - see readme
    bzero (pack + 1,      47);
}

char *getAddr(char *ips) {

	int argl;
	argl = strlen(ips);
	
	if (argl < 7 || argl > 39) // "1.2.3.4" is 7 chars; IPv6 max 39 chars
		{ fprintf(stderr, "bad IP length of %d\n", argl); exit(EXIT_FAILURE);}

	if (strstr(ips, ".") == NULL) return ips;

	if (argl > 15) { fprintf(stderr, "bad IPv4 address - too long\n"); exit(EXIT_FAILURE);}

	const char *ip46p = "::FFFF:";
	const int bsz = strlen(ip46p) + 15 + 1;
	char *sbuf = (char *)malloc(bsz);
	sbuf = strcat(sbuf, ip46p);
	sbuf = strcat(sbuf, ips);

	return sbuf;
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

void popIPs(const char *a) {
    
}

// const char * 

/* const char *a[2];
a[0] = "blah";
a[1] = "hmm"; */

/* 
129.6.15.28 
129.6.15.29 
129.6.15.30 
129.6.15.27 
2610:20:6f15:15::27 
129.6.15.26 
2610:20:6f15:15::26 
*/ 
