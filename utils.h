unsigned long nanotime();
char *getAddr(char *ips);
int getOutboundUDPSock(char *addrStr, int port);
void setOBPack(char *pack);
#define IPN 7
#define M_BILLION 1000000000
#define SNPL 48 // SNTP packet length
void popSocks(int *socks);
double Ufl();
#define NISTMaxS 4.0
