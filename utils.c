#include <stdlib.h> // exit()
#include <stdio.h> // perror
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <string.h> // strcmp
#include <strings.h> // bzero
#include <time.h>   // timespec struct
#include <math.h>
#include <stdbool.h>
#include <sys/file.h>

#include "utils.h"

FILE *getLockedFile() {
    FILE   *lockf = fopen(KWSNTPDLOCKFILE, "w");
    if (flock(fileno(lockf), LOCK_EX | LOCK_NB) != 0) { perror("fifo output lock failure - another process running\n"); exit(28); }
    return lockf;
}

void procArgs(int argc, char *argv[], bool *isd, bool *usefo) {
	*isd = *usefo = false;
	if (argc < 2) return;
	int i;
	for (i=1; i < argc; i++) if (strcmp("-d"	  , argv[i]) == 0) *isd   = true;
	for (i=1; i < argc; i++) if (strcmp("-fifoout", argv[i]) == 0) *usefo = true;
}


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

void popIPs(char **a);

void popSocks(struct sockip *socks) {
    char *ips[IPN];
    popIPs(ips);
    int i;
    for (i=0; i < IPN; i++) {
        strcpy(socks[i].ip, ips[i]);
        socks[i].sock = getOutboundUDPSock(getAddr(ips[i]), 123);
    }
}

void popIPs(char **a) {
    if (1) { 
        a[0] = "129.6.15.28";
        a[1] = "129.6.15.29";
        a[2] = "129.6.15.30";
        a[3] = "129.6.15.27";
        a[4] = "2610:20:6f15:15::27";
        a[5] = "129.6.15.26";
        a[6] = "2610:20:6f15:15::26";
    } else {
        int i;
        for (i = 0; i < IPN; i++) a[i] = TESTIP;
    }
}

long double Ufl() {
    struct timespec sts;
    if (clock_gettime(CLOCK_REALTIME, &sts) != 0) exit(8131);
    return (double) sts.tv_sec + ((double)sts.tv_nsec / (double)M_BILLION);
}

void decodeSNTPP(const unsigned char *p, unsigned long *sr, unsigned long *ss) {

    const unsigned int UminusNTP = 2208988800;
    const unsigned int full32    = 4294967295; 
    int i = 0;
    int j = 0;
    unsigned int ntps = 0;
    unsigned int U;
    unsigned int fri;
    double fr;
    unsigned long Uns;

    for (j = 0; j < 2; j++) {
        
        U = 0;
        fri = 0;

        for (i=0; i < 4; i++) ntps = ntps | (p[i+32 + j * 8] << (8 * (3 - i)));

        U    = ntps - UminusNTP;

        for (i=0; i < 4; i++) fri = fri | (p[i+36 + j * 8] << (8 * (3 - i)));    

        fr = (double)fri / (double)full32;

        Uns = (unsigned long)U * M_BILLION + (unsigned long)round(fr * M_BILLION);

        if (j == 0) *sr = Uns;
        else        *ss = Uns;
        
        const int ignore = 1;
    } // loop
} // func

bool onin() {
    FILE *fp = fopen(KWSNTPDPOKE, "r");
    char c;
    c = fgetc(fp);
    fclose(fp);
    if (c == 'x') return false;
    return true; 
} // func

bool ckq() {
    static long double prev = 0;
    const long double now = Ufl();
    const long double d =  now - prev;
    const long double max = NISTMaxS;
    const long double togo = max - d;
    if (togo <= 0) {
        prev = now;
        return true;
    }
    return false;
}
