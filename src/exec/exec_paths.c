/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_paths.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makamins <makamins@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 15:07:43 by makamins          #+#    #+#             */
/*   Updated: 2025/08/05 16:39:31 by makamins         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "garbage_collector.h"

char	*construct_path(char *dir, char *cmd, t_garbage **gc)
{
	size_t	path_len;
	char	*full_path;

	path_len = ft_strlen(dir) + ft_strlen(cmd) + 2;
	full_path = gc_malloc(path_len, gc);
	if (!full_path)
		return (NULL);
	ft_strlcpy(full_path, dir, path_len);
	ft_strlcat(full_path, "/", path_len);
	ft_strlcat(full_path, cmd, path_len);
	return (full_path);
}

char	*find_cmd_in_paths(char **paths, char *cmd, t_garbage **gc)
{
	char	*full_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		full_path = construct_path(paths[i], cmd, gc);
		printf("DEBUG: trying %s\n", full_path);
		if (full_path && access(full_path, X_OK) == 0)
			return (full_path);
		i++;
	}
	return (NULL);
}

// Extrai o valor da variável PATH da env list, divide nos diretórios,
// e registra cada string no garbage collector.


char	**get_paths(t_env *env, t_garbage **gc)
{
	int		i;
	t_env	*path_node;
	char	**paths;

	path_node = find_env_key(env, "PATH");
	if (!path_node || !path_node->value)
		return (NULL);
	paths = ft_split(path_node->value, ':');
	if (!paths || !gc_add_ptr(paths, gc))
		return (NULL);
	i = 0;
	while (paths[i])
	{
		if (!gc_add_ptr(paths[i], gc))
			return (NULL);
		i++;
	}
	return (paths);
}

char	*get_cmd_path(char *cmd, t_env *env, t_garbage **gc)
{
	char	**paths;
	char	*cmd_path;

	if (!cmd || !cmd[0])
		return (NULL);
	printf("DEBUG: acesso direto a '%s'? -> %d\n", cmd, access(cmd, X_OK));
	if (access(cmd, X_OK) == 0)
	{
		cmd_path = ft_strdup(cmd);
		if (!cmd_path || !gc_add_ptr(cmd_path, gc))
			return (NULL);
		return (cmd_path);
	}
	paths = get_paths(env, gc);
	if (!paths)
	{
		printf("DEBUG: get_paths retornou NULL\n");
		return (NULL);
	}
	printf("DEBUG: chamando find_cmd_in_paths() com cmd = '%s'\n", cmd);
	cmd_path = find_cmd_in_paths(paths, cmd, gc);
	if (cmd_path && !gc_add_ptr(cmd_path, gc))
		return (NULL);
	return (cmd_path);
}
