sntp:					  *.c *.h 
		gcc  -O3 -static  main.c utils.c call.c log.c -o sntp  -lm
		gcc   -g          main.c utils.c call.c log.c -o debug -lm

clean:
	rm -f sntp debug
