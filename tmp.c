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
    if (signal(SIGINT, handler) == SIG_ERR)
        return 0;
    while (1);
    return 0;
}
