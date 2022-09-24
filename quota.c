#include "utils.h"
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void ckq() {
    static long double prev = 0;
    const long double now = Ufl();
    const long double d =  now - prev;
    const long double max = NISTMaxS;
    const long double togo = max - d;
    prev = now;
    if (togo <= 0) return;
    const long usec = (long)roundl(togo * M_MILLION);
    if (usleep(usec) != 0) { 
        perror("usleep fail!\n");
        exit(345);
    }
    
    prev = Ufl();
    const int ignore = 1;
}

