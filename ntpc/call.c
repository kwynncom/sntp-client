#include <unistd.h> // read, write, close
#include <stdio.h>  // stdout, fflush
#include <time.h>   // timespec struct
#include <stdbool.h>
#include <string.h> // memcpy
#include <stdlib.h> // rand
#include <errno.h> // bad file descriptor and other error codes

#include "all.h"

void callServer(int sock, struct timespec *bs, struct timespec *es, char *pack);
void output(const struct timespec bs, const struct timespec es, const char *pack, const char *ip, bool isd, bool usefo, bool didSend);

bool qckf(void);
bool onin(void);

void call10(struct sockip *socks, bool isd, bool usefo, bool qckb, int rand1) {

    unsigned char pack[SNPL], packCache[SNPL];
    struct timespec bsts;
    struct timespec ests;
    int randi = rand1, randiCache = 0;
	bool didSend = false;
    
     do {
        setOBPack(pack);

		if (isd && rand1 < 0) randi = rand() % IPN;

        if (isd && !onin()) return;
        if (!qckb || qckf()) {
 			randiCache = randi;
            callServer(socks[randiCache].sock, &bsts, &ests, pack);
			didSend = true;
        } else didSend = false;

        output(       bsts,  ests, didSend ? pack : packCache, socks[randiCache].ip, isd, usefo, didSend);
        if (didSend) memcpy(packCache, pack, SNPL);
		 
    } while (isd);
}
void decodeSNTPP(const char *p, unsigned long *sr, unsigned long *ss);
void callServer(const int sock, struct timespec *bs, struct timespec *es, char *pack) {

	if (sock <= 0) { errno = EBADF; perror("bad socket to SNTP call"); exit(2216); }

	calllog(true, 0, false);
    clock_gettime(CLOCK_REALTIME, bs);
    if (write(sock, pack, SNPL) != SNPL) perror("bad write");
    if (read (sock, pack, SNPL) != SNPL) perror("bad read" );
    clock_gettime(CLOCK_REALTIME, es);
}

void output(const struct timespec bs, const struct timespec es, const char *pack, const char *ip, bool isd, bool usefo, bool newCall) {

    static char *fmt = "%lu\n%lu\n%lu\n%lu\n%s\n";
    
    const unsigned long b = bs.tv_sec * M_BILLION + bs.tv_nsec;
    const unsigned long e = es.tv_sec * M_BILLION + es.tv_nsec;  
    unsigned long bsl, esl;
    
    decodeSNTPP(pack, &bsl, &esl);

    printf (fmt, b, bsl, esl, e, ip);
	printf("VERSION: %s %s", KWSNTPV, "\n");

	if (isd && usefo) {
		FILE   *outf = fopen(KWSNTPDEXTGET, "w");
		fprintf(outf, fmt, b, bsl, esl, e, ip);
		fprintf(outf, "VERSION: %s %s", KWSNTPV, "\n");
		fclose(outf);
	}

	calllog(false, b, false);

}
