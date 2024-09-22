#include "get_next_line/get_next_line.h"
#include "pipex.h"

// ./pipex here_doc LIMITER cmd1 cmd2 outfile

static void read_write(char *limiter, int write_to)
{
    char *new_limiter;
    char *line;

    new_limiter = ft_strjoin(limiter, "\n");
    line = get_next_line(0);
    while (line)
    {
        if (!ft_strncmp(line, new_limiter, ft_strlen(new_limiter)))
            break ;
        write(write_to, line, ft_strlen(line));
        free(line);
        line = get_next_line(0);
    }
    free(new_limiter);
    close(write_to);
}

int ft_here_doc(char *limiter, char **argv, char **env, int *pipefd)
{
	pid_t pid;
    char **cmd;
    char *path;
    int doc_pipe[2];

    pipe(doc_pipe);
    read_write(limiter, doc_pipe[1]);
    cmd = set_cmd_arguments(argv[3]);
    if (!cmd)
        (close(pipefd[0]), close(pipefd[1]), print_error("cmd-here_doc"));
    pid = fork();
    if (pid == -1)
        (close(pipefd[0]), close(pipefd[1]), print_error("fork(here_doc)"));
    if (pid == 0)
    {
        execute(cmd, env, doc_pipe[0], pipefd[1]);
        
    }
    return (pid);
}
