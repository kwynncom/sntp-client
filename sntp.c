#include <math.h> // round()
#include <stdlib.h> // malloc()
#include <unistd.h> // read, write, close
#include <stdio.h>  // stdout
#include <string.h> // memset, memcpy
#include "time.h"
#include "sntp.h"
#include "socket.h"

void sntp_get(int sock, unsigned long *loc, char (*pack)[SNTP_PLEN]) {
    
	popSNTPPacket(pack);
    *loc = nanotime();
    if (write(sock, pack, SNTP_PLEN) != SNTP_PLEN) ; // not sure I care about write errors
    const int retv = read (sock, pack, SNTP_PLEN);

    if (retv != SNTP_PLEN) { 
        memset(pack, 0, SNTP_PLEN); 
    }

    *(loc+1) = nanotime();
    return;

}
void popSNTPPacket ( char (*pack)[SNTP_PLEN]) {
    const uint32_t bit_max       = 4294967295;
    const uint32_t epoch_convert = 2208988800;
    uint32_t  secs;
    double    frac;
    int i = 0;

    memcpy(pack, "#", 1); // see PHP version; ord('#') == 0x23
    // memset(pack + 1, 0, SNTP_PLEN - 1);

    timeUFF(&secs, &frac);
    u32itobe(secs + epoch_convert, pack, 24);
    u32itobe((uint32_t)lround(frac * bit_max), pack, 28);
}

void u32itoli(uint32_t n, unsigned char *b, int o) {
    for(int i=0; i < 4; i++) b[i + o] = (n >> ((i) * 8)) & 0xff; 
}

void u32itobe(uint32_t n,char (*b)[SNTP_PLEN], int o) {
    char v;
    for(int i=0; i < 4; i++) {
        v =  (n >> (i * 8)) & 0xff;
        memcpy(b + 3 - i + o, &v, 1); 
    }
}

void sntp_doit(const int n, char *addr) {
    const int sock = getBoundSock(addr);
	char packs[n][SNTP_PLEN];

	unsigned long locs[n*2];

	int i;
	for(i=0; i < n; i++) {
			//  memset(packs[i], 0, SNTP_PLEN);	
            sntp_get(sock, &locs[i * 2], &packs[i]);
            }

	int wrl = 0;
	int ig  = 0;
    int len = 0;
	for(i=0; i < n; i++) { 
		len = sizeof(packs + i);
		wrl = fwrite(&packs[i], SNTP_PLEN, 1, stdout);
		// fwrite(locs + i * 2	   , sizeof(unsigned long), 1, stdout);
		// fwrite(locs + i * 2 + 1, sizeof(unsigned long), 1, stdout);
		ig = 2;
		continue;
	}

        exit(0);
}
