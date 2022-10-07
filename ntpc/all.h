#define KWSNTPV "23:29 - rearranging output 2"

#define KWSNTPDLOCKFILE "/var/kwynn/mysd/lock"
#define KWSNTPDEXTGET   "/var/kwynn/mysd/get"
#define KWSNTPDPOKE		"/var/kwynn/mysd/poke"
#define LOGFILE			"/tmp/snl.txt"
#define NISTMaxS 4.0
#define IPN 7 // 7 NIST server IPs by default

#define M_BILLION 1000000000
#define M_MILLION 1000000
#define SNPL   48 // SNTP packet length
#define MAXIPL 40 // full IPv6 addr
#define MINIPL  3 // ::1 is 3 chars

#define TOLERANCENS M_BILLION


void calllog(const bool newCall, const unsigned long Uus, const bool doClose, FILE *genoutf);
unsigned long nanotime();
char *getAddr(const char *ips);
int getOutboundUDPSock(const char *addrStr, const int port);
void setOBPack(char *pack);
long double Ufl();

struct sockInfo {
    char ip[MAXIPL];
    int  sock;
	bool alwaysQuota;
};

int popSocks(struct sockInfo *socks, const char *ip, const bool isd);
void procArgs(int argc, char *argv[], bool *isd, bool *usefo, bool *dosleep, bool *qck, char (*ip)[MAXIPL]);
FILE *getLockedFile();
void cleanup(const struct sockInfo *socks, const FILE *lockf);
void mysleep();

bool sanityCheck(const unsigned long a, const unsigned long b, const unsigned long c, const unsigned long d);

void output(const struct timespec bs, const struct timespec es, const char *pack, const char *ip, 
			const bool isd, const bool usefo, const bool didSend);

bool qckf();
bool onin(void);
