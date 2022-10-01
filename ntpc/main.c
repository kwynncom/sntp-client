#include <stdio.h> // FILE
#include <stdbool.h>
#include "utils.h"

void call10(struct sockip *socks, bool isd, bool usefo, bool qckb);

void main(int argc, char *argv[]) {

    FILE   *lockf = getLockedFile();
	struct sockip socks[IPN];
	bool isd, usefo, dosleep, qck;
	char ip[MAXIPL];
		
	procArgs(argc, argv, &isd, &usefo, &dosleep, &qck, &ip);
    popSocks(socks, ip);
    call10(socks, isd, usefo, qck);
	cleanup(socks, lockf);
	if (!isd && dosleep) mysleep();
}
