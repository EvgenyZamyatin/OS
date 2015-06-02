all: filesender

filesender: libhelpers.so
	gcc -std=c11 -D_POSIX_C_SOURCE=200112L filesender.c -L../lib -I../lib -Wl,-rpath=../lib -lbufio -lhelpers -o filesender

libhelpers.so:
	make -C ../lib/

clean:
	rm -f filesender


