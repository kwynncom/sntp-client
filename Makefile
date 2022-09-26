sntpkw2:					  *.c *.h 
		gcc  -O3 -static  main.c utils.c call.c -o sntpkw2  -lm
		gcc   -g          main.c utils.c call.c -o debug -lm

clean:
	rm -f sntp debug *.o sntpkw2
