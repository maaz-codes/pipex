#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int pid;
    int pipefd[2];

    if (argc > 1)
    {
        printf("%s %s\n", argv[1], argv[2]);
        if (pipe(pipefd) == -1)
            printf("Error in pipe()-ing\n");
        pid = fork();
        if (pid == -1)
            printf("Error in fork()-ing\n");
        
        if (pid == 0) // child_process
        {
            close(pipefd[0]);
            if (write (pipefd[1], "Hello", 5) == -1)
                printf("Error in write()-ing\n");
            close(pipefd[1]);
            exit(0);
        }
        else
        {
            char *str = malloc(sizeof(char) * 6);
            close(pipefd[1]);
            if (read(pipefd[0], str, 5) == -1)
                printf("Error in read()-ing");
            str[5] = '\0';
            printf("From child process : %s\n", str);
            close(pipefd[0]);
            // wait(NULL);
        }
    }
    return (0);
}