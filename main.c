#include <unistd.h> // read, write, close
#include "utils.h"

void call10(struct sockip *socks);

void main(void) {

    struct sockip socks[IPN];
    popSocks(socks);
    call10(socks);

    for (int i=0; i < IPN; i++) close(socks[i].sock);
}
