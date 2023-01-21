#include <unistd.h> // read, write, close
#include <stdio.h>  // stdout, fflush
#include <time.h>   // timespec struct
#include <stdbool.h>
#include <string.h> // memcpy
// #include <stdlib.h> // rand
#include <errno.h> // bad file descriptor and other error codes

#include "all.h"

bool callServer(const int sock, struct timespec *bs, struct timespec *es, char *pack);

void kwSetPackIE(char *pack, const char *msg) { 
	strncpy(pack + SNTPREFIDSTART, msg, REFIDSZ - 1); 
}

void call10(const struct sockInfo *socks, const bool isd, const bool qckb) {

    unsigned char pack[SNPL], packCache[SNPL];
    struct timespec bsts;
    struct timespec ests;
    int ipidx = 0, ipidxCache = 0;
	bool callret = true;
	unsigned long calli = 0;
    
     do {
        setOBPack(pack);

		if (isd) ipidx = calli % IPN;

        if (isd && !onin()) return;
        if ((!(socks[ipidx].alwaysQuota || qckb)) || qckf()) {
 			ipidxCache = ipidx;
            callret = callServer(socks[ipidxCache].sock, &bsts, &ests, pack);
			calli++;
        } else {
			setOBPack(pack);
			kwSetPackIE(pack, "KWQU");
		}

        if (!myoutf(       bsts,  ests, pack, socks[ipidxCache].iphu, isd)) return;
 		 
    } while (isd && callret);
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
