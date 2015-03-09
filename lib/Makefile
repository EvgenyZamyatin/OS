all: libhelpers clean

libhelpers:
	gcc -std=c99 -c -fpic helpers.h helpers.c 
	gcc -std=c99 -shared -o libhelpers.so helpers.o

clean:
	rm -f helpers.h.gch
	rm -f helpers.o
