/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maakhan <maakhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 18:52:06 by maakhan           #+#    #+#             */
/*   Updated: 2024/09/24 19:26:29 by maakhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line/get_next_line.h"
#include "pipex.h"

static void	read_write(char *limiter, int write_to)
{
	char	*new_limiter;
	char	*line;

	new_limiter = ft_strjoin(limiter, "\n");
	line = get_next_line(0);
	while (line)
	{
		if (!ft_strncmp(line, new_limiter, ft_strlen(new_limiter)))
			break ;
		write(write_to, line, ft_strlen(line));
		free(line);
		line = get_next_line(0);
	}
	free(new_limiter);
	close(write_to);
}

int	ft_here_doc(char *limiter, char **argv, char **env, int *pipefd)
{
	pid_t	pid;
	char	**cmd;
	int		doc_pipe[2];

	pipe(doc_pipe);
	write(1, "<here_doc/>\n", 12);
	read_write(limiter, doc_pipe[1]);
	pid = fork();
	if (pid == -1)
		(close(pipefd[0]), close(pipefd[1]), print_error());
	if (pid == 0)
	{
		cmd = set_cmd_arguments(argv[3]);
		if (!cmd)
			(close(pipefd[0]), close(pipefd[1]), print_error());
		execute(cmd, env, doc_pipe[0], pipefd[1]);
	}
	return (pid);
}
