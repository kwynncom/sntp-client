#include <string.h> // memset, memcpy
#include <unistd.h> // read, write, close
#include <stdio.h>  // stdout
#include <stdlib.h> // malloc
#include <time.h>   // timespec struct

#include "utils.h"

void callSNTPAndWrite(int sock) {

    const unsigned int snpl = 48; // SNTP packet length

    char pack[snpl];
    unsigned long b, e;
    struct timespec bsts;
    struct timespec ests;
    
    setOBPack(pack);

    int i;
    for (i=0; i < 1; i++) {
    struct timespec sts;
        clock_gettime(CLOCK_REALTIME, &bsts);
        if (write(sock, pack, snpl) != snpl) perror("bad write");
        if (read (sock, pack, snpl) != snpl) perror("bad read");
        clock_gettime(CLOCK_REALTIME, &ests);

        b = bsts.tv_sec * M_BILLION + bsts.tv_nsec;
        e = ests.tv_sec * M_BILLION + ests.tv_nsec;

        fwrite(&pack, snpl     , 1, stdout);
        fwrite(&b	, sizeof(b), 1, stdout);
        fwrite(&e	, sizeof(e), 1, stdout); 
        fflush(stdout);
        // sleep(4);
    }

    close(sock);
}
