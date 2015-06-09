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
#include <errno.h>
#include <sys/epoll.h>
#define check(x) if (x < 0) {perror(#x);exit(EXIT_FAILURE);}
#define BACKLOG 256
#define MAX_FILE_SIZE 4096
#define MAX_EVENTS 255

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

//===========TONNEL_T=============
typedef struct tonnel_t {
    int fd;
    int *wlen;
    int *rlen;
    char *writeend;
    char *readend;
    struct tonnel_t* twin;
} tonnel_t;

tonnel_t* tonnel(int fd) {
    tonnel_t* ans = (tonnel_t*)malloc(sizeof(tonnel_t));
    *ans = (tonnel_t) {.fd = fd, .wlen = (int*)malloc(sizeof(int)), 
        .rlen = NULL, .writeend = (char*)malloc(MAX_FILE_SIZE), .readend = NULL, .twin = NULL};
    *(ans->wlen) = 0;
    return ans;
} 

int min(int a, int b) {
    if (a < b)
        return a;
    return b;
}

char buf[MAX_FILE_SIZE];
int read_tonnel(tonnel_t* tonnel) {
    int n = *(tonnel->rlen);
    int n1 = write(tonnel->fd, tonnel->readend, n);
    if (n1 < 0)
        return n1;
    memmove(tonnel->readend, tonnel->readend + n1, *(tonnel->rlen) - n1);
    *(tonnel->rlen) -= n1;
    return n1;
}

int write_tonnel(tonnel_t* tonnel) {
    int n = read(tonnel->fd, tonnel->writeend + *(tonnel->wlen), MAX_FILE_SIZE - *(tonnel->wlen));
    if (n < 0)
        return n;
    *(tonnel->wlen) += n;
    return n;
}

void free_tonnel(tonnel_t* tonnel) {
    free(tonnel->writeend);
    free(tonnel->wlen);
    close(tonnel->fd);
    free(tonnel);
}
//=======================================
//==========CONNECTION_T=================

typedef struct connection_t {
    tonnel_t* a;
    tonnel_t* b;
}connection_t;

connection_t* connection(int cfda, int cfdb) {
    tonnel_t* a = tonnel(cfda);
    tonnel_t* b = tonnel(cfdb);
    a->readend = b->writeend;
    a->rlen = b->wlen;
    b->readend = a->writeend;
    b->rlen = a->wlen;
    a->twin = b;
    b->twin = a;
    connection_t* c = (connection_t*)malloc(sizeof(connection_t));
    *c = (connection_t){.a=a, .b=b};
}

void free_connection(connection_t* c) {
    free_tonnel(c->a);
    free_tonnel(c->b);
    free(c);
}
//========================================

void handle(int epollfd, struct epoll_event *event) {
    if (event->events & EPOLLIN) {
        tonnel_t* t = (tonnel_t*)event->data.ptr;
        printf("%d have data\n", t->fd);
        if (*(t->twin->rlen) == 0) {
            struct epoll_event ev;
            ev.events = EPOLLIN | EPOLLOUT | EPOLLRDHUP;
            ev.data.ptr = t->twin;
            epoll_ctl(epollfd, EPOLL_CTL_MOD, t->twin->fd, &ev);
        }
        write_tonnel(t);
    } 
    if (event->events & EPOLLOUT) {
        tonnel_t* t = (tonnel_t*)event->data.ptr;
        printf("%d ready to out\n", t->fd);
        read_tonnel(t);
        if (*(t->rlen) == 0) {
            struct epoll_event ev;
            ev.events = EPOLLIN | EPOLLRDHUP;
            ev.data.ptr = t;
            epoll_ctl(epollfd, EPOLL_CTL_MOD, t->fd, &ev);
        }
    }
    if (event->events & EPOLLRDHUP) {
        tonnel_t* a = (tonnel_t*)event->data.ptr;
        tonnel_t* b = a->twin;
        read_tonnel(b);
        while (write_tonnel(a))read_tonnel(b);
        printf("%d disconected\n", a->fd); 
        epoll_ctl(epollfd, EPOLL_CTL_DEL, a->fd, NULL);
        epoll_ctl(epollfd, EPOLL_CTL_DEL, b->fd, NULL);
        free_tonnel(a);
        free_tonnel(b);
        return;
    } 
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
    
    int epollfd = epoll_create(MAX_EVENTS);
    check(epollfd);
    struct epoll_event ev, events[MAX_EVENTS];
    
    ev.events = EPOLLIN;
    ev.data.ptr = &sfd[0];
    check(epoll_ctl(epollfd, EPOLL_CTL_ADD, sfd[0], &ev));
    
    ev.events = 0;
    ev.data.ptr = &sfd[1];
    check(epoll_ctl(epollfd, EPOLL_CTL_ADD, sfd[1], &ev));
    int fda = -1 , fdb = -1;
    while (1) {
        printf("====\n");
        int n;
        printf("<wait\n");
        while (((n = epoll_wait(epollfd, events, MAX_EVENTS, -1)) == -1) 
                && (errno == EINTR));
        printf("wait>\n");
        check(n);
        for (int i = 0; i < n; ++i) {
            if (events[i].data.ptr == &sfd[0]) {
                fda = accept(sfd[0], NULL, NULL);
                ev.events = 0;
                ev.data.ptr = &sfd[0];
                epoll_ctl(epollfd, EPOLL_CTL_MOD, sfd[0], &ev);
                ev.events = EPOLLIN;
                ev.data.ptr = &sfd[1];
                epoll_ctl(epollfd, EPOLL_CTL_MOD, sfd[1], &ev);   
                printf("1) Accepted %d\n", fda); 
            } else if (events[i].data.ptr == &sfd[1]) {
                fdb = accept(sfd[1], NULL, NULL);
                ev.events = 0;
                ev.data.ptr = &sfd[1];
                epoll_ctl(epollfd, EPOLL_CTL_MOD, sfd[1], &ev);
                ev.events = EPOLLIN;
                ev.data.ptr = &sfd[0];
                epoll_ctl(epollfd, EPOLL_CTL_MOD, sfd[0], &ev);    
                printf("2) Accepted %d\n", fdb);
            } else {
                handle(epollfd, &events[i]);
            } 
        } 
        if (fda != -1 && fdb != -1) {
            connection_t* conn = connection(fda, fdb);
            ev.events = EPOLLIN | EPOLLRDHUP;
            ev.data.ptr = conn->a;
            epoll_ctl(epollfd, EPOLL_CTL_ADD, conn->a->fd, &ev);
            ev.events = EPOLLIN | EPOLLRDHUP;
            ev.data.ptr = conn->b;
            epoll_ctl(epollfd, EPOLL_CTL_ADD, conn->b->fd, &ev);
            free(conn);
            fda = fdb = -1;
        }
    }

    return 0;
}
