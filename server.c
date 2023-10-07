#include "acc.h"

pthread_mutex_t client_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t client_cond = PTHREAD_COND_INITIALIZER;

void thread_reply(void *arg)
{

	struct sockaddr_in servaddr_reply, cli_addr;

	Thread *thread = (Thread *)arg;
	int yes = 1;
	int port = thread->port;
	int sockfd_reply = Socket();

	if (setsockopt(sockfd_reply, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
	{
		perror("setsockopt failed");
		exit(1);
	}

	char *ip = thread->ip;

	bzero(&servaddr_reply, sizeof(servaddr_reply));
	servaddr_reply.sin_family = AF_INET;
	servaddr_reply.sin_port = htons(port);

	bzero(&cli_addr, sizeof(cli_addr));
	cli_addr.sin_family = AF_INET;
	cli_addr.sin_port = htons(SERV_TCP_PORT + 1);

	if (inet_pton(AF_INET, ip, &servaddr_reply.sin_addr) <= 0)
	{
		printf("inet_pton error for %s", ip);
		return;
	}

	Bind(sockfd_reply, (SA *)&cli_addr, sizeof(cli_addr));

	Connect(sockfd_reply, (SA *)&servaddr_reply, sizeof(servaddr_reply));

	PeerName(sockfd_reply, (SA *)&servaddr_reply, sizeof(servaddr_reply), &servaddr_reply);
	SockName(sockfd_reply, (SA *)&cli_addr, sizeof(cli_addr), &cli_addr);

	int connfd_request = thread->connfd_request;

	str_echo(connfd_request, sockfd_reply, thread->timer); // Handle communication with the client
	close(connfd_request);
	close(sockfd_reply);

	for (int i = 0; i < thread->maxClients; i++) // make space for new client after closing connection
	{
		if (thread->client[i] == connfd_request)
		{
			thread->client[i] = -1;
			break;
		}
	}

	free(thread);
}

int main(int argc, char **argv)
{
	int listenfd_request, connfd_request, i, maxi, maxfd, nready, n;
	socklen_t clilen, servlen;
	fd_set rset, allset;

	struct sockaddr_in servaddr, cliaddr;
	char buff[MAXLINE];
	char line[MAXLINE];
	int yes = 1;
	int maxClients = atoi(argv[1]);
	int timeout = atoi(argv[2]);
	int clients[maxClients];

	if (argc != 3)
	{
		fprintf(stderr, "Error: ./tcpserver <number of clients> <timeout> &");
		exit(1);
	}

	if (maxClients > 10)
	{
		printf("\nMax client is 10");
		exit(1);
	}

	if (timeout > 120)
	{
		printf("\nMax timer is 120");
		exit(1);
	}

	listenfd_request = Socket();

	if (setsockopt(listenfd_request, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
	{
		perror("setsockopt failed");
		exit(1);
	}

	// initializes the protocol address for this server
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_TCP_PORT); /* daytime server */
											  // bind the protocol address to the servers socket
	servlen = sizeof(servaddr);
	Bind(listenfd_request, (SA *)&servaddr, servlen);
	Listen(listenfd_request, LISTENQ);

	for (i = 0; i < maxClients; i++)
		clients[i] = -1; /* -1 indicates available entry */

	for (;;)
	{
		clilen = sizeof(cliaddr);
		connfd_request = Accept(listenfd_request, (SA *)&cliaddr, &clilen);
		char *client_ip = inet_ntoa(cliaddr.sin_addr);
		int port_number;
		recv(connfd_request, &port_number, sizeof(port_number), 0);

		for (i = 0; i < maxClients; i++)
		{
			if (clients[i] < 0)
			{
				clients[i] = connfd_request; // save descriptor //
				break;
			}
		}

		if (i == maxClients)
		{
			const char *message = "Too many clients please wait until there is space\n";
			send(connfd_request, message, strlen(message), 0);
			close(connfd_request);
		}
		else
		{

			Thread *threadArgs = (Thread *)malloc(sizeof(Thread));
			int *connfd_arg_request = (int *)malloc(sizeof(int));
			*connfd_arg_request = connfd_request;
			threadArgs->connfd_request = connfd_request;
			threadArgs->maxClients = maxClients;
			threadArgs->client = clients;
			threadArgs->timer = timeout;
			threadArgs->port = port_number;
			threadArgs->ip = client_ip;
			pthread_create(&threadArgs->tid, NULL, (void *)thread_reply, (void *)threadArgs);
			pthread_detach(threadArgs->tid);
		}
	}
}
