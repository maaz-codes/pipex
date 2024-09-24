/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maakhan <maakhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 18:50:20 by maakhan           #+#    #+#             */
/*   Updated: 2024/09/24 19:24:43 by maakhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	count_cmds(char *argv[], int argc)
{
	int	count;

	if (!ft_strncmp(argv[1], "here_doc", 9))
		count = argc - 4;
	else
		count = argc - 3;
	return (count);
}

char	**set_cmd_arguments(char *cmd)
{
	char	**cmd_args;

	cmd_args = ft_split(cmd, ' ');
	if (!cmd_args)
		return (NULL);
	return (cmd_args);
}

char	**extract_path_env(char **env)
{
	int		i;
	char	*path;
	char	**absoulute_paths;

	i = 0;
	path = NULL;
	if (!env)
		return (NULL);
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

char	*set_own_path(char *cmd)
{
	if (ft_strchr(cmd, '/'))
		return (cmd);
	return (NULL);
}

char	*ft_cmd_exits(char **env, char *cmd)
{
	char	**a_paths;
	char	*temp_str_join;
	char	*path;
	int		i;

	i = 0;
	path = NULL;
	a_paths = extract_path_env(env);
	if (!cmd || set_own_path(cmd) || !a_paths)
		return (free(a_paths), ft_strdup(cmd));
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
	return (free_array(a_paths), path);
}
