#ifndef HELPERS_H
#define HELPERS_H
#define _POSIX_SOURCE

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>


ssize_t read_(int fd, void* buf, size_t count);
ssize_t write_(int fd, void* buf, size_t count);
ssize_t read_until(int fd, void* buf, size_t count, char delimiter);
int spawn(const char* file, char* const argv[]);
typedef struct execargs_t {
    char* file;
    char** args;
    int outfd;
    int infd;
} execargs_t;
execargs_t* execargs(char* file, char** args);
int runpiped(execargs_t** programs, size_t n);
#endif
