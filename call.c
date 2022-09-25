#include <unistd.h> // read, write, close
#include <stdio.h>  // stdout, fflush
#include <time.h>   // timespec struct
#include <stdlib.h>
#include <stdbool.h>

#include "utils.h"

void callServer(int sock, struct timespec *bs, struct timespec *es, char *pack);
void output(const struct timespec bs, const struct timespec es, const char *pack, const char *ip);

bool ckq(void);
bool onin(void);

void call10(struct sockip *socks) {

    unsigned long i = 0;

    unsigned char pack[SNPL];
    struct timespec bsts;
    struct timespec ests;
    srand(time(NULL));
    int randi;
    
    while (true) {
        if (!onin()) return;
        if (ckq()) {
            setOBPack(pack);
            randi = rand() % IPN;
            ckq();
            callServer(socks[randi].sock, &bsts, &ests, pack);
        }

        output(       bsts,  ests, pack, socks[randi].ip);
    }
}
void decodeSNTPP(const char *p, unsigned long *sr, unsigned long *ss);
void callServer(const int sock, struct timespec *bs, struct timespec *es, char *pack) {
    clock_gettime(CLOCK_REALTIME, bs);
    if (write(sock, pack, SNPL) != SNPL) perror("bad write");
    if (read (sock, pack, SNPL) != SNPL) perror("bad read" );
    clock_gettime(CLOCK_REALTIME, es);
}

void output(const struct timespec bs, const struct timespec es, const char *pack, const char *ip) {

    static char *fmt = "%lu\n%lu\n%lu\n%lu\n%s\n";
    
    const unsigned long b = bs.tv_sec * M_BILLION + bs.tv_nsec;
    const unsigned long e = es.tv_sec * M_BILLION + es.tv_nsec;  
    unsigned long bsl, esl;
    
    decodeSNTPP(pack, &bsl, &esl);

    printf (      fmt, b, bsl, esl, e, ip);

    FILE   *outf = fopen("/var/kwynn/mysd/get", "w");
    fprintf(outf, fmt, b, bsl, esl, e, ip);
    fclose(outf);

}
