sntp:					  *.c *.h 
		gcc  -O3 -static  main.c utils.c call.c -o sntp  -lm
		gcc   -g          main.c utils.c call.c -o debug -lm

clean:
	rm sntp debug *.o
