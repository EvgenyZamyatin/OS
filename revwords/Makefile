all: revwords 


revwords: libhelpers.so
	gcc -std=c99 revwords.c -L../lib -I../lib -Wl,-rpath=../lib -lhelpers -o revwords

libhelpers.so:
	make -C ../lib/

clean:
	rm -f revwords



