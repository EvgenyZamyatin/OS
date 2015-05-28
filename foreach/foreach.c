#include "foreach.h"
const int BUF_LEN = 4096;

int main(int argc, char* args[]) {
	if (argc == 0)
		return 0;
	char** newargs = (char**)malloc((argc + 1) * sizeof(char**));
	memcpy(newargs, args + 1, (argc - 1) * sizeof(char**));
	char buf[BUF_LEN];
	newargs[argc - 1] = buf;
	newargs[argc] = NULL;
	int len;
	buf_t *b = buf_new(BUF_LEN);
	while ((len = buf_getline(STDIN_FILENO, b, buf)) > 0) {	
		buf[len-1] = 0;
		if (strlen(buf) % 2 != 0)
			continue;
		if (spawn(args[1], newargs) == 0) {
			write_(STDOUT_FILENO, buf, len);
			buf[0] = '\n';
			write_(STDOUT_FILENO, buf, 1);
		}
	}
	free(newargs);
	return 0;
}
