/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maakhan <maakhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 18:50:09 by maakhan           #+#    #+#             */
/*   Updated: 2024/09/25 08:14:10 by maakhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line/get_next_line.h"
#include "pipex.h"

static void	waiting(int argc, char **argv)
{
	int	i;

	i = 1;
	while (i <= count_cmds(argv, argc) - 1)
	{
		wait(NULL);
		i++;
	}
}

int	main(int argc, char *argv[], char *env[])
{
	pid_t	pid_last;
	int		pipefd[2];
	int		status;

	if (argc >= 5)
	{
		if (pipe(pipefd) == -1)
			(print_error());
		if (!ft_strncmp(argv[1], "here_doc", 9))
			ft_here_doc(argv[2], argv, env, pipefd);
		else
			first_process(argv, env, pipefd);
		pipeline(argc, argv, env, pipefd);
		pid_last = last_process(argc, argv, env, pipefd);
		waitpid(pid_last, &status, 0);
		waiting(argc, argv);
		return (WEXITSTATUS(status));
	}
	return (EXIT_FAILURE);
}
