#define KWSNTPV "quota property of sock struct"

#define KWSNTPDLOCKFILE "/var/kwynn/mysd/lock"
#define KWSNTPDEXTGET   "/var/kwynn/mysd/get"
#define KWSNTPDPOKE		"/var/kwynn/mysd/poke"
#define LOGFILE			"/tmp/snl.txt"
#define NISTMaxS 4.0
#define IPN 7

void calllog(const bool newCall, const unsigned long Uus, const bool doClose);

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

struct sockInfo {
    char ip[MAXIPL];
    int  sock;
	bool alwaysQuota;
};


int popSocks(struct sockInfo *socks, char *ip, bool isd);

void procArgs(int argc, char *argv[], bool *isd, bool *usefo, bool *dosleep, bool *qck, char (*ip)[MAXIPL]);

FILE *getLockedFile();

void cleanup();

void mysleep();
