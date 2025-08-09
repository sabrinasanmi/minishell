/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_paths.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makamins <makamins@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 15:07:43 by makamins          #+#    #+#             */
/*   Updated: 2025/08/07 19:54:38 by makamins         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "garbage_collector.h"

char	*construct_path(char *dir, char *cmd, t_garbage **gc)
{
	size_t	path_len;
	char	*full_path;

	if (!dir || !cmd)
		return (NULL);
	path_len = ft_strlen(dir) + ft_strlen(cmd) + 2;
	full_path = gc_malloc(path_len, gc);
	if (!full_path)
		return (NULL);
	ft_strlcpy(full_path, dir, path_len);
	ft_strlcat(full_path, "/", path_len);
	ft_strlcat(full_path, cmd, path_len);
	return (full_path);
}

static int	process_path_entries(char **paths, t_garbage **gc)
{
	int	i;

	i = 0;
	while (paths[i])
	{
		if (paths[i][0] == '\0')
		{
			free(paths[i]);
			paths[i] = ft_strdup(".");
			if (!paths[i])
				return (0);
		}
		if (!gc_add_ptr(paths[i], gc))
			return (0);
		i++;
	}
	return (1);
}

static char	**split_and_register_paths(char *path_value, t_garbage **gc)
{
	char	**paths;

	paths = ft_split(path_value, ':');
	if (!paths)
		return (NULL);
	if (!gc_add_ptr(paths, gc))
	{
		free_array(paths);
		return (NULL);
	}
	if (!process_path_entries(paths, gc))
		return (NULL);
	return (paths);
}

char	**get_paths(t_env *env, t_garbage **gc)
{
	t_env	*path_node;

	if (!env || !gc)
		return (NULL);
	path_node = find_env_key(env, "PATH");
	if (!path_node || !path_node->value)
		return (NULL);
	return (split_and_register_paths(path_node->value, gc));
}

char	*get_cmd_path(char *cmd, t_env *env, t_garbage **gc)
{
	char	**paths;
	char	*cmd_path;

	if (!cmd || !cmd[0] || !gc)
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
		{
			cmd_path = gc_malloc(ft_strlen(cmd) + 1, gc);
			if (!cmd_path)
				return (NULL);
			ft_strlcpy(cmd_path, cmd, ft_strlen(cmd) + 1);
			return (cmd_path);
		}
		return (NULL);
	}
	paths = get_paths(env, gc);
	if (!paths)
		return (NULL);
	return (find_cmd_in_paths(paths, cmd, gc));
}
