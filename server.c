#include "wrappers.h"



int main(int argc, char **argv)
{
	int			listenfd, connfd;
	socklen_t		clilen, servlen;
	struct sockaddr_in	servaddr, cliaddr;
	char			buff[MAXLINE];
	// int yes =1;

	if (argc !=2){
		fprintf(stderr,"Error: ./tcpserver <threadnum> &");
		exit(1);
	}

    listenfd = Socket();
	
//initializes the protocol address for this server
	bzero(&servaddr, sizeof(servaddr)); 
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_TCP_PORT); /* daytime server */
//bind the protocol address to the servers socket
    servlen =sizeof(servaddr);
    Bind(listenfd, (SA*)&servaddr,servlen);
    Listen(listenfd,LISTENQ);
    int numClients =atoi(argv[1]);
	int *clients =&numClients;
    
	// threadArgs threadArguments = {threads,i,&connfd};
    // create_threads(&threadArguments);
    // thread_function(&threadArguments);

	// if(setsockopt (listenfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
	// 	perror("setsockopt failed");
	// 	exit (1);
	// }



		

	for ( ; ; ) {
		clilen = sizeof(cliaddr);	
		Thread *threadArgs = (Thread *)malloc(sizeof(Thread));
		threadArgs->tid; // Initialize tid if necessary
		threadArgs->clients = clients;	

		if (*threadArgs->clients == 0){
			printf("Ran out of clients to server\n");
			free(threadArgs);
			break;
		}


		if ((connfd = Accept(listenfd,(SA*)&cliaddr,&clilen))< 0 ){
			if (errno == EINTR)  // need this because there is a signal that is sent when the process is blocked in accept().
				continue;
			else
				fprintf(stderr,"accept error");
				return 1;
		}
		




    int *connfd_arg = (int *)malloc(sizeof(int));
    *connfd_arg = connfd;
    threadArgs->connfd = connfd_arg;
    


    pthread_create(&threadArgs->tid, NULL, thread_func, (void *)threadArgs);
    pthread_detach(threadArgs->tid);

		// exit(0);



	}
}

