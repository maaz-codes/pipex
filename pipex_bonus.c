#include "pipex.h"
#include "get_next_line/get_next_line.h"

static int count_cmds(char *argv[], int argc)
{
    int count;

    if (!ft_strncmp(argv[1], "here_doc", 9))
        count = argc - 4;
    else    
        count = argc - 3;
    return (count);
}

int main(int argc, char *argv[], char *env[])
{
    int i;
    pid_t pid_last;
    pid_t pid_first;
    int pipefd[2];
    int read_from;
    int status;

    if (argc >= 5)
    {
        if (pipe(pipefd) == -1)
            (print_error("pipe()"));
        if (!ft_strncmp(argv[1], "here_doc", 9))
            pid_first = ft_here_doc(argv[2], argv, env, pipefd);
        else
            pid_first = first_process(argc, argv, env, pipefd);
        read_from = pipefd[0];
        i = 1;
        while(i <= count_cmds(argv, argc) - 2)
        {   
            pipe(pipefd);
            middle_process(i, argv, env, read_from, pipefd);
            close(read_from);
            close(pipefd[1]);
            read_from = pipefd[0];
            i++;
        }
        pid_last = last_process(argc, argv, env, pipefd);
        close(pipefd[0]);
        close(pipefd[1]);
        while () // add array of pids
        {
            waitpid(pid_first, &status, 0)
            i++;
        }
        write(2, "PARENT_PROCESS\n", 15);
        printf("status: %d\n", status);
        if (WIFEXITED(status))
            return (WEXITSTATUS(status));
        else if (WIFSIGNALED(status))
            return (WTERMSIG(status));
        return (EXIT_SUCCESS);
    }
    return (1);
}
