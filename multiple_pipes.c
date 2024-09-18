#include "pipex.h"

void execute_pipeline(char **argv, char **env, int num_cmds, int argc) 
{
    int pipes[num_cmds - 1][2];
    int pid;
    int i;

    // Create pipes
    for (i = 0; i < num_cmds - 1; i++) 
    {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            exit(1);
        }
    }

    // Fork and execute commands
    for (i = 0; i < num_cmds; i++) 
    {
            pid = fork();
            if (pid == 0) 
            {
                // Child process

                // Redirect input
                if (i == 0) 
                {
                    // First command: input from file
                    int infile = open(argv[1], O_RDONLY);
                    if (infile == -1) 
                    {
                        perror("Error opening infile");
                        exit(1);
                    }
                    dup2(infile, 0);  // stdin
                    close(infile);
                } 
                else 
                {
                    // Intermediate/Last command: input from previous pipe
                    dup2(pipes[i - 1][0], 0);  // stdin
                }

                // Redirect output
                if (i == num_cmds - 1) 
                {
                    // Last command: output to file
                    int outfile = open(argv[argc - 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
                    if (outfile == -1) 
                    {
                        perror("Error opening outfile");
                        exit(1);
                    }
                    dup2(outfile, 1);  // stdout
                    close(outfile);
                } 
                else 
                {
                    // Intermediate/First command: output to next pipe
                    dup2(pipes[i][1], 1);  // stdout
                }

                // Close all pipe file descriptors in child
                for (int j = 0; j < num_cmds - 1; j++) 
                {
                    close(pipes[j][0]);
                    close(pipes[j][1]);
                }

                // Execute the command
                char **cmd = set_cmd_arguments(argv[i + 2]);
                char *path = ft_cmd_exits(env, cmd[0]);
                execve(path, cmd, env);
                perror("execve failed");
                exit(127);
            }
    }

    // Close all pipe file descriptors in parent
    for (i = 0; i < num_cmds - 1; i++) 
    {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // Wait for all children to finish
    for (i = 0; i < num_cmds; i++) 
    {
        wait(NULL);
    }
}
