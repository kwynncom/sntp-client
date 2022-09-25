#include <stdio.h> // FILE
#include <stdbool.h>
#include "utils.h"

void call10(struct sockip *socks, bool isd, bool usefo);

void main(int argc, char *argv[]) {

    FILE   *lockf = getLockedFile();
	struct sockip socks[IPN];
    popSocks(socks);

	bool isd, usefo;
	procArgs(argc, argv, &isd, &usefo);
    call10(socks, isd, usefo);

	cleanup(socks, lockf);

	if (!isd) mysleep();
}
