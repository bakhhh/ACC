#include "wrappers.h"

pthread_mutex_t client_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t client_cond = PTHREAD_COND_INITIALIZER;



int main(int argc, char **argv)
{
	int	sockfd,sockfd_request,sockfd_reply;
	struct sockaddr_in	servaddr_request,servaddr_reply, cli_addr;
	socklen_t		clilen, servlen;


	if (argc != 3) {
		printf("usage: ./tcpclient <IPaddress> <port>");
		return 1;
	}

    sockfd_request = Socket();
	sockfd_reply =Socket();
	int port = atoi(argv[2]);

	int port_number;



    //server addr
	bzero(&servaddr_request, sizeof(servaddr_request));
	servaddr_request.sin_family = AF_INET;
	servaddr_request.sin_port   = htons(port);	

	if (inet_pton(AF_INET, argv[1], &servaddr_request.sin_addr) <= 0){
	printf("inet_pton error for %s", argv[1]);
	return 1;
}




	
	



    //Bind(sockfd,(SA *) &cli_addr,sizeof(cli_addr)); //bind the client address to the socket

	pthread_mutex_lock(&client_mutex);
	Connect(sockfd_request,(SA*)&servaddr_request,sizeof(servaddr_request)); // Connect using request socket
	//pthread_mutex_unlock(&client_mutex);




	PeerName(sockfd_request,(SA*)&servaddr_request,sizeof(servaddr_request),&servaddr_request);
	SockName(sockfd_request,(SA*)&cli_addr,sizeof(cli_addr),&cli_addr);

	recv(sockfd_request, &port_number, sizeof(port_number), 0);
	pthread_mutex_unlock(&client_mutex);



	int yes =1;

	if(setsockopt (sockfd_reply, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		perror("setsockopt failed");
		exit (1);
	}

		
	//Set up server address for reply socket
	bzero(&servaddr_reply, sizeof(servaddr_reply));
	servaddr_reply.sin_family = AF_INET;
	servaddr_reply.sin_port   = htons(port_number);	

    //client
	bzero(&cli_addr, sizeof(cli_addr));
	cli_addr.sin_family = AF_INET;
	cli_addr.sin_port   = htons(50007);	



	bind(sockfd_reply, (SA*)&cli_addr,sizeof(cli_addr));
	// Connect using the request socket
	Connect(sockfd_reply,(SA*)&servaddr_reply,sizeof(servaddr_reply)); 

	PeerName(sockfd_reply,(SA*)&servaddr_reply,sizeof(servaddr_reply),&servaddr_reply);
	SockName(sockfd_reply,(SA*)&cli_addr,sizeof(cli_addr),&cli_addr);

	str_cli(stdin, sockfd_request,sockfd_reply);



	exit(0);
}
