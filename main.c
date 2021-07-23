#include <string.h> // memset, memcpy
#include <unistd.h> // read, write, close
#include <stdio.h>  // stdout
#include <stdlib.h> // malloc
#include "config.h"
#include "utils.h"

void main(int argc, char **argv) {

	const unsigned int snpl = 48; // SNTP packet length

	char pack[snpl];
	unsigned long b, e;
	int sock = getOutboundUDPSock(getAddr(argc, argv), 123);

    memcpy(pack    , "#", 1 ); // see PHP version; ord('#') == 0x23	
	bzero (pack + 1, 47     );
	
	b = nanotime();
	if (write(sock, pack, snpl) != snpl) perror("bad write");
    if (read (sock, pack, snpl) != snpl) perror("bad read");
	e = nanotime();

	fwrite(&pack, snpl, 1, stdout);
	fwrite(&b	, sizeof(b), 1, stdout);
	fwrite(&e	, sizeof(e), 1, stdout);

	close(sock);
}
