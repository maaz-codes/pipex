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

static char *ft_cmd_exits(char **env, char *cmd)
{
    char **absoulute_paths;
    char *temp_str_join;
    char *path;
    int i;

    i = 0;
    
    path = NULL;
    if (!cmd)
        return (NULL); // ft_split fails
    absoulute_paths = extract_path_env(env);
    i = 0;
    while (absoulute_paths[i])
    {
        temp_str_join = ft_strjoin(absoulute_paths[i], "/");
        free(absoulute_paths[i]);
        absoulute_paths[i] = ft_strjoin(temp_str_join, cmd);
        free(temp_str_join);
        if (access(absoulute_paths[i], F_OK) == 0)
            if (access(absoulute_paths[i], X_OK) == 0)
                path = ft_strdup(absoulute_paths[i]);
        i++;
    }
    free_array(absoulute_paths);
    return (path);
}

static int child_process(int *pipefd, char **argv, char **env, int cmd_index)
{
    int fd1;
    char **cmd;
    char *path;

    cmd = set_cmd_arguments(argv[cmd_index]); //{"wc", "-l", NULL}
    path = ft_cmd_exits(env, cmd[0]);
    if (!path)
    {
        free_array(cmd);
        return (ft_error("Error in path of child\n"));
    }
    fd1 = open(argv[1], O_RDONLY);
    if (fd1 == -1)
        return (ft_error("Error in opening file1\n"));
    if (dup2(fd1, 0) == -1)
        perror("Error in dup()-ing fd1\n");
    if (dup2(pipefd[1], 1) == -1)
        perror("Error in dup()-ing pipefd[1] for file-1\n");
    close (pipefd[0]);
    close(pipefd[1]);
    close(fd1);
    execve(path, cmd, env);
    perror("Error in exceve()-ing in child_process\n");
    return (0);
}

static int parent_process(int *pipefd, char **argv, char **env)
{
    int fd2;
    char **cmd;
    char *path;

    cmd = set_cmd_arguments(argv[3]);
    path = ft_cmd_exits(env, cmd[0]);
    if (!path)
    {
        free_array(cmd);
        return (ft_error("Error in path of parent\n"));
    }
    fd2 = open(argv[4], O_CREAT | O_WRONLY | O_TRUNC, 0644);
        if (fd2 == -1)
            return (ft_error("Error in opening file2\n"));
    if (dup2(pipefd[0], 0) == -1)
        perror("Error in dup()-ing pipefd[0] for file-2\n");
    if (dup2(fd2, 1) == -1)
        perror("Error in dup()-ing fd2\n");
    close(pipefd[1]);
    close (pipefd[0]);
    close(fd2);
    execve(path, cmd, env);
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
            if (child_process(pipefd, argv, env, 2) == 1)
                exit(1);
        if (parent_process(pipefd, argv, env) == 1)
            exit(1);
    }
    return (0);
}