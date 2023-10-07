#include "acc.h"

int Socket()
{
    int listenfd;

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    { // creates socket for server
        fprintf(stderr, "socket creation failed\n");
        exit(1);
    }

    return listenfd;
}

void Bind(int fd, const struct sockaddr *sa, socklen_t len)
{

    if (bind(fd, sa, len) < 0)
    {
        fprintf(stderr, "bind failed: %s\n", strerror(errno)); // Include error message
        exit(1);
    }
}

void Listen(int listenfd, int backlog)
{
    if (listen(listenfd, backlog) < 0)
    {
        fprintf(stderr, "listen failed\n");
        exit(1);
    }
}

int Accept(int listenfd, const struct sockaddr *sa, socklen_t *len)
{
    int connfd;

    if ((connfd = accept(listenfd, sa, len)) < 0)
    {
        fprintf(stderr, "accept failed\n");
        exit(1);
    }

    return connfd;
}

void Connect(int sockfd, const struct sockaddr *sa, socklen_t len)
{

    if (connect(sockfd, sa, len) < 0)
    {
        fprintf(stderr, "connect error: %s\n", strerror(errno));
        exit(1);
    }
}

void SockName(int fd, const struct sockaddr *sa, socklen_t len, struct sockaddr_in *addr,const char *message)
{
    char buff[MAXLINE];

    if (getsockname(fd, sa, &len) < 0)
    {
        printf("getsockname error");
        exit(1);
    }
    else
    {
        inet_ntop(AF_INET, &addr->sin_addr, buff, sizeof(buff));
        printf(message, buff, ntohs(addr->sin_port));
    }
}

void PeerName(int fd, const struct sockaddr *sa, socklen_t len, struct sockaddr_in *addr,const char *message)
{
    char buff[MAXLINE];

    if (getpeername(fd, sa, &len) < 0)
    {
        printf("getpeername error");
        exit(1);
    }
    else
    {
        inet_ntop(AF_INET, &addr->sin_addr, buff, sizeof(buff));
        printf(message, buff, ntohs(addr->sin_port));
    }
}

int Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout)
{
    int n;

    if ((n = select(nfds, readfds, writefds, exceptfds, timeout)) < 0)
    {
        perror("select error");
        exit(EXIT_FAILURE);
    }
    return (n);
}
