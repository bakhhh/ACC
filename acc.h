#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <sys/uio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <sys/select.h>
#include <sys/time.h>
#define	MAXLINE	4096
#define	LISTENQ	1024
#define	SERV_TCP_PORT 50005 
#define	SA struct sockaddr

typedef struct threadArgs{
    pthread_t tid;
    int timer;
    int maxClients;
    int *client;
    int *connfd_request;
    int *connfd_reply;
    int port;
    char* ip;


}Thread;



int Socket();
void Bind(int fd, const struct sockaddr * sa, socklen_t len);
void Listen(int listenfd, int backlog);
int Accept(int listenfd, const struct sockaddr * sa, socklen_t *len);
void Connect(int sockfd, const struct sockaddr * sa, socklen_t len);
void SockName(int fd, const struct sockaddr * sa, socklen_t len, struct sockaddr_in	* addr,const char *message);
void PeerName(int fd, const struct sockaddr * sa, socklen_t len, struct sockaddr_in	* addr,const char *message);
int Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);


ssize_t Readline(int fd, void *ptr, size_t maxlen);
void Writen(int fd, void *ptr, size_t nbytes);

int ADD(int x, int y);
int MUL(int x, int y);
float DIV(float x, float y);
int MOD(int x, int y);
void INFO(char *line);
void calculate(char *line);


void str_echo(int sockfd_request,int sockfd_reply,int time);
void str_cli(FILE *fp, int sockfd_request, int sockfd_reply);

