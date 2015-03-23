#include "filter.h"
const int BUF_LEN = 4096;

int main(int argc, char* args[]) {
	if (argc == 0)
		return 0;
	char** newargs = (char**)malloc((argc + 1) * sizeof(char**));
	memcpy(newargs, args + 1, (argc - 1) * sizeof(char**));
	char buf[BUF_LEN+1];
	newargs[argc - 1] = buf;
	newargs[argc] = NULL;
	int len;
	while ((len = read_until(STDIN_FILENO, buf, BUF_LEN, '\n')) > 0) {
		buf[len] = 0;
		if (spawn(args[1], newargs) == 0) 
			printf("%s\n", buf);
	}
	free(newargs);

	return 0;
}
