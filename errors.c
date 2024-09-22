#include "pipex.h"

void print_error(char *msg)
{
    perror("perror");
    printf("In: %s\n", msg);
    printf("errno: %d = %s\n", errno, strerror(errno));
    if (errno == ENOENT)
        exit(127);
    exit(1);
}

void free_array(char **array)
{
    int i;

    i = 0;
    if (!array)
        return ;
    while (array[i])
    {
        free(array[i]);
        i++;
    }
    free(array);
}
