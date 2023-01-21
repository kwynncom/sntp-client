#include <stdio.h> // FILE
#include <stdbool.h>
#include <stdlib.h> // exit()
#include <time.h>   // timespec struct
#include "all.h"

void call10(const struct sockInfo *socks, const bool isd, const bool qckb);

void main(int argc, char *argv[]) {

    FILE   *lockf = getLockedFile();
	if (lockf == NULL) { fprintf(stderr, "could not open and lock lockfile\n"); exit(150); /* safe to exit because nothing has been done */ }
	struct sockInfo socks[IPN];
	bool isd, dosleep, qck;
	char ip[MAXIPL];
	bool sockr;
		
	procArgs(argc, argv, &isd, &dosleep, &qck, &ip);
    sockr = popSocks(socks, ip, isd);
	if (sockr) call10(socks, isd, qck);
	cleanup(socks, lockf);
	if (!isd && dosleep && sockr) mysleep();
}
