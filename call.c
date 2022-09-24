#include <unistd.h> // read, write, close
#include <stdio.h>  // stdout, fflush
#include <time.h>   // timespec struct
#include <stdlib.h>
#include <stdbool.h>

#include "utils.h"

void call20(int sock, struct timespec *bs, struct timespec *es, char *pack);
void call30(const struct timespec bs, const struct timespec es, const char *pack, const char *ip);

void ckq(void);
bool onin(void);

void call10(struct sockip *socks) {

    unsigned long i = 0;

    unsigned char pack[SNPL];
    struct timespec bsts;
    struct timespec ests;
    srand(time(NULL));
    int randi;
    
    while (true) {
        setOBPack(pack);
        randi = rand() % IPN;
        if (!onin()) return;
        ckq();
        call20(socks[randi].sock, &bsts, &ests, pack);
        call30(       bsts,  ests, pack, socks[randi].ip);
    }
}
void decodeSNTPP(const char *p, unsigned long *sr, unsigned long *ss);
void call20(const int sock, struct timespec *bs, struct timespec *es, char *pack) {

    ssize_t ignoreForWarn;

    clock_gettime(CLOCK_REALTIME, bs);
    if (write(sock, pack, SNPL) != SNPL) perror("bad write");
    if (read (sock, pack, SNPL) != SNPL) perror("bad read");
    clock_gettime(CLOCK_REALTIME, es);
}

void call30(const struct timespec bs, const struct timespec es, const char *pack, const char *ip) {
    
    const unsigned long b = bs.tv_sec * M_BILLION + bs.tv_nsec;
    const unsigned long e = es.tv_sec * M_BILLION + es.tv_nsec;  
    unsigned long bsl, esl;
    
    decodeSNTPP(pack, &bsl, &esl);

    printf("%lu\n%lu\n%lu\n%lu\n%s\n", b, bsl, esl, e, ip);
    
    // fflush(stdout); 
}
