#define KWSNTPV "10/10 19:18 - usefo === isd"

#define KWSNTPDLOCKFILE "/var/kwynn/mysd/lock"
#define KWSNTPDEXTGET   "/var/kwynn/mysd/get"
#define KWSNTPDPOKE		"/var/kwynn/mysd/poke"
#define LOGFILE			"/tmp/snl.txt"
#define NISTMaxS 4.0
#define IPN 7 // 7 NIST server IPs by default

#define M_BILLION 1000000000
#define M_MILLION 1000000
#define SNPL   48 // SNTP packet length
#define MAXIPL 40 // full IPv6 addr plus \0
#define MINIPL  3 // ::1 is 3 chars

#define TOLERANCENS M_BILLION
#define KWSNTPMINOUTLEN 250

#define KWSNTP_RETURN_FAIL -87;

#include <stdbool.h>

void calllog(const bool newCall, const unsigned long Uus, const bool doClose, FILE *genoutf);
unsigned long nanotime();
bool setAddr(const char *ipin, char *ipout);
int getOutboundUDPSock(const char *addrStr, const int port);
void setOBPack(char *pack);
long double Ufl();

struct sockInfo {
    char iphu[MAXIPL];
	char ip46[MAXIPL];
    int  sock;
	bool alwaysQuota;
};

int popSocks(struct sockInfo *socks, const char *ip, const bool isd);
void procArgs(int argc, char *argv[], bool *isd, bool *dosleep, bool *qck, char (*ip)[MAXIPL]);
FILE *getLockedFile();
void cleanup(const struct sockInfo *socks, const FILE *lockf);
void mysleep();

bool sanityCheck(const unsigned long a, const unsigned long b, const unsigned long c, const unsigned long d);

bool myoutf(const struct timespec bs, const struct timespec es, const char *pack, const char *ip, 
			const bool isd, const bool didSend);

bool qckf();
bool onin(void);
