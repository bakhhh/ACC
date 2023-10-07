#include "acc.h"

// prac3

ssize_t writen(int fd, const void *vptr, size_t n)
{
    size_t nleft;
    ssize_t nwritten;
    const char *ptr;

    ptr = vptr;
    nleft = n;
    while (nleft > 0)
    {
        if ((nwritten = write(fd, ptr, nleft)) <= 0)
        {
            if (errno == EINTR)
                nwritten = 0; /* and call write() again */
            else
                return (-1); /* error */
        }

        nleft -= nwritten;
        ptr += nwritten;
    }
    return (n);
}
/* end writen */
// prac3

void Writen(int fd, void *ptr, size_t nbytes)
{
    if (writen(fd, ptr, nbytes) != nbytes)
    {
        fprintf(stderr, "writen error\n");
        exit(EXIT_FAILURE); // or return an error code
    }
}

// prac3

static ssize_t my_read(int fd, char *ptr)
{
    static int read_cnt = 0;
    static char *read_ptr;
    static char read_buf[MAXLINE];

    if (read_cnt <= 0)
    {
    again:
        if ((read_cnt = read(fd, read_buf, sizeof(read_buf))) < 0)
        {
            if (errno == EINTR)
                goto again;
            return (-1);
        }
        else if (read_cnt == 0)
            return (0);
        read_ptr = read_buf;
    }

    read_cnt--;
    *ptr = *read_ptr++;
    return (1);
}
// prac3

ssize_t readline(int fd, void *vptr, size_t maxlen)
{
    int n, rc;
    char c, *ptr;

    ptr = vptr;
    for (n = 1; n < maxlen; n++)
    {
        if ((rc = my_read(fd, &c)) == 1)
        {
            *ptr++ = c;
            if (c == '\n')
                break; /* newline is stored, like fgets() */
        }
        else if (rc == 0)
        {
            if (n == 1)
                return (0); /* EOF, no data read */
            else
                break; /* EOF, some data was read */
        }
        else
            return (-1); /* error, errno set by read() */
    }

    *ptr = 0; /* null terminate like fgets() */
    return (n);
}

// prac3
ssize_t Readline(int fd, void *ptr, size_t maxlen)
{
    ssize_t n;

    if ((n = readline(fd, ptr, maxlen)) < 0)
        return -1; /* Error */

    return n;
}
