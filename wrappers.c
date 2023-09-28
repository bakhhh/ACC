#include "wrappers.h"



int Socket(){
    int listenfd;

	if ( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ){ //creates socket for server
		fprintf(stderr, "socket creation failed\n");
		exit (1);
	}
    
    return listenfd;
    
}


void Bind(int fd, const struct sockaddr * sa, socklen_t len){
    
    if (bind(fd, sa, len) < 0) {
        fprintf(stderr, "bind failed: %s\n", strerror(errno)); // Include error message
        exit(1);
    }
}

		
void Listen(int listenfd, int backlog){
	if ( listen(listenfd, backlog) < 0) {
		fprintf(stderr, "listen failed\n");
		exit (1);
	}
}

int Accept(int listenfd, const struct sockaddr * sa, socklen_t *len){
        int connfd;

		if ( (connfd = accept(listenfd, sa, len)) < 0 ) 
        {
			fprintf(stderr, "accept failed\n");
			exit (1);
		}
        
        return connfd;
        
}
	


void Connect(int sockfd, const struct sockaddr * sa, socklen_t len){

	if (connect(sockfd, sa, len) < 0) {
		fprintf(stderr, "connect error: %s\n", strerror(errno));
		exit (1);
	}
    }


void SockName(int fd, const struct sockaddr * sa, socklen_t len, struct sockaddr_in	* addr){
    char buff[MAXLINE];

    if (getsockname(fd, sa, &len) < 0) {
		printf("getsockname error");
		exit (1);
    } else {
        inet_ntop(AF_INET, &addr->sin_addr, buff, sizeof(buff));
        printf("\nClient Connection from %s, port %d\n", buff, ntohs(addr->sin_port));
        
    }
}

void PeerName(int fd, const struct sockaddr * sa, socklen_t len, struct sockaddr_in	* addr){
    char buff[MAXLINE];

    if (getpeername(fd, sa, &len) < 0) {
		printf("getpeername error");
		exit (1);
    } else {
        inet_ntop(AF_INET, &addr->sin_addr, buff, sizeof(buff));
        printf("\nServer Connection from %s, port %d\n", buff, ntohs(addr->sin_port));
        
    }
}


//prac3

ssize_t writen(int fd, const void *vptr, size_t n)
{
	size_t		nleft;
	ssize_t		nwritten;
	const char	*ptr;

	ptr = vptr;
	nleft = n;
	while (nleft > 0) {
		if ( (nwritten = write(fd, ptr, nleft)) <= 0) {
			if (errno == EINTR)
				nwritten = 0;		/* and call write() again */
			else
				return(-1);			/* error */
		}

		nleft -= nwritten;
		ptr   += nwritten;
	}
	return(n);
}
/* end writen */
//prac3

void Writen(int fd, void *ptr, size_t nbytes) {
    if (writen(fd, ptr, nbytes) != nbytes) {
        fprintf(stderr, "writen error\n");
        exit(EXIT_FAILURE); // or return an error code
    }
}


//prac3

static ssize_t my_read(int fd, char *ptr)
{
	static int	read_cnt = 0;
	static char	*read_ptr;
	static char	read_buf[MAXLINE];

	if (read_cnt <= 0) {
again:
		if ( (read_cnt = read(fd, read_buf, sizeof(read_buf))) < 0) {
			if (errno == EINTR)
				goto again;
			return(-1);
		} else if (read_cnt == 0)
			return(0);
		read_ptr = read_buf;
	}

	read_cnt--;
	*ptr = *read_ptr++;
	return(1);
}
//prac3

ssize_t readline(int fd, void *vptr, size_t maxlen)
{
	int		n, rc;
	char	c, *ptr;

	ptr = vptr;
	for (n = 1; n < maxlen; n++) {
		if ( (rc = my_read(fd, &c)) == 1) {
			*ptr++ = c;
			if (c == '\n')
				break;	/* newline is stored, like fgets() */
		} else if (rc == 0) {
			if (n == 1)
				return(0);	/* EOF, no data read */
			else
				break;		/* EOF, some data was read */
		} else
			return(-1);		/* error, errno set by read() */
	}

	*ptr = 0;	/* null terminate like fgets() */
	return(n);
}
/* end readline */

//prac3
ssize_t Readline(int fd, void *ptr, size_t maxlen)
{
    ssize_t n;

    if ((n = readline(fd, ptr, maxlen)) < 0)
        return -1;  /* Error */

    return n;
}


int ADD(int x, int y){
    int result;
    result = x + y;
    return result;
}

int MUL(int x, int y){
    int result;
    result = x * y;
    return result;
}

float DIV(float x, float y){
    float result;
    result = x / y;
    return result;
}
int MOD(int x, int y){
    int result;
    result = x % y;
    return result;
}

void INFO(char * line){
    FILE *fp;
    fp = fopen("info.txt","r");
    if (fp == NULL){
        printf("file opening error");
        return;
    }

    char buffer[MAXLINE]; // This is a buffer to store the read data
    char buffer2[MAXLINE]; // This is a buffer to store the read data
   // buffer2[0] = '\0'; // Initialize buffer2 with an empty string


    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("%s",buffer);
      //  strcat(buffer2,buffer);

        //snprintf(line, MAXLINE, "%s\n", buffer);


    }
    //snprintf(line, MAXLINE, "%s\n", buffer2);



    fclose(fp); // Close the file
}






void calculate(char * line){
    long arg1, arg2, result =0;
    if (sscanf(line, "%s", line) == 1 && strncmp(line, "ADD(", 4) == 0) {
        if (sscanf(line, "ADD(%ld,%ld)", &arg1, &arg2) == 2) {
            result = ADD(arg1,arg2);

        }
    }
    else if (sscanf(line, "%s", line) == 1 && strncmp(line, "MUL(", 4) == 0) {
        if (sscanf(line, "MUL(%ld,%ld)", &arg1, &arg2) == 2) {
            result = MUL(arg1,arg2);

        }
    }
    else if (sscanf(line, "%s", line) == 1 && strncmp(line, "DIV(", 4) == 0) {
        if (sscanf(line, "DIV(%ld,%ld)", &arg1, &arg2) == 2) {
            result = DIV(arg1,arg2);

        }
    }
    else if (sscanf(line, "%s", line) == 1 && strncmp(line, "MOD(", 4) == 0) {
        if (sscanf(line, "MOD(%ld,%ld)", &arg1, &arg2) == 2) {
            result = MOD(arg1,arg2);

        }

    }
    else {
        snprintf(line, MAXLINE, "Invalid input\n");
        return;
    }
    snprintf(line, sizeof(line), "%ld\n", result);




}
//prac4
int Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout)
{
	int	n;

	if ( (n = select(nfds, readfds, writefds, exceptfds, timeout)) < 0){
		    perror("select error");
            exit(EXIT_FAILURE);
    }
	return(n);		
}

int max(int a, int b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}




void str_echo(int sockfd_request,int sockfd_reply,int time) {
    ssize_t n;
    char line[MAXLINE];
    fd_set rset;
    struct timeval timeout;
    int maxfd = max(sockfd_request,sockfd_reply) +1;

    for (;;) {
        FD_ZERO(&rset);
        FD_SET(sockfd_request, &rset);

        timeout.tv_sec = time;

        int ready = Select(maxfd, &rset, NULL, NULL, &timeout);

        if (ready == 0) {
            printf("Server: Timeout reached. Sending 'goodbye' message and terminating...\n");
            return;
        }

        if (FD_ISSET(sockfd_request, &rset)) {
            if ((n = Readline(sockfd_request, line, MAXLINE)) == 0)
                return; /* connection closed by other end */

            if (strncmp(line, "INFO", 4) == 0){
                INFO(line);
            }
            else if (strncmp(line, "QUIT", 4) == 0){
                printf("Thank you for using SFC. Goodbye!\n");
                printf("Client Quitting...\n");

                return;
            }
            else {
                calculate(line);
            }

            n = strlen(line);
            Writen(sockfd_reply, line, n);
        }
    }
}


// void str_echo(int sockfd_request,int sockfd_reply,int time) {
//     ssize_t n;
//     char line[MAXLINE];
//     int times = time;

//     for (;;) {
//         if ((n = Readline(sockfd_request, line, MAXLINE)) == 0)
//             return; /* connection closed by other end */
        
//         if (strncmp(line, "INFO", 4) == 0){
//             INFO(line);
//         }
//         else if (strncmp(line, "QUIT", 4) == 0){
//             printf("Thank you for using SFC. Goodbye!\n");
//             printf("Client Quitting...\n");

//             return;
//         }
//         else{
//             calculate(line);

//         }
 
//         n = strlen(line);
//         Writen(sockfd_reply, line, n);
//     }
// }



// void str_echo(int sockfd) {
//     ssize_t n;
//     char line[MAXLINE];

//     for (;;) {
//         if ((n = Readline(sockfd, line, MAXLINE)) == 0)
//             return; /* connection closed by other end */
        
//         if (strncmp(line, "INFO", 4) == 0){
//             INFO(line);
//         }
//         else if (strncmp(line, "QUIT", 4) == 0){
//             printf("Thank you for using SFC. Goodbye!\n");
//             printf("Client Quitting...\n");

//             return;
//         }
//         else{
//             calculate(line);

//         }
 
//         n = strlen(line);
//         Writen(sockfd, line, n);
//     }
// }

void resetLog(char *filename) // resets the log file every time i run the program because it was annoying constantly manually deleting it
{
    FILE *logFp = fopen(filename, "w");
    if (logFp == NULL) {
        printf("Error");
    }
    fclose(logFp);
}


void str_cli(FILE *fp, int sockfd_request, int sockfd_reply)
{
    char sendline[MAXLINE], recvline[MAXLINE];
    resetLog("log_file");
    FILE *log_file = fopen("log_file", "a"); 

    printf("\nClient: Type your message to server + Return or Control+C to terminate --> ");
    while ((void *) fgets(sendline, MAXLINE, fp) != NULL) {
        fprintf(log_file, "Request: %s", sendline);

        Writen(sockfd_request, sendline, strlen(sendline));

        if (Readline(sockfd_reply, recvline, MAXLINE) == 0) {
            fprintf(stderr, "str_cli: server terminated prematurely\n");
            exit(EXIT_FAILURE); 
        }
        fprintf(log_file, "Reply: %s\n", recvline);

        printf("\nClient: Echo from Server --> ");
        fputs(recvline, stdout);
        printf("\nClient: Type your message to server + Return or Control+C to terminate --> ");
    }
}




// void thread_function(void* arg) {
//     threadArgs * arguments = arg;
//     char buff[MAXLINE];
//     int	connfd;

//     printf("Thread %d is starting\n", arguments->threadID);
// 	connfd = arguments->connfd;

// 	pthread_detach(pthread_self());
// 	str_echo(connfd);		
// 	close(connfd);			

//     printf("Thread %d is exiting\n", arguments->threadID);
//     pthread_exit(NULL);
// }


// void thread_make(int i){
//     void * thread_func(void*);
//     threadArgs * tptr;
//     pthread_create(&tptr[i].thread, NULL, (void*)thread_function, (void*)&i);



// }

// void *thread_func(void *arg) {
//     Thread *threadArgs = (Thread*)arg;
//     int connfd = threadArgs->connfd;
//     int count = threadArgs->clients;

//     while (count > 0){
//         if (count == 0){
//             printf("Ran out of clients to serve");
//             break;
//         }
//         else{
//             str_echo(connfd); // Handle communication with the client
//             count--;
//         }
//     }

//     close(connfd); // Close the connection
//     free(arg); // Free the memory allocated for the argument

//     return NULL;
// }

// void thread_func(void *arg) {
//     Thread *threadArgs = (Thread *)arg;
//     // int *count = *(threadArgs->clients);
//     int connfd = *(threadArgs->connfd);
//     // free(threadArgs);
//     // int connfd = *(int *)(arg);

//     str_echo(connfd); // Handle communication with the client
//     close(connfd);

//     *(threadArgs->exit) = 1;

//     free(threadArgs->connfd);
//     free(threadArgs);
// }

// void create_threads(void * arg){
//     threadArgs * threadArg = arg;
//     pthread_t threads[threadArg->numThreads];  
//     int i;
//     for (i=0; i<threadArg->numThreads; i++){
//         threadArg[i].threadID = i;
//         pthread_create(&threads[i], NULL, (void*)thread_function, (void*)&threadArg);

//     }

// }

