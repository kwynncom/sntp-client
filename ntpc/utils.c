#include <stdlib.h> // exit()
#include <stdio.h> // perror
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <string.h> // strcmp
#include <strings.h> // bzero
#include <time.h>   // timespec struct
#include <math.h>
#include <stdbool.h>
#include <sys/file.h> // LOCK_UN
#include <unistd.h> // sleep, close
#include <errno.h>

#include "all.h"

void mysleep() {
	int sl = (int)round(NISTMaxS);
	printf("Sleeping for %d seconds, from C.  Zzzzz....\n", sl);
	sleep(sl);
}

void cleanup(const struct sockInfo *socks, const FILE *lockf) {
	for (int i=0; i < IPN; i++) close(socks[i].sock);

    flock(fileno((FILE *)lockf), LOCK_UN);
    fclose((FILE *)lockf);
	calllog(false, 0, true);
}

int popIPs(struct sockInfo *socks, const char *ipin);

int popSocks(struct sockInfo *socks, const char *ipin, const bool isd) {
    
    int ipnl = popIPs(socks, ipin);
    int i = 0;
	int maxi = 0;

	if (!isd && IPN == ipnl) {
		i = rand() % IPN;
		maxi = IPN;
	}
	else { 
		i = 0;
		maxi = ipnl;
	}

    for (; i < ipnl; i++) socks[i].sock = getOutboundUDPSock(getAddr(socks[i].ip), 123);
   
	if (ipnl < IPN && i > 0) return 0;
	else if (!isd)			 return i;
	else	  return -1;
}


FILE *getLockedFile() {
    FILE   *lockf = fopen(KWSNTPDLOCKFILE, "w");
    if (flock(fileno(lockf), LOCK_EX | LOCK_NB) != 0) { 
		perror("fifo output lock failure - another process running\n"); // correct errno by default
		exit(28); 
	}
    return lockf;
}

void procArgs(int argc, char *argv[], bool *isd, bool *usefo, bool *dosleep, bool *qck, char (*ip)[MAXIPL]) {
	*isd = *usefo = false;
	*dosleep = *qck = true;
	if (argc < 2) return;
	int i;
	*ip[0] = '\0';
	for (i=1; i < argc; i++) {
		if (strcmp("-d"		 , argv[i]) == 0) *isd		= true;
		if (strcmp("-fifoout", argv[i]) == 0) *usefo    = true;
		if (strcmp("-nosleep", argv[i]) == 0) *dosleep  = false;
		if (strcmp("-noqck"  , argv[i]) == 0) *qck      = false;
		if (strcmp("-ip"  , argv[i]) == 0 && i < argc + 1 && strlen(argv[i + 1]) < MAXIPL) strcpy((char *)ip, argv[i + 1]);
		
	}
}

void setOBPack(char *pack) {
    memcpy(pack    , "#",  1); // SNTP packet header - see readme
    bzero (pack + 1,      47);
}

char *getAddr(const char *ips) {

	int argl;
	argl = strlen(ips);
	
	if (argl < MINIPL || argl > MAXIPL) // "1.2.3.4" is 7 chars; IPv6 max 39 chars; ::1 is 3
		{ fprintf(stderr, "bad IP length of %d\n", argl); exit(EXIT_FAILURE);}

	if (strstr((char *)ips, ".") == NULL) return (char *)ips;

	if (argl > 15) { fprintf(stderr, "bad IPv4 address - too long\n"); exit(EXIT_FAILURE);}

	const char *ip46p = "::FFFF:";
	const int bsz = strlen(ip46p) + 15 + 1;
	char *sbuf = (char *)malloc(bsz);
	sbuf = strcat(sbuf, ip46p);
	sbuf = strcat(sbuf, ips);

	return sbuf;
}

int getOutboundUDPSock(const char *addrStr, const int port) {

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

    if (setsockopt (sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0) { perror("setsockopt failed\n"); exit(2236); }
    if (connect(sock, (struct sockaddr *) &addro, sizeof(addro)) != 0) {  perror("connection with the server failed...\n"); exit(EXIT_FAILURE); } 
        
    return sock;
}

int popIPs(struct sockInfo *socks, const char *ipin) {

	int i;
							
	const char *nista[IPN] = {         "129.6.15.26",         "129.6.15.27", "129.6.15.28", "129.6.15.29", "129.6.15.30", 
							   "2610:20:6f15:15::26", "2610:20:6f15:15::27" }; /* https://tf.nist.gov/tf-cgi/servers.cgi */

	const int l = strlen(ipin);
	if (l >= MINIPL) {

		if (l > MAXIPL - 1) { printf("IP too long"); exit(2328); }

		strcpy(socks[0].ip, ipin);
		socks[0].alwaysQuota = false;
		for (i=0; i < IPN; i++) if (strcmp(ipin, nista[i]) == 0) socks[0].alwaysQuota = true;

		return 1;
	}

	for (i=0; i < IPN; i++) {
		strcpy(socks[i].ip, nista[i]);
		socks[i].alwaysQuota = true;		
	}

	if (IPN == 7) return IPN;

	printf("ERROR: expecting a specific IPN const / #define value");
	exit(2154);
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

bool qckf() {

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
