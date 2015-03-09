all: libhelpers clean

libhelpers:
	gcc -c -fpic helpers.h helpers.c 
	gcc -shared -o libhelpers.so helpers.o

clean:
	rm -f helpers.h.gch
	rm -f helpers.o
