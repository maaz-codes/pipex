#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

int main(void)
{
    int pid1 = fork();
    printf("printing 1... %d\n", pid1);
    int pid2 = fork();
    printf("printing 2... %d %d\n", pid2, pid1);
    return (0);
}
