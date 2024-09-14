#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    char *args[4] = {"./pipes2", "Hello", "World", NULL};
    printf("In the first program\n");
    if (execve("./pipes2", args, NULL) == -1)
        printf("Error in execve()\n");
}