tldmonitor: tldmonitor.o date.o tldlist.o
	clang -Wall -Werror -o tldmonitor tldmonitor.o date.o tldlist.o -g

date.o: date.h date.c
	clang -Wall -Werror -o date.o -c date.c -g

tldlist.o: tldlist.h tldlist.c
	clang -Wall -Werror -o tldlist.o -c tldlist.c -g

tldmonitor.o: tldmonitor.c date.h tldlist.h
	clang -Wall -Werror -o tldmonitor.o -c tldmonitor.c -g

clean:
	rm -f *.o tldmonitor