#ifndef BUFIO_H
#define BUFIO_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../lib/helpers.h"

typedef struct buf_t {
	size_t capacity;
	size_t size;
	char* data;
} buf_t;

struct buf_t* buf_new(size_t _capacity);
void buf_free(struct buf_t *);
size_t buf_capacity(buf_t *);
size_t buf_size(buf_t *);
ssize_t buf_fill(int fd, buf_t *buf, size_t required);
ssize_t buf_flush(int fd, buf_t *buf, size_t required);
ssize_t buf_getline(int fd, buf_t *buf, char* dest);

#endif
