#include <unistd.h> // read, write, close
#include <stdio.h>  // stdout, fflush
#include <time.h>   // timespec struct

#include "utils.h"

void call20(int sock, struct timespec *bs, struct timespec *es, char *pack);

void call10(int sock) {

    char pack[SNPL];
    unsigned long b, e;
    struct timespec bsts;
    struct timespec ests;
    
    setOBPack(pack);

    call20(sock, &bsts, &ests, pack);

    b = bsts.tv_sec * M_BILLION + bsts.tv_nsec;
    e = ests.tv_sec * M_BILLION + ests.tv_nsec;

    fwrite(&pack, SNPL     , 1, stdout);
    fwrite(&b	, sizeof(b), 1, stdout);
    fwrite(&e	, sizeof(e), 1, stdout); 
    fflush(stdout);
}

void call20(int sock, struct timespec *bs, struct timespec *es, char *pack) {

    ssize_t ignoreForWarn;

    clock_gettime(CLOCK_REALTIME, bs);
    ignoreForWarn = write(sock, pack, SNPL);
    ignoreForWarn = read (sock, pack, SNPL);
    clock_gettime(CLOCK_REALTIME, es);
}
