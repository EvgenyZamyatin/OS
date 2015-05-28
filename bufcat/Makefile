all: bufcat 


bufcat: libbufio.so
	gcc -std=c99 bufcat.c -L../lib -I../lib -Wl,-rpath=../lib -lbufio -lhelpers -o bufcat

libbufio.so:
	make -C ../lib/

clean:
	rm -f bufcat



