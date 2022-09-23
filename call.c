#include <unistd.h> // read, write, close
#include <stdio.h>  // stdout, fflush
#include <time.h>   // timespec struct

#include "utils.h"

void call20(int sock, struct timespec *bs, struct timespec *es, char *pack);
void call30(const struct timespec bs, const struct timespec es, char *pack);

void call10(int sock) {

    char pack[SNPL];
    struct timespec bsts;
    struct timespec ests;
    
    setOBPack(pack);

    call20(sock, &bsts, &ests, pack);
    call30(       bsts,  ests, pack);




}

void call20(const int sock, struct timespec *bs, struct timespec *es, char *pack) {

    ssize_t ignoreForWarn;

    clock_gettime(CLOCK_REALTIME, bs);
    if (write(sock, pack, SNPL) != SNPL) perror("bad write");
    if (read (sock, pack, SNPL) != SNPL) perror("bad read");
    clock_gettime(CLOCK_REALTIME, es);
}

void call30(const struct timespec bs, const struct timespec es, char *pack) {
    
    const unsigned long b = bs.tv_sec * M_BILLION + bs.tv_nsec;
    const unsigned long e = es.tv_sec * M_BILLION + es.tv_nsec;  
    static size_t lsz = sizeof(b);
    
    fwrite( pack, SNPL, 1, stdout);
    fwrite(&b	, lsz , 1, stdout);
    fwrite(&e	, lsz , 1, stdout);
    fflush(stdout); 
}
