#include "acc.h"


int max(int a, int b)
{
    if (a > b)
    {
        return a;
    }
    else
    {
        return b;
    }
}

void str_echo(int sockfd_request, int sockfd_reply, int time)
{
    ssize_t n;
    char line[MAXLINE];
    fd_set rset;
    struct timeval timeout;
    int maxfd = max(sockfd_request, sockfd_reply) + 1;

    for (;;)
    {
        FD_ZERO(&rset);
        FD_SET(sockfd_request, &rset);

        timeout.tv_sec = time;
        timeout.tv_usec = 0;

        int ready = Select(maxfd, &rset, NULL, NULL, &timeout);

        if (ready == 0)
        {
            // Writen(sockfd_reply, "QUIT", sizeof("QUIT"));
            const char *message = "Timeout";
            send(sockfd_reply, message, strlen(message), 0);
            close(sockfd_reply);
            return;
        }

        if (FD_ISSET(sockfd_request, &rset))
        {
            if ((n = Readline(sockfd_request, line, MAXLINE)) == 0)
                return; /* connection closed by other end */

            if (strncmp(line, "QUIT", 4) == 0)
            {
                const char *message = "Thank you for using SFC. Goodbye!\n";
                send(sockfd_reply, message, strlen(message), 0);

                // printf("Thank you for using SFC. Goodbye!\n");

                return;
            }
            else
            {
                calculate(line);
            }

            n = strlen(line);
            Writen(sockfd_reply, line, n);
        }
    }
}