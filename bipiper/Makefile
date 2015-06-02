all: forking

forking: libhelpers.so
	gcc -std=c11 -D_POSIX_C_SOURCE=200112L forking.c -L../lib -I../lib -Wl,-rpath=../lib -lbufio -lhelpers -o forking

libhelpers.so:
	make -C ../lib/

clean:
	rm -f forking


