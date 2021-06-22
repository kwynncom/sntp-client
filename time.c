#include <time.h>   // timespec struct
#include <stdlib.h> // exit()
#include "time.h"

void timeUFF(uint32_t *u, double *f) {
                      // 123456789
   const long billion = 1000000000;

    struct timespec sts;
    if (clock_gettime(CLOCK_REALTIME, &sts) != 0) exit(8129);

    *u = (uint32_t) sts.tv_sec;     
    *f = (double  ) sts.tv_nsec / billion;
}

unsigned long nanotime() {
    struct timespec sts;
    if (clock_gettime(CLOCK_REALTIME, &sts) != 0) exit(8131);
    return sts.tv_sec * 1000000000 + sts.tv_nsec;
}
