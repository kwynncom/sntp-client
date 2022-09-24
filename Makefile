sntp :              main.c utils.c call.c quota.c onin.c
	gcc  -O3    main.c utils.c call.c quota.c onin.c -o sntp -lm

debug :        main.c utils.c call.c quota.c onin.c
	gcc -g main.c utils.c call.c quota.c onin.c -o debug -lm

clean : 
	rm sntp debug

