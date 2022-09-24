#include "utils.h"
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>

bool ckq() {
    static long double prev = 0;
    const long double now = Ufl();
    const long double d =  now - prev;
    const long double max = NISTMaxS;
    const long double togo = max - d;
    if (togo <= 0) {
        prev = now;
        return true;
    }
    return false;
}
