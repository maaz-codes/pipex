#include "pipex.h"

// file1.txt -content> cmd1 -output> cmd2 -output> file2.txt

static void child_process(int *pipefd, char *path, char *arguments[])
{
    close (pipefd[0]);
    if (dup2(pipefd[1], 1) == -1)
        printf("Error in dup()-ing pipefd[1] for file-1\n");
    close(pipefd[1]);
    execve(path, arguments, NULL);
    printf("Error in exeve()-ing in child_process\n");
}

static void parent_process(int *pipefd, int fd2, char *path, char *arguments[])
{
    close(pipefd[1]);
    if (dup2(pipefd[0], 0) == -1)
        printf("Error in dup()-ing pipefd[0] for file-2\n");
    if (dup2(fd2, 1) == -1)
        printf("Error in dup()-ing fd2\n");
    execve(path, arguments, NULL);
    printf("Error in exeve()-ing in parent_process\n");
}

static char *ft_cmds_exits(char **env, char *cmd)
{
    char **absoulute_paths;
    char *temp_str_join;
    int i;

    i = 0;
    absoulute_paths = ft_split(env[4], ':');
    while (i < 5)
    {
        temp_str_join = absoulute_paths[i];
        absoulute_paths[i] = ft_strjoin(absoulute_paths[i], "/");
        free(temp_str_join);
        temp_str_join = absoulute_paths[i];
        absoulute_paths[i] = ft_strjoin(absoulute_paths[i], cmd);
        free(temp_str_join);
        // printf("%s\n", absoulute_paths[i]);
        if (access(absoulute_paths[i], F_OK) == 0)
            return (absoulute_paths[i]);
        i++;
    }
    return (NULL);
}

int main(int argc, char *argv[], char *env[])
{
    int fd1;
    int fd2;
    int pipefd[2];
    int pid;
    char *arguments[2];
    char *path;

    path = ft_cmds_exits(env, argv[2]);
    if (path)
    {
        arguments[0] = argv[2]; // cmd1
        // arguments[1] = argv[1]; // file1-name
        arguments[1] = NULL;
        pipe(pipefd);
        fd1 = open(argv[1], O_RDONLY);
        fd2 = open(argv[4], O_CREAT | O_WRONLY | O_TRUNC, 0644);
        if (fd2 == -1)
            perror("Error in fds\n");
        if (dup2(fd1, 0) == -1)
            printf("Error in dup()-ing fd1\n");
        close(fd1);
        pid = fork();
        if (pid == -1)
            printf("Error in fork()-ing\n");
        if (pid == 0)
            child_process(pipefd, path, arguments);
        else
        {
            wait(NULL);
            path = ft_cmds_exits(env, argv[3]);
            if (path)
            {
                arguments[0] = argv[3]; // cmd2
                // arguments[1] = argv[4]; // file2-name
                arguments[1] = NULL;
                parent_process(pipefd, fd2, path, arguments);
            }
            else
            {
                printf("cmd not found: %s\n", argv[3]);
            }
        }
        write (2, "OK\n", 3);
    }
    else
    {
        printf("cmd not found: %s\n", argv[2]);
    }
    return (0);
}
