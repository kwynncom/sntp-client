# sntp-client

SNTP (network time) client - gets your time relative to the US NIST time servers.

REQUIREMENTS - gcc

BASIC USAGE

make
./sntp
1664088521955087266
1664088522112967429
1664088522112968686
1664088522202237553
129.6.15.30
Sleeping for 4 seconds.  Zzzzz....

First is 4 timestamps, the UNIX Epoch timestamp in nanoseconds.
Your outgoing time.
NIST's receipt time
NIST's send time
your receive time.
Then, the IP address of the server.

I sleep for 4 seconds because NIST forbids more frequent usage.

There is a formula of your clock's error from those numbers.  I process the numbers with 
the PHP language or other.  That's in other repos.  

NOTES

SNTP - simple network time protocol
https://tf.nist.gov/tf-cgi/servers.cgi
Specifically, I'm using the 7 NIST servers in Gaithersburg, Maryland, USA.

CREDITS

https://zetcode.com/articles/cdatetime/ - Human-readable time for log.


HISTORY

As of Sep 23, 2022, ./original/README.md existed.  It probably has interesting stuff.  

The original version didn't assume NIST and didn't have a daemon version.  I may delete 
that directory at any time.

Before 2022/09/25 00:01 ET, I had "notes.txt" that included information on JSON and base64 
in C.  That's before I decided to decode the SNTP packet in C.
