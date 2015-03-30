#include "../lib/bufio.h"

int main() {
	buf_t *buf = buf_new(4096);
	if (buf == NULL)
		return 0;
	int now = 1;
	while ((now = buf_fill(STDIN_FILENO, buf, -1)) > 0) {
		if (buf_flush(STDOUT_FILENO, buf, -1) < 0)
			break; 
	}
	buf_free(buf);
	return 0;
}
