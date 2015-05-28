all: cat 


cat: libhelpers.so 
	gcc -std=c99 foreach.c -L../lib -I../lib -Wl,-rpath=../lib -lbufio -lhelpers -o foreach

libhelpers.so:
	make -C ../lib/

clean:
	rm -f foreach




