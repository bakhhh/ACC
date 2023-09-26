#include "wrappers.h"

pthread_mutex_t client_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t client_cond = PTHREAD_COND_INITIALIZER;


void thread_func(void *arg) {
    Thread *threadArgs = (Thread *)arg;
    // int *count = *(threadArgs->clients);
    int connfd = *(threadArgs->connfd);
    // free(threadArgs);
    // int connfd = *(int *)(arg);

	//pthread_mutex_lock(&client_mutex); // Lock the mutex before modifying the array


    str_echo(connfd); // Handle communication with the client
    close(connfd);

    //*(threadArgs->exit) = 1;

	for (int i = 0; i < threadArgs->maxClients; i++) {
		if (threadArgs->client[i] == connfd) 
		{
			threadArgs->client[i] = -1; 
			break;
			}
		}

    free(threadArgs->connfd);
    free(threadArgs);
	//pthread_cond_signal(&client_cond);
	//pthread_mutex_unlock(&client_mutex); // Lock the mutex before modifying the array

}

int main(int argc, char **argv)
{
	int	listenfd, connfd, i, maxi, maxfd, sockfd, nready,n;
	socklen_t		clilen, servlen;
	fd_set			rset, allset;

	struct sockaddr_in	servaddr, cliaddr;
	char			buff[MAXLINE];
	char			line[MAXLINE];
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
    int maxClients =atoi(argv[1]);
	int clients[maxClients];

	for (i = 0; i < maxClients; i++)
		clients[i] = -1;		/* -1 indicates available entry */

	// if(setsockopt (listenfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
	// 	perror("setsockopt failed");
	// 	exit (1);
	// }

    // Thread * threadPool[maxClients];

    // for (int i = 0; i < maxClients; i++) {
    //     threadPool[i] = NULL;
    // }


	// for (;;) {
    //     connfd = Accept(listenfd, (SA*)&cliaddr, &clilen);
    //     pthread_mutex_lock(&client_mutex);

    //     int i;
    //     for (i = 0; i < maxClients; i++) {
    //         if (threadPool[i] == NULL) {
    //             Thread* threadArgs = (Thread*)malloc(sizeof(Thread));
    //             threadArgs->connfd = (int*)malloc(sizeof(int));
    //             *(threadArgs->connfd) = connfd;
    //             threadArgs->exit = (int*)malloc(sizeof(int));
    //             *(threadArgs->exit) = 0;

    //             threadPool[i] = threadArgs;

    //             pthread_create(&threadArgs->tid, NULL, thread_func, (void*)threadArgs);
	// 			printf("exit =%d\n",*(threadPool[i]->exit) );
    //             break;
    //         }
    //     }

    //     	pthread_mutex_unlock(&client_mutex);

	// 			if (i == maxClients) {
	// 				close(connfd);
	// 				printf("Too many clients\n");
	// 			}
	// 		}

	// 		// After the main loop, join all threads that are still active
	// 	for (int i = 0; i < maxClients; i++) {
	// 		if (threadPool[i] != NULL) {
	// 			pthread_join(threadPool[i]->tid, NULL);
				
	// 		}
	// 	}

		

	for ( ; ; ) {
		connfd = Accept(listenfd,(SA*)&cliaddr,&clilen);
		//pthread_mutex_lock(&client_mutex); // Lock the mutex before modifying the array

		for (i = 0; i < maxClients; i++) {
				if (clients[i] < 0) {
					clients[i] = connfd; /* save descriptor */
					break;
				}
			}
        //pthread_mutex_unlock(&client_mutex); // Unlock the mutex after modifying the array


			if (i==maxClients){

				close(connfd);
				printf("too many clients\n");
			}
			else{
					Thread *threadArgs = (Thread *)malloc(sizeof(Thread));
					//threadArgs->exit = (int *)malloc(sizeof(int));
					//*(threadArgs->exit) = 0;
					int *connfd_arg = (int *)malloc(sizeof(int));
					*connfd_arg = connfd;
					threadArgs->connfd = connfd_arg;
					threadArgs->maxClients = maxClients;
					threadArgs->client = clients;
					
					// pthread_mutex_lock(&client_mutex); // Lock the mutex before modifying the array


					pthread_create(&threadArgs->tid, NULL, (void*)thread_func, (void *)threadArgs);
					pthread_detach(threadArgs->tid);

					//pthread_cond_wait(&client_cond,&client_mutex);

       				// pthread_mutex_unlock(&client_mutex); // Unlock the mutex after modifying the array


					//printf("exit =%d\n",*(threadArgs->exit) );

					// if (*(threadArgs->exit) == 1){
					// 	for (int i = 0; i < maxClients; i++) {
					// 		if (client[i] == connfd) 
					// 		{
					// 			client[i] = -1; 
					// 			break;
					// 			}
					// 		}

					// 	}
						//free(*(threadArgs->exit));


				// pthread_t tid;
				// int *connfd_arg = (int *)malloc(sizeof(int));
				// *connfd_arg = connfd;
				// pthread_create(&tid, NULL, thread_func, connfd_arg);
				// pthread_detach(tid);



				//pthread_mutex_lock(&client_mutex);
						
				//pthread_mutex_unlock(&client_mutex);
				


			}



		






		// int *connfd_arg = (int *)malloc(sizeof(int));
		// *connfd_arg = connfd;
		// threadArgs->connfd = connfd_arg;
		


		// pthread_create(&threadArgs->tid, NULL, thread_func, (void *)threadArgs);
		// pthread_detach(threadArgs->tid);

		// exit(0);



	}
}

