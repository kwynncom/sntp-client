#include <unistd.h> // read, write, close
#include <stdio.h>  // stdout, fflush
#include <time.h>   // timespec struct
#include <stdbool.h>

#include "utils.h"

void call20(int sock, struct timespec *bs, struct timespec *es, char *pack);
void call30(const struct timespec bs, const struct timespec es, const char *pack);

bool isq(void);

void call10(const int *socks) {

    unsigned long i = 0;

    char pack[SNPL];
    struct timespec bsts;
    struct timespec ests;
    
    setOBPack(pack);

    if (isq()) {
        printf("quota OK\n");
        // call20(socks[i++ % IPN], &bsts, &ests, pack);
        // call30(       bsts,  ests, pack);
    } else {
        perror("quota fail!\n");
        return;
    }
}

void call20(const int sock, struct timespec *bs, struct timespec *es, char *pack) {

    ssize_t ignoreForWarn;

    clock_gettime(CLOCK_REALTIME, bs);
    if (write(sock, pack, SNPL) != SNPL) perror("bad write");
    if (read (sock, pack, SNPL) != SNPL) perror("bad read");
    clock_gettime(CLOCK_REALTIME, es);
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
