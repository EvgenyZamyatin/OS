#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<signal.h>

void handler(int sig) {
    printf ("Catched\n");
}

int main ()
{
    if (signal(SIGQUIT, handler) == SIG_ERR)
        return 0;
    while (1);
    int pipefd[2];
    pipe(pipefd);
    write(pipefd[1], "hello\n", 6);
    //close(pipefd[1]);
    int p;
    if ((p = fork()) == 0) {
        //char buf[256];
        //int len; 
        //while ((len = read(pipefd[0], buf, 256)) > 0) 
        //    write(STDOUT_FILENO, buf, len);
        dup2(pipefd[0], STDIN_FILENO);
        char* tmp[2];
        tmp[0] = "cat";
        tmp[1] = 0;
        execvp("cat", tmp);
    } else {
        wait(NULL);
    }
    return 0;
}
