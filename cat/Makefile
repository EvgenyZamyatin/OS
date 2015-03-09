all: cat 


cat: libhelpers
	gcc -L/home/evgeny/programming/os/lib -Wl,-rpath=/home/evgeny/programming/os/lib -Wall -o cat cat.c -lhelpers

libhelpers:
	make -C ../lib/



