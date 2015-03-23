all: lenwords 


lenwords: libhelpers.so
	gcc -std=c99 lenwords.c -L../lib -I../lib -Wl,-rpath=../lib -lhelpers -o lenwords

libhelpers.so:
	make -C ../lib/



