#include "../lib/bufio.h"

int main() {
	buf_t *buf = buf_new(4096);
	int now = 1;	
	while ((now = buf_fill(STDIN_FILENO, buf, 1)) > 0) {
		if (buf_flush(STDOUT_FILENO, buf, buf_size(buf)) < 0)
			break; 
	}
    while (buf_size(buf) > 0) {
        if (buf_flush(STDOUT_FILENO, buf, buf_size(buf)) < 0)
            break;
    }
	buf_free(buf);
	return 0;
}
