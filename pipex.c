#include "pipex.h"

// file1.txt -content> cmd1 -output> cmd2 -output> file2.txt

static void free_array(char **array)
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

static int ft_error(char *error_msg)
{
    perror(error_msg);
    return (1);
}

static char **set_cmd_arguments(char *cmd)
{
    char **cmd_args;

    cmd_args = ft_split(cmd, ' ');
    if (!cmd_args)
        perror("split fails\n");
    return (cmd_args);
}

static char **extract_path_env(char **env)
{
    int i;
    char *path;
    char **absoulute_paths;

    i = 0;
    while (env[i])
    {
        if (ft_strncmp(env[i], "PATH=", 5) == 0)
        {
            path = ft_strdup(env[i] + 5);
            break ;
        }
        i++;
    }
    absoulute_paths = ft_split(path, ':');
    free(path);
    return (absoulute_paths);
}

static char *set_own_path(char *cmd)
{
    int i;

    i = 0;
    if (cmd[0] == '/')
        return (cmd);
    return (NULL);
}

static char *ft_cmd_exits(char **env, char *cmd, int pid)
{
    char **a_paths;
    char *temp_str_join;
    char *path;
    int i;

    i = 0;
    path = NULL;
    if (!cmd || set_own_path(cmd))
        return (cmd); // ft_split fails
    a_paths = extract_path_env(env);
    while (a_paths[i])
    {
        temp_str_join = ft_strjoin(a_paths[i], "/");
        free(a_paths[i]);
        a_paths[i] = ft_strjoin(temp_str_join, cmd);
        free(temp_str_join);
        if (access(a_paths[i], F_OK) == 0 && access(a_paths[i], X_OK) == 0)
        {
                path = ft_strdup(a_paths[i]);
                break ;
        }
        i++;
    }
    free_array(a_paths);
    return (path);
}

static int child_process(int *pipefd, char **argv, char **env, int pid)
{
    int fd1;
    char **cmd;
    char *path;

    cmd = set_cmd_arguments(argv[2]); //{"wc", "-l", NULL} // {"/bin/wc", NULL}
    path = ft_cmd_exits(env, cmd[0], pid);
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

static int parent_process(int *pipefd, char **argv, char **env, int pid)
{
    int fd2;
    char **cmd;
    char *path;

    cmd = set_cmd_arguments(argv[3]);
    path = ft_cmd_exits(env, cmd[0], pid);
    close(pipefd[1]);
    fd2 = open(argv[4], O_CREAT | O_WRONLY | O_TRUNC, 0644);
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

int main(int argc, char *argv[], char *env[])
{
    int pipefd[2];
    int pid;

    if (argc == 5)
    {
        if (pipe(pipefd) == -1)
            return (ft_error("Error in pipe()-ing\n"));
        pid = fork();
        if (pid == -1)
            perror("Error in fork()-ing\n");
        if (pid == 0)
            child_process(pipefd, argv, env, pid);
        else
        {
            wait(NULL);
            parent_process(pipefd, argv, env, pid);
        }
    }
    return (0);
}
