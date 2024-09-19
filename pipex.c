#include "pipex.h"
#include <string.h>

// ./pipex infile cmd1 cmd2 cmd3 outfile = 3 => argc - 3;
// ./pipex here_doc LIMITER cmd1 cmd2 cmd3 outfile = 3 => argc - 4;

static int *get_std_fds(int flag, int *std_fds)
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

typedef struct  s_pipex
{
    int process_count;
    int **pipefd;
    int pipe_index;
    int cmd_index;
}               t_pipex;

static void free_pipes(int **pipefds)
{
    int i;

    i = 0;
    while (pipefds[i])
    {
        free(pipefds[i]);
        i++;
    }
    free(pipefds);
}

static int first_process(int *pipefd, char **argv, char **env)
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

static int last_process(t_pipex *pipex, char **argv, char **env, int argc)
{
    int fd2;
    char **cmd;
    char *path;
    int *lastpipe = pipex->pipefd[pipex->process_count - 2];

    printf("Last-process\n");
    cmd = set_cmd_arguments(argv[argc - 2]);
    path = ft_cmd_exits(env, cmd[0]);
    close(lastpipe[1]); // write_end
    fd2 = open(argv[argc - 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (!path || fd2 == -1 || dup2(lastpipe[0], 0) == -1 || dup2(fd2, 1) == -1)
    {
        if (path)
            free(path);
        close(fd2);
        close (lastpipe[0]);
        free_array(cmd);
        perror("Error in cmd_parent\n");
        exit(127);
    }
    close (lastpipe[0]); // read_end
    close(fd2);
    free_pipes(pipex->pipefd);
    write(2, "run\n", 4);
    // printf("path = %s | cmd = %s\n", path, cmd[0]);
    execve(path, cmd, env);
    free(path);
    free_array(cmd);
    perror("Error in exceve()-ing in parent_process\n");
    return (0);
}

void middle_process(t_pipex *pipex, char **argv, char **env, int argc)
{
    char **cmd;
    char *path;
    int *pipe1 = (pipex)->pipefd[(pipex)->pipe_index];
    int *pipe2 = (pipex)->pipefd[(pipex)->pipe_index + 1];

    cmd = set_cmd_arguments(argv[(pipex)->cmd_index]); //cmd2
    path = ft_cmd_exits(env, cmd[0]);
    close(pipe1[1]);
    close(pipe2[0]);
    if (!path)
        perror("path:middle_process: "); 
    if (dup2(pipe1[0], 0) == -1)
        perror("dup2:pipe1: ");
    if (dup2(pipe2[1], 1) == -1)
        perror("dup2:pipe2: ");    
    printf("Middle-process\n");
    close(pipe1[0]);
    close(pipe2[1]);
    // pipex->pipe_index += 1; // moving to next pipe
    write(2, "cmd2\n", 5);
    execve(path, cmd, env); 
    free(path);
    free_array(cmd);
    perror("Error in exceve()-ing in child_process\n");
}

void pipeline(t_pipex *pipex, char **argv, char **env, int argc, int *std_fds)
{
    int i;
    int pid;

    i = 0;
    while (i < pipex->process_count - 2)
    {
        pid = fork();
        if (pid == -1)
            perror("Error in fork()-ing\n");
        if (pid == 0) // 
        {
            // printf("cmd_index = %d\n", pipex->cmd_index);
            pipex->cmd_index = pipex->cmd_index + 1;
            // pipex->pipe_index += 1; 
            printf("cmd_index after = %d\n", pipex->cmd_index);
            // printf("Middle-process\n");
            get_std_fds(1, std_fds);
            middle_process(pipex, argv, env, argc);
            exit(0);
        }
        else
        {
            pipex->pipe_index += 1;
        }
        i++;
    }
}

int **get_pipeline(t_pipex *pipex)
{
    int **pipeline;
    int *pipefd;
    int i;

    i = 0;
    pipeline = malloc(sizeof(int *) * pipex->process_count);
    while (i < pipex->process_count - 1)
    {
        pipefd = malloc(sizeof(int) * 2);
        if (!pipefd)
            perror("Error in pipe-malloc()-ing\n");
        pipeline[i] = pipefd;
        if (pipe(pipeline[i]) == -1)
        {
            perror("Error in pipe()-ing\n");
            exit(1);
        }
        i++;
    }
    printf("Total pipes created = %d | Total process created = %d\n", i, pipex->process_count);
    return (pipeline);
}

int count_cmds(char *argv[], int argc)
{
    int count;

    if (!strncmp(argv[1], "here_doc", 9))
        count = argc - 4;
    else    
        count = argc - 3;
    return (count);
}

int main(int argc, char *argv[], char *env[])
{
    t_pipex pipex;
    int *std_fds;
    int pid;

    pipex.process_count = 0;
    pipex.pipe_index = 0;

    // ./pipex infile cmd1 cmd2 cmd3 outfile
    if (argc >= 5)
    {   
        if (!strncmp(argv[1], "here_doc", 9))
            pipex.cmd_index = 3;
        else
            pipex.cmd_index = 2;
        pipex.process_count = count_cmds(argv, argc);
        pipex.pipefd = get_pipeline(&pipex);
        std_fds = get_std_fds(0, pipex.pipefd[0]); //returns NULL if fails
        pid = fork();
        if (pid == -1)
            perror("Error in fork()-ing\n");
        if (pid == 0) // first_process
        {
            if (!strncmp(argv[1], "here_doc", 9))
            {
                ft_here_doc(argv[2], pipex.pipefd[0], argv, env);
                printf("After here_doc cmd_index = %d \n", pipex.cmd_index);
                get_std_fds(1, std_fds);
                // free(std_fds);
            }
            else
            {
                printf("First-process\n");
                first_process(pipex.pipefd[0], argv, env);
            }
        }
        else // last_process
        {
            if (pipex.process_count > 2) // middle_process
            {
                wait(NULL);
                printf("Entering Pipeline...\n");
                get_std_fds(1, std_fds);
                pipex.pipe_index += 1;
                printf("cmd_index = %d\n", pipex.cmd_index);
                pipeline(&pipex, argv, env, argc, std_fds);
            }
            wait(NULL);
            get_std_fds(1, std_fds);
            write(2, "last\n", 6);
            last_process(&pipex, argv, env, argc); // free pipes_fd in this
        }
    }
}
