#include "get_next_line/get_next_line.h"
#include "pipex.h"

// ./pipex here_doc LIMITER cmd1 cmd2 outfile

int ft_here_doc(char *limiter, int *pipefd, char **argv, char **env)
{
	char	*line;
    int     len;
    char    *new_limiter;
    
    char **cmd = set_cmd_arguments(argv[2]); // {"wc", "-l", NULL}
    char *path = ft_cmd_exits(env, cmd[0]); // {"/usr/bin/wc"}
    len = ft_strlen(limiter) + 1;
    new_limiter = ft_strjoin(limiter, "\n");
    line = get_next_line(0);
    while (line)
    {
        if (!ft_strncmp(line, new_limiter, len))
            return (0);
        write(pipefd[1], line, ft_strlen(line));
        free(line);
        line = get_next_line(0);
    }
    dup2(pipefd[0], 0);
    dup2(pipefd[1], 1);
    close(pipefd[0]);
    close(pipefd[1]);
    free(new_limiter);
    free(line);
    execve(path, cmd, env); //cmd1
    free(path);
    free(cmd);
    return (1);
}

    // if (fd1 == -1 || !path || dup2(fd1, 0) == -1 || dup2(pipefd[1], 1) == -1)
    // {
    //     if (path)
    //         free(path);
    //     close(fd1);
    //     free_array(cmd);
    //     perror("zsh");
    //     exit(0);
    // }
    // close(pipefd[1]);
    // close(fd1);
    // execve(path, cmd, env);
    // free(path);
    // free_array(cmd);
    // perror("Error in exceve()-ing in child_process\n");
    // return (0);
