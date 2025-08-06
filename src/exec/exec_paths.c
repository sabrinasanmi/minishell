/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_paths.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makamins <makamins@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 15:07:43 by makamins          #+#    #+#             */
/*   Updated: 2025/08/06 16:59:21 by makamins         ###   ########.fr       */
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
	printf("DEBUG: entrando em find_cmd_in_paths com cmd = %s\n", cmd);
	while (paths[i])
	{
		printf("DEBUG: path %d = %s\n", i, paths[i]);
		full_path = construct_path(paths[i], cmd, gc);
		if (full_path && access(full_path, X_OK) == 0)
			return (full_path);
		
		i++;
	}
	printf("DEBUG: comando não encontrado em nenhum path\n");
	return (NULL);
}
// char	*find_cmd_in_paths(char **paths, char *cmd, t_garbage **gc)
// {
// 	char	*full_path;
// 	int		i;

// 	i = 0;
// 	while (paths[i])
// 	{
// 		if (!paths[i][0])
// 		{
// 			i++;
// 			continue;
// 		}
// 		full_path = construct_path(paths[i], cmd, gc);
// 		printf("DEBUG: trying path: %s\n", full_path);
// 		if (full_path && access(full_path, X_OK) == 0)
// 		{
// 			printf("DEBUG: FOUND at %s\n", full_path);
// 			return (full_path);
// 		}
// 		i++;
// 	}
// 	return (NULL);
// }

// Extrai o valor da variável PATH da env list, divide nos diretórios,
// e registra cada string no garbage collector.
// char	**get_paths(t_env *env, t_garbage **gc)
// {
// 	int		i;
// 	t_env	*path_node;
// 	char	**paths;

// 	path_node = find_env_key(env, "PATH");
// 	if (!path_node || !path_node->value)
// 	{
// 		fprintf(stderr, "ERRO: PATH não encontrado no ambiente\n");
// 		return (NULL);
// 	}
// 	printf("DEBUG: PATH = %s\n", path_node->value);
// 	paths = ft_split(path_node->value, ':');
// 	if (!paths || !gc_add_ptr(paths, gc))
// 		return (NULL);
// 	i = 0;
// 	while (paths[i])
// 	{
// 		if (paths[i][0] == '\0')
// 		{
// 			free(paths[i]);
// 			paths[i] = ft_strdup(".");
// 			if (!paths[i] || !gc_add_ptr(paths[i], gc))
// 				return (NULL);	
// 		}
// 		else if (!gc_add_ptr(paths[i], gc))
// 			return (NULL);
// 		i++;
// 	}
// 	return (paths);
// }

#include "minishell.h"
#include "garbage_collector.h"

// Extrai o valor da variável PATH da env list, divide nos diretórios,
// e registra cada string no garbage collector.


char	**get_paths(t_env *env, t_garbage **gc)
{
	int		i;
	t_env	*path_node;
	char	**paths;

	path_node = find_env_key(env, "PATH");
	if (!path_node || !path_node->value)
	{
		fprintf(stderr, "ERRO: PATH não encontrado no ambiente\n");
		return (NULL);
	}

	printf("DEBUG: PATH = %s\n", path_node->value);

	paths = ft_split(path_node->value, ':');
	if (!paths)
	{
		fprintf(stderr, "ERRO: ft_split falhou\n");
		return (NULL);
	}
	if (!gc_add_ptr(paths, gc))
	{
		fprintf(stderr, "ERRO: gc_add_ptr(paths) falhou\n");
		return (NULL);
	}

	i = 0;
	while (paths[i])
	{
		printf("DEBUG: paths[%d] = '%s'\n", i, paths[i]);

		if (paths[i][0] == '\0') // caso "::" no PATH, substitui por "."
		{
			printf("DEBUG: path vazio detectado (\"::\")\n");

			free(paths[i]);
			paths[i] = ft_strdup(".");
			if (!paths[i])
			{
				fprintf(stderr, "ERRO: ft_strdup falhou ao substituir por '.'\n");
				return (NULL);
			}
			if (!gc_add_ptr(paths[i], gc))
			{
				fprintf(stderr, "ERRO: gc_add_ptr(paths[%d]) falhou após substituir por '.'\n", i);
				return (NULL);
			}
		}
		else if (!gc_add_ptr(paths[i], gc))
		{
			fprintf(stderr, "ERRO: gc_add_ptr(paths[%d]) falhou\n", i);
			return (NULL);
		}
		i++;
	}

	return (paths);
}


// char	*get_cmd_path(char *cmd, t_env *env, t_garbage **gc)
// {
// 	char	**paths;
// 	char	*cmd_path;

// 	if (!cmd || !cmd[0])
// 		return (NULL);
// 	if (access(cmd, X_OK) == 0)
// 	{
// 		cmd_path = ft_strdup(cmd);
// 		if (!cmd_path || !gc_add_ptr(cmd_path, gc))
// 			return (NULL);
// 		return (cmd_path);
// 	}
// 	paths = get_paths(env, gc);
// 	if (!paths)
// 		return (NULL);
// 	cmd_path = find_cmd_in_paths(paths, cmd, gc);
// 	if (cmd_path && !gc_add_ptr(cmd_path, gc))
// 		return (NULL);
// 	return (cmd_path);
// }

char	*get_cmd_path(char *cmd, t_env *env, t_garbage **gc)
{
	char	**paths;
	char	*cmd_path;

	if (!cmd || !cmd[0])
	{
		printf("DEBUG: cmd vazio ou nulo em get_cmd_path\n");
		return (NULL);
	}
	if (access(cmd, X_OK) == 0)
	{
		printf("DEBUG: cmd é caminho executável direto\n");
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
