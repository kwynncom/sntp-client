sntp:					  *.c *.h 
		gcc  -O3 -static  main.c utils.c call.c quota.c onin.c -o sntp  -lm
		gcc   -g          main.c utils.c call.c quota.c onin.c -o debug -lm

clean:
	rm sntp debug
