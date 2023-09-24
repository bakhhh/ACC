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



#define	MAXLINE	4096
#define	LISTENQ	1024
#define	SERV_TCP_PORT 50005 
#define	SA struct sockaddr

typedef struct threadArgs{
    pthread_t tid;
    int *clients;
    int *connfd;


}Thread;


int Socket();
void Bind(int fd, const struct sockaddr * sa, socklen_t len);
void Listen(int listenfd, int backlog);
int Accept(int listenfd, const struct sockaddr * sa, socklen_t *len);
void Connect(int sockfd, const struct sockaddr * sa, socklen_t len);
void SockName(int fd, const struct sockaddr * sa, socklen_t len, struct sockaddr_in	* addr);
void PeerName(int fd, const struct sockaddr * sa, socklen_t len, struct sockaddr_in	* addr);
void thread_function(void* arg);
void create_threads(void * arg);
ssize_t Readline(int fd, void *ptr, size_t maxlen);
void Writen(int fd, void *ptr, size_t nbytes);
void str_echo(int sockfd);
void str_cli(FILE *fp, int sockfd);

void *thread_func(void *arg);
