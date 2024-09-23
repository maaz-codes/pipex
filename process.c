#include "pipex.h"

void execute(char **cmd, char *env[], int read_from, int write_to)
{
    char *path;

    path = ft_cmd_exits(env, cmd[0]);
    if (!path)
        (free_array(cmd), close(read_from), close(write_to), print_error("path"));
    // if (dup2(read_from, 0) < 0 || dup2(write_to, 1) < 0)
    //     (free_array(cmd), free(path), close(read_from), close(write_to), print_error("dup2()"));
    if (dup2(read_from, 0) < 0)
        (free_array(cmd), free(path), close(read_from), close(write_to), print_error("dup2(read_from)"));
    if (dup2(write_to, 1) < 0)
        (free_array(cmd), free(path), close(read_from), close(write_to), print_error("dup2(write_from)"));
    close(read_from), close(write_to);
    execve(path, cmd, env);
    free_array(cmd), free(path), print_error("execve()");
}

pid_t first_process(int argc, char **argv, char **env, int *pipefd)
{
    write(2, "first_process\n", 14);
    pid_t pid;
    int in_file;
    char **cmd;

    pid = fork();
    if (pid == -1)
        (close(pipefd[0]), close(pipefd[1]), print_error("fork(last_process)"));
    if (pid == 0) // child_process
    {
        in_file = open(argv[1], O_RDONLY);
        if (in_file == -1)
            (close(pipefd[0]), close(pipefd[1]), print_error("open(in_file)"));
        cmd = set_cmd_arguments(argv[2]);
        if (!cmd)
            (close(pipefd[0]), close(pipefd[1]), print_error("cmd1"));
        close(pipefd[0]);
        execute(cmd, env, in_file, pipefd[1]);
    }
    return (pid);
}

pid_t last_process(int argc, char **argv, char **env, int *pipefd)
{
    write(2, "last_process\n", 13);
    pid_t pid;
    int out_file;
    char **cmd;

    pid = fork();
    if (pid == -1)
        (close(pipefd[0]), close(pipefd[1]), print_error("fork(last_process)"));
    if (pid == 0) // child_process
    {
        out_file = open(argv[argc - 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
        if (out_file == -1)
            (close(pipefd[0]), close(pipefd[1]), print_error("open(out_file)"));
        cmd = set_cmd_arguments(argv[argc - 2]);
        if (!cmd)
            (close(pipefd[0]), close(pipefd[1]), print_error("cmd2"));
        close(pipefd[1]);
        execute(cmd, env, pipefd[0], out_file);
    }
    return (pid);
}

pid_t middle_process(char *argv_cmd, char **env, int read_from, int *pipefd)
{
    write(2, "middle_process\n", 15);
    pid_t pid;
    char **cmd;

    pid = fork();
    if (pid == -1)
        (close(read_from), close(pipefd[0]), close(pipefd[1]), print_error("fork(middle_process)"));
    if (pid == 0)
    {
        cmd = set_cmd_arguments(argv_cmd);
        if (!cmd)
            (close(read_from), close(pipefd[0]), close(pipefd[1]), print_error("cmd_middle"));
        execute(cmd, env, read_from, pipefd[1]);
    }
    return (pid);
}
