#include "../lib/helpers.h"
#include <string.h>
#include <stdlib.h>

const int LEN = 4096;



int main() 
{
	void* buf = malloc(LEN+1);
	memset(buf, 0, LEN);
	int have = 0;
	int red;
	while((red = read_until(STDIN_FILENO, (char*)buf, LEN, ' ')) != 0) 
	{	
		((char*)buf)[red] = 0;
		sprintf(buf, "%d", (int)strlen(buf)-1);
		write_(STDOUT_FILENO, buf, strlen(buf));
	}

	((char*)buf)[0] = '\n';
	((char*)buf)[1] = 0;
	write_(STDOUT_FILENO, buf, strlen(buf));
	free(buf);
	return 0;
}
