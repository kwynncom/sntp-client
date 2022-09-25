#define KWSNTPDLOCKFILE "/tmp/kwsdl"
#define KWSNTPDEXTGET   "/var/kwynn/mysd/get"
#define KWSNTPDPOKE		"/var/kwynn/mysd/poke"
#define TESTIP			"34.193.238.16" // kwynn.com as of early 2022/09
#define NISTMaxS 4.0
#define IPN 7

unsigned long nanotime();
char *getAddr(char *ips);
int getOutboundUDPSock(char *addrStr, int port);
void setOBPack(char *pack);
#define M_BILLION 1000000000
#define M_MILLION 1000000
#define SNPL 48 // SNTP packet length
long double Ufl();
#define MaxIPL 40

struct sockip {
    char ip[MaxIPL];
    int  sock;
};


void popSocks(struct sockip *socks);

void procArgs(int argc, char *argv[], bool *isd, bool *usefo, bool *dosleep, bool *qck);

FILE *getLockedFile();

void cleanup();

void mysleep();
