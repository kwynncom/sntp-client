#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool onin() {
    static FILE *fp = NULL;
if (1) {

    if (fp == NULL) fp = fopen("/tmp/mysntpd", "r");
    char c;
    c = fgetc(fp);
    if (c == 'x') return false;
    return true;     
}


if (0) {    FILE *fp = fopen("/tmp/mysntpd", "r");
    char c;
    c = fgetc(fp);
    fclose(fp);
    if (c == 'x') return false;
    return true; 
}
}
