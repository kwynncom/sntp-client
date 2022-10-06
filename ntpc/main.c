#include <stdio.h> // FILE
#include <stdbool.h>
#include <stdlib.h> // rand
#include <time.h>   // timespec struct
#include "all.h"

void call10(struct sockip *socks, bool isd, bool usefo, bool qckb, int rand1);

void main(int argc, char *argv[]) {

    FILE   *lockf = getLockedFile();
    srand(time(NULL));
	struct sockip socks[IPN];
	bool isd, usefo, dosleep, qck;
	char ip[MAXIPL];
	int rand1;
		
	procArgs(argc, argv, &isd, &usefo, &dosleep, &qck, &ip);
    rand1 = popSocks(socks, ip, isd);
    call10(socks, isd, usefo, qck, rand1);
	cleanup(socks, lockf);
	if (!isd && dosleep) mysleep();
}
