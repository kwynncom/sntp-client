#include "utils.h"
#include <stdbool.h>

bool isq() {
    static double prev = 0;
    double now = Ufl();
    return now - prev > NISTMaxS;
}
