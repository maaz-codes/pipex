#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>

int main(void)
{
    int pid;
    int pipefd[2];
    // pipefd[0] -> read end
    // pipefd[1] -> write end

    if (pipe(pipefd) == -1)
        printf("Error in opening of pipes\n");
    pid = fork();
    if (pid == -1)
        printf("Error in fork()");
    if (pid == 0)
    {
        close(pipefd[0]);
        int x;
        printf("Input a number from 0 to 9 : ");
        scanf("%d", &x);
        if (write (pipefd[1], &x, 4) == -1)
            printf("Error in writing to pipe\n");
        close(pipefd[1]);
        exit(0);
    }
    else
    {
        close(pipefd[1]);
        int y;
        if (read (pipefd[0], &y, 4) == -1)
            printf("Error in reading the pipe\n");
        printf("Read from child : %d\n", y);
        close(pipefd[0]);
    }
    return (0);
}