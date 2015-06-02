#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netdb.h>
#include <helpers.h>
#define check(x) if (x < 0) {perror(#x);exit(EXIT_FAILURE);}
#define BACKLOG 256
#define MAX_FILE_SIZE 4096


void proc(int a, int b) {
    char buf[MAX_FILE_SIZE];
    int n;
    while ((n = read_until(a, buf, MAX_FILE_SIZE, '\n')) > 0) {
        buf[n] = 0;
        printf("Rec from %d: %s", a, buf);
        if (write_(b, buf, n) < 0)
            break;
    }
    close(a);
    close(b);
    exit(EXIT_SUCCESS); 
}

int init_port(char* port) {
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, s;
    struct sockaddr_storage peer_addr;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    
    hints.ai_socktype = SOCK_STREAM; 
    hints.ai_flags = AI_PASSIVE;    
    hints.ai_protocol = 0;        
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    s = getaddrinfo(NULL, port, &hints, &result);
    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype,
                rp->ai_protocol);
        if (sfd == -1)
            continue;
        if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
            break;
        close(sfd);
    }
    
    freeaddrinfo(result);            
    if (rp == NULL)
        return -1;
    return sfd;
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage: ./%s port port\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int sfd[2];
    for (int i = 0; i < 2; ++i) {
        sfd[i] = init_port(argv[i + 1]);
        check(sfd[i]);
    }
    check(listen(sfd[0], BACKLOG)); 
    check(listen(sfd[1], BACKLOG));
    
    while (1) {
        struct sockaddr addr;
        socklen_t slen;
        memset(&addr, 0, sizeof(addr));
        memset(&slen, 0, sizeof(slen));
        int fd[2];
        for (int i = 0; i < 2; ++i) {
            fd[i] = accept(sfd[i], &addr, &slen);
        }
        check(fd[0]);
        check(fd[1]);
        int prc = fork();
        check(prc);
        if (prc == 0) 
            proc(fd[0], fd[1]);  
        prc = fork();
        check(prc);
        if (prc == 0) 
            proc(fd[1], fd[0]);
        close(fd[0]);
        close(fd[1]);
    }
    return 0;
}
