all: cat 


cat: libhelpers.so
	gcc -std=c99 filter.c -L../lib -I../lib -Wl,-rpath=../lib -lhelpers -o filter

libhelpers.so:
	make -C ../lib/



