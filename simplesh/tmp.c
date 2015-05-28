#include <unistd.h>
#include <stdlib.h>

int main() {
    char buf[256];
    perror("A start"); 
    int len = read(STDIN_FILENO, buf, 256);
    buf[len-1] = '!';
    buf[len] = '\n';
    len++;
    write(STDOUT_FILENO, buf, len);
    perror("A finish");
    while (1);
    exit(0);
    return 0;
}
