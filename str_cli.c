#include "acc.h"



void getTime(int *hour, int *min, int *sec) // got a little help from stack overflow to figure out how to use time but i did not copy //Antrromet. 2011. “Get the Current Time in C.” Stack Overflow. February 28, 2011. https://stackoverflow.com/questions/5141960/get-the-current-time-in-c.
{
    time_t timeNow;
    struct tm *timeinfo;
    time(&timeNow);
    timeinfo = localtime(&timeNow);
    *hour = timeinfo->tm_hour;
    *min = timeinfo->tm_min;
    *sec = timeinfo->tm_sec;
}
pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;

void str_cli(FILE *fp, int sockfd_request, int sockfd_reply)
{
    char sendline[MAXLINE], recvline[MAXLINE];
    pthread_mutex_lock(&log_mutex);
    char log_filename[20];
    int hour, min, sec;
    getTime(&hour, &min, &sec);
    sprintf(log_filename, "log_file_%d:%d:%d", hour, min, sec);

    FILE *log_file = fopen(log_filename, "w");
    pthread_mutex_unlock(&log_mutex);

    printf("\nClient> ");
    while (fgets(sendline, MAXLINE, fp) != NULL)
    {
        fprintf(log_file, "Request: %s", sendline);

        Writen(sockfd_request, sendline, strlen(sendline));

        if (strcmp(sendline, "QUIT\n") == 0)
        {
            char buffer[MAXLINE];
            ssize_t n = recv(sockfd_reply, buffer, sizeof(buffer), 0);
            buffer[n] = '\0';    
            printf("%s", buffer); 

            return;
        }

        if (Readline(sockfd_reply, recvline, MAXLINE) == 0)
        {
            return;
        }
        if (strcmp(recvline, "Timeout") == 0)
        {
            printf("Server timed out. Exiting...\n");
            return;
        }

        fprintf(log_file, "Reply: %s\n", recvline);

        printf("\nServer> ");

        char *token = strtok(recvline, "|");
        while (token != NULL)
        {
            fputs(token, stdout);
            printf("\n"); // Add a newline after each segment
            token = strtok(NULL, "|");
        }

        printf("\nClient> ");
    }

    fclose(log_file); // Close the log file when finished
}
