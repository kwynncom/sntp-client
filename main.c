#include <unistd.h> // read, write, close
#include <sys/file.h>
#include <stdlib.h>
#include <stdio.h>
#include "utils.h"

void call10(struct sockip *socks);

void main(void) {

    FILE   *lockf = fopen("/tmp/kwsdl", "w");
    if (flock(fileno(lockf), LOCK_EX | LOCK_NB) != 0) { perror("fifo output lock failure\n"); exit(28); }
    struct sockip socks[IPN];
    popSocks(socks);

    call10(socks);

    flock(fileno(lockf), LOCK_UN);
    fclose(lockf);
    for (int i=0; i < IPN; i++) close(socks[i].sock);
}
