sntp:					  *.c *.h 
		gcc  -O3  -static main.c utils.c call.c -o sntp  -lm
		gcc   -g  -static main.c utils.c call.c -o debug -lm
		gcc -g cwrap.c -o sntpr

clean:
	rm -f sntp debug
