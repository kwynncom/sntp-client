#define KWSNTPV "Uus as cached time"
#define KWSNTPDLOCKFILE "/var/kwynn/mysd/lock"
#define KWSNTPDEXTGET   "/var/kwynn/mysd/get"
#define KWSNTPDPOKE		"/var/kwynn/mysd/poke"
#define LOGFILE			"/tmp/snl.txt"
#define TESTIP			"2600:1f18:23ab:9500:7a93:a206:f823:20c3" // kwynn.com as of early 2022/09
#define NISTMaxS 4.0
#define IPN 7

void calllog(bool doClose, bool toFile, bool newCall, unsigned long Uus);

unsigned long nanotime();
char *getAddr(char *ips);
int getOutboundUDPSock(char *addrStr, int port);
void setOBPack(char *pack);
#define M_BILLION 1000000000
#define M_MILLION 1000000
#define SNPL 48 // SNTP packet length
long double Ufl();
#define MAXIPL 40
#define MINIPL  3

struct sockip {
    char ip[MAXIPL];
    int  sock;
};


int popSocks(struct sockip *socks, char *ip, bool isd);

void procArgs(int argc, char *argv[], bool *isd, bool *usefo, bool *dosleep, bool *qck, char (*ip)[MAXIPL]);

FILE *getLockedFile();

void cleanup();

void mysleep();
