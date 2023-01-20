#include <stdlib.h> // rand(), abs(), etc.
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
#include "out.h"

void mysleep() {
	int sl = (int)round(NISTMaxS);
	printf("Sleeping for %d seconds, from C.  Zzzzz....\n", sl);
	sleep(sl);
}

void closeOutputFiles();
void cleanup(const struct sockInfo *socks, const FILE *lockf) {
	for (int i=0; i < IPN; i++) if (socks[i].sock >= 0) close(socks[i].sock);

    flock(fileno((FILE *)lockf), LOCK_UN);
    fclose((FILE *)lockf);
	calllog(false, 0, true, NULL);
	closeOutputFiles();
}

int popIPs(struct sockInfo *socks, const char *ipin, const bool isd);

int popSocks(struct sockInfo *socks, const char *ipin, const bool isd) {
    
	int ipnl = popIPs(socks, ipin, isd);
	if (ipnl < 1) return KWSNTP_RETURN_FAIL;
    int i = 0;

    for (i=0; i < ipnl; i++) {
		if (!setAddr(socks[i].iphu, socks[i].ip46)) return KWSNTP_RETURN_FAIL;
		socks[i].sock = getOutboundUDPSock(socks[i].ip46, 123);
		if (socks[i].sock < 0) return KWSNTP_RETURN_FAIL; 
	}
   
	if (isd && ipnl == IPN) return IPN;
	else return 0;
}

int popIPs(struct sockInfo *socks, const char *ipin, const bool isd) {

	int i;
							
	const char *nista[IPN] = {         "129.6.15.26",         "129.6.15.27", "129.6.15.28", "129.6.15.29", "129.6.15.30", 
							   "2610:20:6f15:15::26", "2610:20:6f15:15::27" }; /* https://tf.nist.gov/tf-cgi/servers.cgi */

	const int l = strlen(ipin);
	if (l >= MINIPL) {

		if (l > MAXIPL - 1) { fprintf(stderr, "IP too long"); return 0; }

		strncpy(socks[0].iphu, ipin, MAXIPL);
		socks[0].alwaysQuota = false;
		for (i=0; i < IPN; i++) if (strcmp(ipin, nista[i]) == 0) socks[0].alwaysQuota = true;

		return 1;
	}
	
	for (i=0; i < IPN; i++) {
		strncpy(socks[i].iphu, nista[isd ? i : rand() % IPN], MAXIPL);
		socks[i].alwaysQuota = true;		
		if (!isd) return 1;
	}

	if (IPN == 7) return IPN;

	fprintf(stderr, "ERROR: expecting a specific IPN const / #define value");
	return 0;
}


FILE *getLockedFile() {
    FILE   *lockf = fopen(KWSNTPDLOCKFILE, "w");
	if (lockf == NULL) return NULL;
    if (flock(fileno(lockf), LOCK_EX | LOCK_NB) != 0) { 
		fclose(lockf);
		lockf = NULL;
		perror("fifo output lock failure - another process running\n"); // correct errno by default
	}
    return lockf;
}

void procArgs(int argc, char *argv[], bool *isd, bool *dosleep, bool *qck, char (*ip)[MAXIPL]) {
	*isd = false;
	*dosleep = *qck = true;
	*ip[0] = '\0'; // need to do this now in case of return just below

	if (argc < 2) return;
	int i;
	for (i=1; i < argc; i++) {
		if (strcmp("-d"		 , argv[i]) == 0) *isd		= true;
		if (strcmp("-nosleep", argv[i]) == 0) *dosleep  = false;
		if (strcmp("-noqck"  , argv[i]) == 0) *qck      = false;
		if (strcmp("-ip"  , argv[i]) == 0 && i < argc + 1 && strlen(argv[i + 1]) < MAXIPL) strncpy((char *)ip, argv[i + 1], MAXIPL);
		
	}
}

void setOBPack(char *pack) {
	// bit-in-packet index						   01234567
	//											   LIvvvmmm
	// SNTP packet header - "#" ASCII === 0x23 === 00100011
	// the very next bit begins the "stratum" field, which is also used for the KoD (kiss of death) indication
	// https://datatracker.ietf.org/doc/rfc4330/ ; Simple Network Time Protocol (SNTP) Version 4 for IPv4, IPv6 and OSI ; RFC 4330
	// 00 === LI === leap second indicator; 100 === 4 or SNTP v4.  011 === 3 "mode" 3 means I am an SNTP client

    memcpy(pack    , "#",  1); // set first byte of header, as just above 
    bzero (pack + 1,      47); // nothing else is needed for my purposes, so 0 fill.  0 fill works, from long experience.
}

bool setAddr(const char *ipin, char *ipout) {

	bzero(ipout, MAXIPL);

	const int argl = strlen(ipin);
	
	if (argl < MINIPL || argl > MAXIPL - 1) // "1.2.3.4" is 7 chars; IPv6 max 39 chars; ::1 is 3
		{ fprintf(stderr, "bad IP length of %d\n", argl); return false; }

	if (strstr((char *)ipin, ".") == NULL) {
		strncpy(ipout, ipin, MAXIPL);
		return true;
	}

	if (argl > 15) { fprintf(stderr, "bad IPv4 address - too long\n"); return false; }

	const char *ip46p  = "::FFFF:";
	strcat(ipout, ip46p);
	strcat(ipout, ipin);

	return true;
}

int getOutboundUDPSock(const char *addrStr, const int port) {

    int sock;
	struct sockaddr_in6 addro; // addr object, more of a PHP convention, but whatever

    bzero(&addro, sizeof(addro));

    addro.sin6_family = AF_INET6; 
	if (inet_pton(AF_INET6, addrStr, &addro.sin6_addr) != 1) { 
		fprintf(stderr, "bad IP address (pton): %s\n", addrStr); 
		return -1;
	}
    addro.sin6_port = htons(port);

    if ((sock = socket(AF_INET6, SOCK_DGRAM, 17)) < 0) { perror("socket creation failed"); return -1; }

    struct timeval timeout;      
    timeout.tv_sec  = 1;
    timeout.tv_usec = 0;

    if (setsockopt (sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0) { perror("setsockopt failed\n"); return -1;  }
    if (connect(sock, (struct sockaddr *) &addro, sizeof(addro)) != 0) {  perror("connection with the server failed...\n"); return -1; } 
        
    return sock;
}



long double Ufl() {
    struct timespec sts;
    clock_gettime(CLOCK_REALTIME, &sts);
    return (double) sts.tv_sec + ((double)sts.tv_nsec / (double)M_BILLION);
}


void decSNTPPTime(const unsigned char *p, unsigned long *sr, unsigned long *ss) {

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

	// decodeSNTPResponseHeader(p);
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



unsigned long nanotime() {
    struct timespec sts;
	if (clock_gettime(CLOCK_REALTIME, &sts) != 0) return 0;
    return sts.tv_sec * 1000000000 + sts.tv_nsec;
}

bool sanityCheck(const unsigned long a, const unsigned long b, const unsigned long c, const unsigned long d) {

	if (b > c) return false;
	if (a > d) return false;
	if (d - a > TOLERANCENS) return false;
	if ((abs(d - c) + abs(b - a)) > TOLERANCENS) return false;
	if (nanotime() - a > TOLERANCENS) return false;
	return true;
}

void setRefIDIE() {

}

// returns stratum ; SNTP_spect.txt has format / byte / bit details
unsigned int decSNTPStratumRefID(const unsigned char *p, char ri[REFIDSZ]) {
	const unsigned int stratum = (unsigned int)p[1];
	for (int i=0; i < REFIDSZ - 1; i++) ri[i] = p[i+SNTPREFIDSTART];
	return stratum;
}

FILE *kwsnfifoptrGlob = NULL;
FILE *kwsnLogFPGlob	  = NULL;

void closeOutputFiles() {
	// myoutf(NULL, NULL, NULL, NULL, NULL, false, true);
	if (kwsnfifoptrGlob != NULL) fclose(kwsnfifoptrGlob);
	if (kwsnLogFPGlob   != NULL) fclose(kwsnLogFPGlob);
}

bool myoutf(const struct timespec bs, const struct timespec es, const char *pack, const char *ip, 
			const bool isd, const bool newCall) {

    const unsigned long b = bs.tv_sec * M_BILLION + bs.tv_nsec;
    const unsigned long e = es.tv_sec * M_BILLION + es.tv_nsec;  

    unsigned long bsl, esl;
    
    decSNTPPTime(pack, &bsl, &esl);
	const bool ck = sanityCheck(b, bsl, esl, e);

	static char refid[REFIDSZ];
	bzero(refid, REFIDSZ);
	const int stratum = decSNTPStratumRefID(pack, refid);

	static char ob10[obuffullsz];
	myout20f(ob10, b, bsl, esl, e, ip, stratum, refid, isd, ck);

	static FILE *outf = NULL;

// fifo IS conditional on isd, not the new file
	if (kwsnLogFPGlob == NULL) {
		outf = kwsnLogFPGlob = fopen(ALLRESULTSFILE, "a"); 
		if (kwsnLogFPGlob == NULL) { perror("output full log file open fail"); return false; }
	}

	if (isd && kwsnfifoptrGlob == NULL) {
		kwsnfifoptrGlob = fopen(KWSNTPDEXTGET, "w"); 
		if (kwsnfifoptrGlob == NULL) { perror("output file (fifo) open fail"); return false; }
	}

	const char *begs = "*** BEGIN ***\n";

	if (outf != NULL) 
		fprintf(outf, "%s", begs);
		printf (	  "%s", begs); // unconditional

	if (outf != NULL) 
		fprintf(outf, "%s", ob10);
		printf (	  "%s", ob10); // unconditional

	calllog(false, b, false, outf);


	if (outf) fflush(outf);
			  fflush(stdout); // unc

	if (kwsnfifoptrGlob != NULL) {
		// putc('a', kwsnfifoptrGlob);
		fprintf(kwsnfifoptrGlob, "a\n");
		fflush(kwsnfifoptrGlob);
	}

	return true;
}

void calllog(const bool newCall, const unsigned long Uus, const bool doClose, FILE *genof ) {
	static FILE *f = NULL;
	static unsigned long prevts = 1;

	int fpfr = 0;

	if (doClose) { if (f != NULL) fclose(f); return; }
	if (f == NULL) f = fopen(LOGFILE, "a");

	char hubuf[hubufsz];
	hutime(Uus, hubuf);

	char *cs = "(unk - should not happen)";
	if (Uus == prevts && !newCall) cs = "(cached / quota fail)";
	else if (newCall) cs = "";
	else cs = "(after call with ns)";
	
	char *fmt =  "%s %s\n";

	if (newCall) {
		fprintf(f, fmt, hubuf, cs);	
		fflush(f); // note that you cannot write out fifo or stdout because I assume the first 4 lines are ns times
	} else {
		printf(    fmt, hubuf, cs);
		fflush(stdout);
		if (genof != NULL) fprintf(genof, fmt, hubuf, cs);
		fflush(genof);
		prevts = Uus;

		
	}
}
