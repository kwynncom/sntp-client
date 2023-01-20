#include <unistd.h> // read, write, close
#include <stdio.h>  // stdout, fflush
#include <time.h>   // timespec struct
#include <stdbool.h>
#include <string.h> // memcpy
#include <stdlib.h> // rand
#include <errno.h> // bad file descriptor and other error codes

#include "all.h"

bool callServer(const int sock, struct timespec *bs, struct timespec *es, char *pack);

void call10(const struct sockInfo *socks, const bool isd, const bool qckb, const int randOne) {

    unsigned char pack[SNPL], packCache[SNPL];
    struct timespec bsts;
    struct timespec ests;
    int randi = randOne, randiCache = 0;
	bool didSend = false;
	bool callret = true;
    
     do {
        setOBPack(pack);

		if (isd && randOne >= IPN) randi = rand() % IPN;

        if (isd && !onin()) return;
        if ((!(socks[randi].alwaysQuota || qckb)) || qckf()) {
 			randiCache = randi;
            callret = callServer(socks[randiCache].sock, &bsts, &ests, pack);
			didSend = true;
        } else didSend = false;

        if (!myoutf(       bsts,  ests, didSend ? pack : packCache, socks[randiCache].iphu, isd, didSend)) return;
        if (didSend) memcpy(packCache, pack, SNPL);
		 
    } while (isd && callret);
}

void kwSetPackIE(char *pack, const char *msg) { 
	strncpy(pack + SNTPREFIDSTART, msg, REFIDSZ - 1); 
}

bool callServer(const int sock, struct timespec *bs, struct timespec *es, char *pack) {

	if (sock <= 0) { fprintf(stderr, "bad socket to SNTP call"); return false; }

	calllog(true, 0, false, NULL);
    clock_gettime(CLOCK_REALTIME, bs);
    if (write(sock, pack, SNPL) != SNPL) { fprintf(stderr, "bad write\n"); kwSetPackIE(pack, KWSNBW); return false; }
    if (read (sock, pack, SNPL) != SNPL) { kwSetPackIE(pack, KWSNBR); /* do not return false because it will break the main call loop above */	}
    clock_gettime(CLOCK_REALTIME, es);

	return true;
}
