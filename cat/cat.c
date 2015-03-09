#include "../lib/helpers.h"
#include <string.h>
#include <stdlib.h>

const int LEN = 1024;

int main() 
{
	while (1)
	{
		void* buf = malloc(LEN);
		int have = 0;
		int red;
		while((red = read_(STDIN_FILENO, (char*)buf + have, LEN)) == LEN && red != 0) 
		{
			have += red;
			void* nbuf = malloc(have);
			memcpy(nbuf, buf, have);
			free(buf);
			buf = nbuf;
		}
		have += red;
		write_(STDOUT_FILENO, buf, have);
		free(buf);
	}
	return 0;
}
