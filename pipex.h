#ifndef PIPEX_H
# define PIPEX_H

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>

#define BUFFER_SIZE 5

// libft
size_t	ft_strlen(const char *str);
int	    ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strchr(const char *s, int c);
char	*ft_strdup(const char *s1);
char	*ft_strjoin(char const *s1, char const *s2);
char    **ft_split(const char *s, char c);

// paths
char **set_cmd_arguments(char *cmd);
char **extract_path_env(char **env);
char *set_own_path(char *cmd);
char *ft_cmd_exits(char **env, char *cmd);

// errors
int ft_error(char *error_msg);
void free_array(char **array);

// here_doc
int ft_here_doc(char *limiter, int *pipefd, char **argv, char **env);


#endif