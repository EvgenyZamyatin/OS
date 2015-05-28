all: libhelpers bufio clean

libhelpers:
	gcc -std=c99 -c -fpic helpers.h helpers.c 
	gcc -std=c99 -shared -o libhelpers.so helpers.o

bufio:
	gcc -std=c99 -D DEBUG -c -fpic bufio.h bufio.c 
	gcc -std=c99 -shared -o libbufio.so bufio.o
clean:
	rm -f helpers.h.gch
	rm -f helpers.o
	rm -f bufio.h.gch
	rm -f bufio.o
	rm -f libbufio.so
	rm -f libhelpers.so

