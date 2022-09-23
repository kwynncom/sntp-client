#include <unistd.h> // read, write, close
#include <stdio.h>  // stdout, fflush
#include <time.h>   // timespec struct

#include "utils.h"

void call20(int sock, struct timespec *bs, struct timespec *es, char *pack);
void call30(const struct timespec bs, const struct timespec es, const char *pack);

void ckq(void);

void call10(const int *socks) {

    unsigned long i = 0;

    unsigned char pack[SNPL];
    struct timespec bsts;
    struct timespec ests;
    
    setOBPack(pack);

    ckq();
    call20(socks[i++ % IPN], &bsts, &ests, pack);
    call30(       bsts,  ests, pack);
}
void decodeSNTPP(const char *p /*, unsigned long *sr, unsigned long *ss*/);
void call20(const int sock, struct timespec *bs, struct timespec *es, char *pack) {

    ssize_t ignoreForWarn;

    clock_gettime(CLOCK_REALTIME, bs);
    if (write(sock, pack, SNPL) != SNPL) perror("bad write");
    if (read (sock, pack, SNPL) != SNPL) perror("bad read");
    clock_gettime(CLOCK_REALTIME, es);
    decodeSNTPP(pack);

}

void call30(const struct timespec bs, const struct timespec es, const char *pack) {
    
    const unsigned long b = bs.tv_sec * M_BILLION + bs.tv_nsec;
    const unsigned long e = es.tv_sec * M_BILLION + es.tv_nsec;  
    static size_t lsz = sizeof(b);
    
    fwrite( pack, SNPL, 1, stdout);
    fwrite(&b	, lsz , 1, stdout);
    fwrite(&e	, lsz , 1, stdout);
    fflush(stdout); 
}