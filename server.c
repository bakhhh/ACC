#include "wrappers.h"

pthread_mutex_t client_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t client_cond = PTHREAD_COND_INITIALIZER;


void thread_func(void *arg) {
    Thread *threadArgs = (Thread *)arg;
    // int *count = *(threadArgs->clients);
    int connfd_reply = *(threadArgs->connfd_reply);
	int connfd_request = *(threadArgs->connfd_request);
    // free(threadArgs);
    // int connfd = *(int *)(arg);

	//pthread_mutex_lock(&client_mutex); // Lock the mutex before modifying the array


    str_echo(connfd_request,connfd_reply,threadArgs->timer); // Handle communication with the client
    close(connfd_request);
	close(connfd_reply);


    //*(threadArgs->exit) = 1;

	for (int i = 0; i < threadArgs->maxClients; i++) //make space for new client after closing connection 
	{
		if (threadArgs->client[i] == connfd_reply) 
		{
			threadArgs->client[i] = -1; 
			break;
			}
		}

    free(threadArgs->connfd_reply);
    free(threadArgs);
	//pthread_cond_signal(&client_cond);
	//pthread_mutex_unlock(&client_mutex); // Lock the mutex before modifying the array

}


void thread_reply(void *arg)
{
	struct sockaddr_in	servaddr, cliaddr;
	socklen_t		clilen;
	Thread *thread = (Thread *)arg;
	
	int listenfd_reply;
	int connfd_reply;
	int connfd_request =thread->connfd_request;
	int yes =1;
	listenfd_reply = Socket();

	if(setsockopt (listenfd_reply, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
	perror("setsockopt failed");
	exit (1);
}
	int maxClients= thread->maxClients;
	int timeout = thread->timer;
	int * clients= thread->client;
	int i;
	

	bzero(&servaddr, sizeof(servaddr)); 
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(thread->port); /* daytime server */

    Bind(listenfd_reply, (SA*)&servaddr,sizeof(servaddr));
    Listen(listenfd_reply,LISTENQ);



	for ( ; ; ) {
		connfd_reply = Accept(listenfd_reply,(SA*)&cliaddr,&clilen);
		str_echo(connfd_request,connfd_reply,timeout);
		close(connfd_request);
		close(connfd_reply);
		//exit(0);


		// for (i = 0; i < maxClients; i++) {
		// 				if (clients[i] < 0) {
		// 					clients[i] = connfd_reply; /* save descriptor */
		// 					break;
		// 				}
		// 			}
		// 		//pthread_mutex_unlock(&client_mutex); // Unlock the mutex after modifying the array


		// 		if (i==maxClients){

		// 			close(connfd_reply);
		// 			printf("too many clients\n");
		// 		}
		// 		else{
		// 				Thread *threadArgs = (Thread *)malloc(sizeof(Thread));
		// 				int *connfd_arg_reply = (int *)malloc(sizeof(int));
		// 				int *connfd_arg_request = (int *)malloc(sizeof(int));
		// 				*connfd_arg_reply = connfd_reply;
		// 				*connfd_arg_request = connfd_request;
		// 				threadArgs->connfd_reply = connfd_arg_reply;
		// 				threadArgs->connfd_request=connfd_request;
		// 				threadArgs->maxClients = maxClients;
		// 				threadArgs->client = clients;
		// 				threadArgs->timer = timeout;
						
		// 				// pthread_mutex_lock(&client_mutex); // Lock the mutex before modifying the array


		// 				pthread_create(&threadArgs->tid, NULL, (void*)thread_func, (void *)threadArgs);
		// 				pthread_detach(threadArgs->tid);

		// 				//pthread_cond_wait(&client_cond,&client_mutex);

		// 				// pthread_mutex_unlock(&client_mutex); // Unlock the mutex after modifying the array

		// 		}

		// close(connfd_reply);
		// close(connfd_request);

	free(thread);


}
}

int main(int argc, char **argv)
{
	int	listenfd_request, connfd_request, i, maxi, maxfd, nready,n;
	socklen_t		clilen, servlen;
	fd_set			rset, allset;

	struct sockaddr_in	servaddr, cliaddr;
	char			buff[MAXLINE];
	char			line[MAXLINE];
	int yes =1;

	if (argc !=3){
		fprintf(stderr,"Error: ./tcpserver <threadnum> <timeout> &");
		exit(1);
	}

    listenfd_request = Socket();


	if(setsockopt (listenfd_request, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		perror("setsockopt failed");
		exit (1);
	}

	
//initializes the protocol address for this server
	bzero(&servaddr, sizeof(servaddr)); 
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_TCP_PORT); /* daytime server */
//bind the protocol address to the servers socket
    servlen =sizeof(servaddr);
    Bind(listenfd_request, (SA*)&servaddr,servlen);
    Listen(listenfd_request,LISTENQ);
    int maxClients =atoi(argv[1]);
	int timeout =atoi(argv[2]);
	int clients[maxClients];

	for (i = 0; i < maxClients; i++)
		clients[i] = -1;		/* -1 indicates available entry */


	for ( ; ; ) {
		pthread_mutex_lock(&client_mutex);
		connfd_request = Accept(listenfd_request,(SA*)&cliaddr,&clilen);
		int port_number =(SERV_TCP_PORT +1);
		send(connfd_request, &port_number, sizeof(port_number), 0);
		pthread_mutex_unlock(&client_mutex);
		//close(connfd_request);

		Thread *threadArgs= (Thread *)malloc(sizeof(Thread));
		int *connfd_arg_request = (int *)malloc(sizeof(int));
		*connfd_arg_request = connfd_request;
		threadArgs->connfd_request=connfd_request;
		threadArgs->maxClients = maxClients;
		threadArgs->client = clients;
		threadArgs->timer = timeout;
		threadArgs->port = port_number;
					



		pthread_create(&threadArgs->tid, NULL, (void*)thread_reply, (void *)threadArgs);
		pthread_detach(threadArgs->tid);
		// pthread_join(threadArgs->tid,NULL);
		//close(connfd_request);
		

		//pthread_mutex_lock(&client_mutex); // Lock the mutex before modifying the array

		// for (i = 0; i < maxClients; i++) {
		// 		if (clients[i] < 0) {
		// 			clients[i] = connfd; /* save descriptor */
		// 			break;
		// 		}
		// 	}
        // //pthread_mutex_unlock(&client_mutex); // Unlock the mutex after modifying the array


		// 	if (i==maxClients){

		// 		close(connfd);
		// 		printf("too many clients\n");
		// 	}
		// 	else{
		// 			Thread *threadArgs = (Thread *)malloc(sizeof(Thread));
		// 			int *connfd_arg = (int *)malloc(sizeof(int));
		// 			*connfd_arg = connfd;
		// 			threadArgs->connfd = connfd_arg;
		// 			threadArgs->maxClients = maxClients;
		// 			threadArgs->client = clients;
		// 			threadArgs->timer = timeout;
					
		// 			// pthread_mutex_lock(&client_mutex); // Lock the mutex before modifying the array


		// 			pthread_create(&threadArgs->tid, NULL, (void*)thread_func, (void *)threadArgs);
		// 			pthread_detach(threadArgs->tid);

		// 			//pthread_cond_wait(&client_cond,&client_mutex);

       	// 			// pthread_mutex_unlock(&client_mutex); // Unlock the mutex after modifying the array

		// 	}


	}
}

