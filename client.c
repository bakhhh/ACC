#include "wrappers.h"

pthread_mutex_t client_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t client_cond = PTHREAD_COND_INITIALIZER;


void serve_client(void *arg){

	Thread * thread = (Thread*)arg;


	str_cli(stdin, thread->connfd_request,thread->connfd_reply);

	printf("cli exit\n");

	free(thread);
	close(thread->connfd_reply);
	exit(0);



}

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
	int port = atoi(argv[2]);

	int port_number;
		int client_port =50007;




    //server addr
	bzero(&servaddr_request, sizeof(servaddr_request));
	servaddr_request.sin_family = AF_INET;
	servaddr_request.sin_port   = htons(port);	

// 	if (inet_pton(AF_INET, argv[1], &servaddr_request.sin_addr) <= 0){
// 	printf("inet_pton error for %s", argv[1]);
// 	return 1;
// }




	
	



    //Bind(sockfd,(SA *) &cli_addr,sizeof(cli_addr)); //bind the client address to the socket

	pthread_mutex_lock(&client_mutex);
	Connect(sockfd_request,(SA*)&servaddr_request,sizeof(servaddr_request)); // Connect using request socket
	//pthread_mutex_unlock(&client_mutex);
	//send(sockfd_request, &argv[1], sizeof(argv[1]), 0);





	PeerName(sockfd_request,(SA*)&servaddr_request,sizeof(servaddr_request),&servaddr_request);
	SockName(sockfd_request,(SA*)&cli_addr,sizeof(cli_addr),&cli_addr);

	pthread_mutex_unlock(&client_mutex);


		
	int yes =1;
	int listenfd_reply = Socket();
	int connfd_reply;

	if(setsockopt (listenfd_reply, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
	perror("setsockopt failed");
	exit (1);
}
	bzero(&servaddr_reply, sizeof(servaddr_reply)); 
	servaddr_reply.sin_family      = AF_INET;
	servaddr_reply.sin_port        = htons(50007); /* daytime server */
	servaddr_reply.sin_addr.s_addr = htonl(INADDR_ANY);


	Bind(listenfd_reply, (SA*)&servaddr_reply,sizeof(servaddr_reply));
	Listen(listenfd_reply,LISTENQ);
	

	int num;
	//printf("%d\n", num);
	



	for ( ; ; ) {
		clilen = sizeof(cli_addr);
		connfd_reply = Accept(listenfd_reply,(SA*)&cli_addr,&clilen);

		Thread * threadArgs = (Thread*)malloc(sizeof(Thread));

		threadArgs->connfd_request = sockfd_request;
		threadArgs->connfd_reply = connfd_reply;

		pthread_create(&threadArgs->tid, NULL, (void*)serve_client, (void *)threadArgs);
		pthread_detach(threadArgs->tid);
		
		
		
		// str_cli(stdin,sockfd_request,connfd_reply);
		
		// close(connfd_reply);
		// exit(0);
		// printf("client\n");

		// int num =21;
		// int num2;

		//Writen(connfd_reply,&num,sizeof(num));
		//Readline(connfd_reply,&num2,sizeof(num2));
		//printf("Number from server %d\n",num2);

		//close(connfd_reply);

		// Writen(connfd_reply,&connfd_reply,sizeof(connfd_reply));
		// Readline(sockfd_request,&sockfd_reply,sizeof(sockfd_reply));
		



		// recv(sockfd_request, &sockfd_reply, sizeof(sockfd_reply), 0);

		// send(connfd_reply, &connfd_reply, sizeof(connfd_reply), 0);
		// // int num;
		// // recv(listenfd_reply, &num, sizeof(num), 0);

		// // printf("%d\n", num);



		//str_cli(stdin,connfd_reply,sockfd_reply);



		// PeerName(connfd_reply,(SA*)&servaddr_reply,sizeof(servaddr_reply),&servaddr_reply);
		// SockName(connfd_reply,(SA*)&cli_addr,sizeof(cli_addr),&cli_addr);


	}



	// int yes =1;

	// if(setsockopt (sockfd_reply, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
	// 	perror("setsockopt failed");
	// 	exit (1);
	// }

		
	// //Set up server address for reply socket
	// bzero(&servaddr_reply, sizeof(servaddr_reply));
	// servaddr_reply.sin_family = AF_INET;
	// servaddr_reply.sin_port   = htons(port_number);	

    // //client
	// bzero(&cli_addr, sizeof(cli_addr));
	// cli_addr.sin_family = AF_INET;
	// cli_addr.sin_port   = htons(50007);	



	// bind(sockfd_reply, (SA*)&cli_addr,sizeof(cli_addr));
	// // Connect using the request socket
	// Connect(sockfd_reply,(SA*)&servaddr_reply,sizeof(servaddr_reply)); 



	// str_cli(stdin, sockfd_request,sockfd_reply);



	//exit(0);
}
