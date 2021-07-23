#include <string.h> // memset, memcpy
#include <unistd.h> // read, write, close
#include <stdio.h>  // stdout
#include <stdlib.h> // malloc
#include "config.h"
#include "socket.h"
#include "time.h"

char *getAddr(int argc, char **argv);
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

char *getAddr(int argc, char **argv) {

	if (argc < 2) return KW_DEFAULT_NTP_SERVER_IP6;

	char *arg;
	int argl;

	arg  = argv[1];
	argl = strlen(arg);

	
	if (argl < 7 || argl > 39) // "1.2.3.4" is 7 chars; IPv6 max 39 chars
		{ fprintf(stderr, "bad IP length of %d\n", argl); exit(EXIT_FAILURE);}

	if (strstr(arg, ".") == NULL) return arg;

	if (argl > 15) { fprintf(stderr, "bad IPv4 address - too long\n"); exit(EXIT_FAILURE);}

	const char *ip46p = "::FFFF:";
	const int bsz = strlen(ip46p) + 15 + 1;
	char *sbuf = (char *)malloc(bsz);
	sbuf = strcat(sbuf, ip46p);
	sbuf = strcat(sbuf, arg);

	return sbuf;
}
