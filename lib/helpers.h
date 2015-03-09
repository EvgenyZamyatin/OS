#ifndef HELPERS_H
#define HELPERS_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


ssize_t read_(int fd, void* buf, size_t count);
ssize_t write_(int fd, void* buf, size_t count);
ssize_t read_until(int fd, void* buf, size_t count, char delimiter);

#endif
