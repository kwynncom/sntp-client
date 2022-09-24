unsigned long nanotime();
char *getAddr(char *ips);
int getOutboundUDPSock(char *addrStr, int port);
void setOBPack(char *pack);
#define IPN 7
#define M_BILLION 1000000000
#define M_MILLION 1000000
#define SNPL 48 // SNTP packet length
long double Ufl();
#define NISTMaxS 4.0
#define MaxIPL 40

struct sockip {
    char ip[MaxIPL];
    int  sock;
};


void popSocks(struct sockip *socks);
