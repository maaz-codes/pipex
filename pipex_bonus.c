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
    int status;

    if (argc >= 5)
    {
        if (pipe(pipefd) == -1)
            (print_error("pipe()"));
        if (!ft_strncmp(argv[1], "here_doc", 9))
            pid_first = ft_here_doc(argv[2], argv, env, pipefd);
        else
            pid_first = first_process(argc, argv, env, pipefd);
        pid_last = last_process(argc, argv, env, pipefd);
        close(pipefd[0]);
        close(pipefd[1]);
        waitpid(pid_first, &status, 0);
        waitpid(pid_last, &status, 0);
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
