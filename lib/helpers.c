#include "helpers.h"
#include <stdio.h>

int read_(int fd, const void* buf, size_t count)
{
	int have = 0;
	int rad;
	
	while ((rad = read(fd, (char*)buf + have, count)) > 0) 
	{
		have += rad;
		count -= rad;
	}
	
	return have;
}

int write_(int fd, const void* buf, size_t count)
{
	int have = 0;
	int writed;
	while ((writed = write(fd, (char*)buf + have, count)) > 0) 
	{
		have += writed;
		count -= writed;
	}
	return have;
}