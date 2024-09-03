#include <unistd.h>     // read(), write()
#include <stdlib.h>     // exit()
#include <sys/types.h>  // fork()
#include <stdio.h>      // printf()

int main(void)
{
    int nums[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int arr_size = sizeof(nums) / sizeof(int); 
    int pipefd[2];

    if (pipe(pipefd) == -1)
        printf("Error in creating pipe\n");
    int sum = 0;
    int start;
    int end;
    int pid = fork();
    if (pid == -1)
        printf("Error in fork()-ing...\n");
    if (pid == 0) // child_process
    {
        start = 0;
        end = arr_size / 2;
    }
    else
    {
        start = arr_size / 2;
        end = arr_size;
    }
    while (start < end)
        sum += nums[start++];
    if (pid == 0)
    {
        close (pipefd[0]);
        write (pipefd[1], &sum, sizeof(int));
        close (pipefd[1]);
    }
    else
    {
        int child_sum;

        close (pipefd[1]);
        read (pipefd[0], &child_sum, sizeof(int));
        close (pipefd[0]);
        sum += child_sum;
        printf("Sum = %d\n", sum);
        wait(NULL);
    }
    return (0);
}