sntp:					  *.c *.h 
		gcc  -O3  -static main.c utils.c call.c -o sntpkw  -lm
		gcc   -g  -static main.c utils.c call.c -o debug   -lm
		gcc -g cwrap.c -o sntpr

clean:
	rm -f sntpkw debug
