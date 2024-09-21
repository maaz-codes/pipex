#include "pipex.h"

void print_error()
{
    perror("zsh:");
    exit(EXIT_FAILURE);
}

static void execute(char **cmd, char *env[], int read_from, int write_to)
{
    char *path;

    path = ft_cmd_exits(env, cmd[0]);
    if (!path)
        (free_array(cmd), print_error());
    if (dup2(read_from, STDIN_FILENO) < 0 || dup2(write_to, STDOUT_FILENO) < 0)
        (free_array(cmd), free(path), close(read_from), close(write_to), print_error());
    close(read_from);
    close(write_to);
    execve(path, cmd, env);
    free_array(cmd), free(path);
}

static void first_process(int argc, char **argv, char **env, int *pipefd)
{
    write(2, "first_process\n", 14);
    int pid;
    int in_file;
    char **cmd;

    close(pipefd[0]);
    in_file = open(argv[1], O_RDONLY);
    if (in_file == -1)
        (print_error());
    cmd = set_cmd_arguments(argv[2]);
    if (!cmd)
        (print_error());
    pid = fork();
    if (pid == -1)
        (free_array(cmd), close(in_file), print_error());
    if (pid == 0) // child_process
        execute(cmd, env, in_file, pipefd[1]);
}

static void last_process(int argc, char **argv, char **env, int *pipefd)
{
    write(2, "last_process\n", 13);
    int pid;
    int out_file;
    char **cmd;

    close(pipefd[1]);
    out_file = open(argv[argc - 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (out_file == -1)
        (print_error());
    cmd = set_cmd_arguments(argv[argc - 2]);
    if (!cmd)
        (print_error());
    pid = fork();
    if (pid == -1)
        (free_array(cmd), close(out_file), print_error());
    if (pid == 0) // child_process
        execute(cmd, env, pipefd[0], out_file);
}

int count_cmds(char *argv[], int argc)
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
    int pid;
    int pipefd[2];
    int status;

    if (argc == 5)
    {
        if (pipe(pipefd) == -1)
            (print_error());
        first_process(argc, argv, env, pipefd);
        last_process(argc, argv, env, pipefd);
        waitpid(pid, &status, 0);
        write(2, "parent_process\n", 16);
        return (0);
    }
    return (1);
}
