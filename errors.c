#include "pipex.h"

void free_array(char **array)
{
    int i;

    i = 0;
    while (array[i])
    {
        free(array[i]);
        i++;
    }
    free(array);
}

int ft_error(char *error_msg)
{
    perror(error_msg);
    return (1);
}