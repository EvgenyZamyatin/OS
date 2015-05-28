#include <helpers.h>
#include <bufio.h>
#include <string.h>

int ewrite(int fd, char* buf, int len) {
    int n;
    while ((n = write(fd, buf, len) > 0)) {
        len -= n;
        buf += n;
    }
    if (n != 0)
        exit(0);
    return 0;
}

int eread(int fd, char* buf, int len) {
    int n;
    int sum = 0;
    while ((n = read(fd, buf, len)) > 0) {
        len -= n;
        buf += n;
        sum += n;
    }
    if (n != 0)
        exit(0);
    return sum;
}

execargs_t* parseprog(char* begin, char* end) {
    char* name;
    char** args = malloc(256);
    name = strtok(begin, " ");
    int n = 0;
    args[0] = name;
    n = 1;
    while ((args[n++] = strtok(NULL, " ")) != NULL); 
    return execargs(name, args);
}

void getexecargs(char* str, int len, execargs_t** args, int *n) {
    int last = 0;
    *n = 0;
    for (int i = 0; i < len; ++i) {
        if (str[i] == '|') {
            str[i] = 0;
            args[(*n)++] = parseprog(str + last, str + i);
            last = i + 1;
        }
    }
    if (last != len)
        args[(*n)++] = parseprog(str + last, str + len);
}

void print(execargs_t** ea, int n) {
    for (int i = 0; i < n; ++i) {
        printf("File: %s, ", ea[i]->file);
        printf("Args: ");
        for (int j = 0; ea[i]->args[j] != NULL; ++j) {
            printf("%s, ", ea[i]->args[j]);
        }
        printf("\n====\n");
    }
}

buf_t* iobuf;

void sigquit_handler(int sig) {
    //printf ("SIGINT catched\n");
    exit(0);
}

int main() {
    if (signal(SIGQUIT, sigquit_handler) == SIG_ERR) {
        printf("Can't bind handler to SIGQUIT");
    }
    char buf[4096];
    iobuf = buf_new(4096);
    while (1) {
        ewrite(STDOUT_FILENO, "$", 1);
        int len = buf_getline(STDIN_FILENO, iobuf, buf);
        buf[len-1] = 0;
        execargs_t* ea[256];
        int n;
        getexecargs(buf, len-1, ea, &n);
        if (n > 0 && runpiped(ea, n) < 0)
            break;
        for (int i = 0; i < n; ++i) {
            free(ea[i]->args);
            free(ea[i]); 
        }
    }
    perror("shell crashed");
    return 0;
}
