all: cat 


cat: libhelpers.so
	gcc cat.c -L../lib -I../lib -Wl,-rpath=../lib -lhelpers -o cat

libhelpers.so:
	make -C ../lib/



