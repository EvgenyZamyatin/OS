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

int main(int argc, char *argv[])
{
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, s;
    struct sockaddr_storage peer_addr;
    socklen_t peer_addr_len;
    ssize_t nread;
    if (argc != 3) {
        fprintf(stderr, "Usage: ./%s port file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int filefd = open(argv[2], O_RDONLY);
    check(filefd);
    char file[MAX_FILE_SIZE];
    int filelen = read_(filefd, file, MAX_FILE_SIZE);
    if (filelen == MAX_FILE_SIZE) {
        fprintf(stderr, "File too big.\n");
        exit(EXIT_FAILURE);
    }
    close(filefd);

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    
    hints.ai_socktype = SOCK_STREAM; 
    hints.ai_flags = AI_PASSIVE;    
    hints.ai_protocol = 0;        
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    s = getaddrinfo(NULL, argv[1], &hints, &result);
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

    if (rp == NULL) {               
        fprintf(stderr, "Could not bind\n");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(result);           
    check(listen(sfd, BACKLOG));

    while (1) {
        int accept(int s, struct sockaddr *addr, socklen_t *addrlen);
        struct sockaddr addr;
        socklen_t slen;
        memset(&addr, 0, sizeof(addr));
        memset(&slen, 0, sizeof(slen));
        int fd = accept(sfd, &addr, &slen);
        check(fd);
        int prc = fork();
        check(prc);
        if (prc == 0) {
            write_(fd, file, filelen);
            close(fd);
            exit(EXIT_SUCCESS);
        } else {
            close(fd);
        }        
    }
    return 0;
}
