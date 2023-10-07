#include "acc.h"

pthread_mutex_t client_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t client_cond = PTHREAD_COND_INITIALIZER;

void serve_client(void *arg)
{
	Thread *thread = (Thread *)arg;
	str_cli(stdin, thread->connfd_request, thread->connfd_reply);
	free(thread);
	close(thread->connfd_reply);
	exit(0);
}

int main(int argc, char **argv)
{
	int sockfd, sockfd_request, sockfd_reply;
	struct sockaddr_in servaddr_request, servaddr_reply, cli_addr;
	socklen_t clilen, servlen;
	int connfd_reply;

	if (argc != 3)
	{
		printf("usage: ./tcpclient <IPaddress> <port>");
		return 1;
	}

	sockfd_request = Socket();
	int yes = 1;
	int yes2 = 1;

	if (setsockopt(sockfd_request, SOL_SOCKET, SO_REUSEADDR, &yes2, sizeof(int)) == -1)
	{
		perror("setsockopt failed");
		exit(1);
	}
	int port_L = atoi(argv[2]);
	int port_U = 50007;

	// server addr
	bzero(&servaddr_request, sizeof(servaddr_request));
	servaddr_request.sin_family = AF_INET;
	servaddr_request.sin_port = htons(port_L);

	if (inet_pton(AF_INET, argv[1], &servaddr_request.sin_addr) <= 0)
	{
		printf("inet_pton error for %s", argv[1]);
		return 1;
	}

	Connect(sockfd_request, (SA *)&servaddr_request, sizeof(servaddr_request)); // Connect using request socket

	PeerName(sockfd_request, (SA *)&servaddr_request, sizeof(servaddr_request), &servaddr_request,"\nRequest line server on %s, port %d\n");
	SockName(sockfd_request, (SA *)&cli_addr, sizeof(cli_addr), &cli_addr,"\nClient connecting to request Line from %s, port %d\n");

	
	int listenfd_reply = Socket();

	send(sockfd_request, &port_U, sizeof(port_U), 0);

	if (setsockopt(listenfd_reply, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
	{
		perror("setsockopt failed");
		exit(1);
	}
	bzero(&servaddr_reply, sizeof(servaddr_reply));
	servaddr_reply.sin_family = AF_INET;
	servaddr_reply.sin_port = htons(port_U); /* daytime server */
	servaddr_reply.sin_addr.s_addr = htonl(INADDR_ANY);

	Bind(listenfd_reply, (SA *)&servaddr_reply, sizeof(servaddr_reply));
	Listen(listenfd_reply, LISTENQ);


	char buffer[MAXLINE];
	fd_set readfds;
	struct timeval timeout;

	timeout.tv_sec = 0;
	timeout.tv_usec = 1000;

	FD_ZERO(&readfds);
	FD_SET(sockfd_request, &readfds);
	int ready = Select(sockfd_request + 1, &readfds, NULL, NULL, &timeout);

	if (ready == 0)
	{
		goto skipread;
	}
	else
	{
		ssize_t n = recv(sockfd_request, buffer, sizeof(buffer), 0);
		buffer[n] = '\0';	  
		printf("%s", buffer); 

		if (strcmp(buffer, "Too many clients please wait until there is space\n") == 0)
		{
			close(sockfd_request);
			exit(0);
		}
	}
skipread:

	for (;;)
	{
		clilen = sizeof(cli_addr);
		connfd_reply = Accept(listenfd_reply, (SA *)&cli_addr, &clilen);
		Thread *threadArgs = (Thread *)malloc(sizeof(Thread));
		threadArgs->connfd_request = sockfd_request;
		threadArgs->connfd_reply = connfd_reply;

		pthread_create(&threadArgs->tid, NULL, (void *)serve_client, (void *)threadArgs);
		pthread_detach(threadArgs->tid);
	}
}
