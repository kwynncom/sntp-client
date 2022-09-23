#include <unistd.h> // read, write, close
#include "utils.h"

void call10(int sock);

void main(void) {

    const char *ips[IPN];
    const int sock = getOutboundUDPSock(getAddr("129.6.15.28"), 123);
    call10(sock);
    close(sock);
}
