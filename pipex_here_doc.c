#include "pipex.h"

// file1.txt -content> cmd1 -output> cmd2 -output> file2.txt

// ./pipex infile cmd1 | ...
static int child_process(int *pipefd, char **argv, char **env, int pid)
{
    int fd1;
    char **cmd;
    char *path;

    cmd = set_cmd_arguments(argv[2]); //{"wc", "-l", NULL} // {"/bin/wc", NULL}
    path = ft_cmd_exits(env, cmd[0]);
    fd1 = open(argv[1], O_RDONLY);
    close (pipefd[0]);
    if (fd1 == -1 || !path || dup2(fd1, 0) == -1 || dup2(pipefd[1], 1) == -1)
    {
        if (path)
            free(path);
        close(fd1);
        free_array(cmd);
        perror("zsh");
        exit(0);
    }
    close(pipefd[1]);
    close(fd1);
    execve(path, cmd, env);
    free(path);
    free_array(cmd);
    perror("Error in exceve()-ing in child_process\n");
    return (0);
}

// ... | cmd2 outfile
static int parent_process(int *pipefd, char **argv, char **env, int argc)
{
    int fd2;
    char **cmd;
    char *path;

    cmd = set_cmd_arguments(argv[argc - 2]);
    path = ft_cmd_exits(env, cmd[0]);
    close(pipefd[1]);
    fd2 = open(argv[argc - 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (!path || fd2 == -1 || dup2(pipefd[0], 0) == -1 || dup2(fd2, 1) == -1)
    {
        if (path)
            free(path);
        close(fd2);
        close (pipefd[0]);
        free_array(cmd);
        perror("Error in cmd_parent\n");
        exit(127);
    }
    close (pipefd[0]);
    close(fd2);
    execve(path, cmd, env);
    free(path);
    free_array(cmd);
    perror("Error in exceve()-ing in parent_process\n");
    return (0);
}

int *get_std_fds(int flag, int *std_fds)
{
    int *fds;

    if (flag == 0)
    {
        fds = malloc(sizeof(int *) * 2);
        if (!fds)
            return (NULL);
        fds[0] = dup(0);
        fds[1] = dup(1);
        return (fds);
    }
    else    
    {
        dup2(0, std_fds[0]);
        dup2(1, std_fds[1]);
    }
    return (fds);
}

int main(int argc, char *argv[], char *env[])
{
    int pipefd[2];
    int pid;
    int *std_fds;

    std_fds = get_std_fds(0, pipefd); //returns NULL if fails
    if (pipe(pipefd) == -1)
        return (ft_error("Error in pipe()-ing\n"));
    pid = fork();
    if (pid == -1)
        perror("Error in fork()-ing\n");
    if (pid == 0) // first_process
    {
        if (!ft_strncmp(argv[1], "here_doc", 8))
        {
            if (argc != 6)
            {
                free(std_fds);
                perror("Error in here_doc\n");
                exit(1);
            }
            ft_here_doc(argv[2], pipefd, argv, env);
            get_std_fds(1, std_fds);
            free(std_fds);
        }
        else
            child_process(pipefd, argv, env, pid);
    }
    else // last_process
    {
        wait(NULL);
        parent_process(pipefd, argv, env, argc);
    }
    return (0);
}
