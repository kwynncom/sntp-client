#include <unistd.h> // read, write, close
#include <stdio.h>  // stdout, fflush
#include <time.h>   // timespec struct
#include <stdbool.h>
#include <string.h> // memcpy
#include <stdlib.h> // rand
#include <errno.h> // bad file descriptor and other error codes

#include "all.h"

void callServer(const int sock, struct timespec *bs, struct timespec *es, char *pack);

void call10(const struct sockInfo *socks, const bool isd, const bool usefo, const bool qckb, const int rand1) {

    unsigned char pack[SNPL], packCache[SNPL];
    struct timespec bsts;
    struct timespec ests;
    int randi = rand1, randiCache = 0;
	bool didSend = false;
    
     do {
        setOBPack(pack);

		if (isd && rand1 < 0) randi = rand() % IPN;

        if (isd && !onin()) return;
        if ((!(socks[randi].alwaysQuota || qckb)) || qckf()) {
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

	if (sock <= 0) { printf("bad socket to SNTP call"); exit(2216); }

	calllog(true, 0, false, NULL);
    clock_gettime(CLOCK_REALTIME, bs);
    if (write(sock, pack, SNPL) != SNPL) fprintf(stderr, "bad write");
    if (read (sock, pack, SNPL) != SNPL) fprintf(stderr, "bad read" );
    clock_gettime(CLOCK_REALTIME, es);
}
