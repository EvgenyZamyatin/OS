#include "bufio.h"
#ifdef DEBUG
#define check(f) if(!(f)) abort();
#else 
#define check(f)  
#endif


struct buf_t* buf_new(size_t capacity) {
	char* data = malloc(capacity);
	if (data == NULL)
		return NULL;
	buf_t* buf = (buf_t*)malloc(sizeof(buf_t));
	if (buf == NULL) {
		free(data);
		return NULL;
	}
	*buf = (buf_t){.capacity = capacity, .size = 0, .data = data};
	return buf;
}

void buf_free(struct buf_t *buf) {
	check(buf != NULL);
	free(buf->data);
	free(buf);
}

size_t buf_capacity(struct buf_t *buf) {
	check(buf != NULL);
	return buf->capacity;
}

size_t buf_size(struct buf_t *buf) {
	check(buf != NULL);
	return buf->size;
}

ssize_t buf_fill(int fd, buf_t *buf, size_t required) {
	if (required == -1)
		required = buf->capacity;
	check(buf != NULL);
	check(required <= buf_capacity(buf));
	buf->size = 0;
	int rad = 0;
	int now = -1;
	while ((rad < required) && now != 0) {
		now = read(fd, buf->data + buf_size(buf), buf_capacity(buf) - buf_size(buf));
		if (now == -1)
			return -1;
		rad += now;
		buf->size += now;
	}
	return rad;
}

ssize_t buf_flush(int fd, buf_t *buf, size_t required) {
	if (required == -1)
		required = buf_size(buf);
	check(buf != NULL);
	check(required <= buf_capacity(buf));
	int wrote = 0;
	int now = -1;
	while (wrote < required && now != 0) {
		now = write(fd, buf->data, buf_size(buf));
		if (now == -1)
			return -1;
		memmove(buf->data, buf->data + now, buf_size(buf) - now);
		buf->size -= now;
		wrote += now;
	}
	return wrote;
}




