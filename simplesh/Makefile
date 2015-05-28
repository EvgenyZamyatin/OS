all: simplesh.c

simplesh.c: libhelpers.so
	gcc -std=c99 simplesh.c -L../lib -I../lib -Wl,-rpath=../lib -lbufio -lhelpers -o simplesh

libhelpers.so:
	make -C ../lib/

clean:
	rm -f simplesh



