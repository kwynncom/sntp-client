#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool onin() {
    FILE *fp = fopen("/tmp/mysntpd", "r");
    char c;
    c = fgetc(fp);
    fclose(fp);
    if (c == 'x') return false;
    return true;
}
