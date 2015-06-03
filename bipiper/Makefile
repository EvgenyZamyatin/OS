all: forking polling

forking: libhelpers.so
	gcc -std=c11 -D_POSIX_C_SOURCE=200112L forking.c -L../lib -I../lib -Wl,-rpath=../lib -lbufio -lhelpers -o forking

polling: libhelpers.so
	gcc -std=c11 -D_POSIX_C_SOURCE=200112L polling.c -L../lib -I../lib -Wl,-rpath=../lib -lbufio -lhelpers -o polling



libhelpers.so:
	make -C ../lib/

clean:
	rm -f forking
	rm -f polling


