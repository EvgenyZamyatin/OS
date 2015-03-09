#include "../lib/helpers.h"
#include <string.h>
#include <stdlib.h>
#define LEN 1024

int main() 
{
	char buf[LEN];
	while (1)
	{
		int red = read_(STDIN_FILENO, buf, LEN); 
		if (red == 0)
			break;
		write_(STDOUT_FILENO, buf, red);
	}
	return 0;
}
