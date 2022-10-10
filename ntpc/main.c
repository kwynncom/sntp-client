#include <stdio.h> // FILE
#include <stdbool.h>
#include <stdlib.h> // rand
#include <time.h>   // timespec struct
#include "all.h"

void call10(const struct sockInfo *socks, const bool isd, const bool usefo, const bool qckb, const int randOne);

void main(int argc, char *argv[]) {

    FILE   *lockf = getLockedFile();
	if (lockf == NULL) { fprintf(stderr, "could not open and lock lockfile\n"); exit(150); /* safe to exit because nothing has been done */ }
    srand(time(NULL));
	struct sockInfo socks[IPN];
	bool isd, usefo, dosleep, qck;
	char ip[MAXIPL];
	int randOne;
		
	procArgs(argc, argv, &isd, &usefo, &dosleep, &qck, &ip);
    randOne = popSocks(socks, ip, isd);
	if (randOne >= 0) call10(socks, isd, usefo, qck, randOne);
	cleanup(socks, lockf);
	if (!isd && dosleep && randOne >= 0) mysleep();
}
