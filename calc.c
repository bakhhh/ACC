#include "acc.h"


int ADD(int x, int y)
{
    int result;
    result = x + y;
    return result;
}

int MUL(int x, int y)
{
    int result;
    result = x * y;
    return result;
}

float DIV(float x, float y)
{
    float result;
    result = x / y;
    return result;
}
int MOD(int x, int y)
{
    int result;
    result = x % y;
    return result;
}

void INFO(char *line)
{
    FILE *fp;
    fp = fopen("info.txt", "r");
    if (fp == NULL)
    {
        printf("file opening error");
        return;
    }

    char buffer[MAXLINE];
    char buffer2[MAXLINE];
    buffer2[0] = '\0'; // Initialize buffer2 with an empty string

    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        strcat(buffer2, buffer);
    }
    snprintf(line, MAXLINE, "%s\n", buffer2);

    fclose(fp); // Close the file
}

void calculate(char *line)
{
    long arg1, arg2, result = 0;
    if (sscanf(line, "%s", line) == 1 && strncmp(line, "ADD(", 4) == 0)
    {
        if (sscanf(line, "ADD(%ld,%ld)", &arg1, &arg2) == 2)
        {
            result = ADD(arg1, arg2);
            snprintf(line, sizeof(line), "%ld\n", result);
        }
        else{
            snprintf(line, MAXLINE, "Invalid input\n");

        }
    }
    else if (sscanf(line, "%s", line) == 1 && strncmp(line, "MUL(", 4) == 0)
    {
        if (sscanf(line, "MUL(%ld,%ld)", &arg1, &arg2) == 2)
        {
            result = MUL(arg1, arg2);
            snprintf(line, sizeof(line), "%ld\n", result);
        }
        else{
            snprintf(line, MAXLINE, "Invalid input\n");

        }
    }
    else if (sscanf(line, "%s", line) == 1 && strncmp(line, "DIV(", 4) == 0)
    {
        if (sscanf(line, "DIV(%ld,%ld)", &arg1, &arg2) == 2)
        {
            result = DIV(arg1, arg2);
            snprintf(line, sizeof(line), "%ld\n", result);
        }
        else{
            snprintf(line, MAXLINE, "Invalid input\n");

        }
    }
    else if (sscanf(line, "%s", line) == 1 && strncmp(line, "MOD(", 4) == 0)
    {
        if (sscanf(line, "MOD(%ld,%ld)", &arg1, &arg2) == 2)
        {
            result = MOD(arg1, arg2);
            snprintf(line, sizeof(line), "%ld\n", result);
        }
        else{
            snprintf(line, MAXLINE, "Invalid input\n");

        }
    }
    else if (strncmp(line, "INFO", 4) == 0)
    {
        INFO(line);
    }
    else
    {
        snprintf(line, MAXLINE, "Invalid input\n");
        return;
    }
}