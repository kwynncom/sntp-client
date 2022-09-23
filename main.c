#include <unistd.h> // read, write, close
#include "utils.h"

void call10(const int *socks);

void main(void) {

    int socks[IPN];
    popSocks(socks);
    call10(socks);

    for (int i=0; i < IPN; i++) close(socks[i]);
}
