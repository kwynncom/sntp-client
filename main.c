#include "utils.h"

void callSNTPAndWrite(int sock);

void main(void) {

    const char *ips[IPN];
    

    const int sock = getOutboundUDPSock(getAddr("129.6.15.28"), 123);
    callSNTPAndWrite(sock);
    

}
