/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makamins <makamins@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 19:13:48 by makamins          #+#    #+#             */
/*   Updated: 2025/07/30 19:33:03 by makamins         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "garbage_collector.h"

int	fork_and_exec(char *cmd_path, char **args, char **envp)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (1);
	}
	if (pid == 0)
	{
		execve(cmd_path, args, envp);
		perror("execve");
		exit (1);
	}
	if (waitpid(pid, &status, 0) == -1)
	{
		perror("waitpid");
		return (1);
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

int	exec_cmd(char **args, t_env *env, t_garbage **gc)
{
	char	*cmd_path;
	char	**env_array;

	if (!args || !args[0])
		return (1);
	env_array = env_list_to_array(env, gc);
	if (!env_array)
		return (1);
	cmd_path = get_cmd_path(args[0], env, gc);
	if (!cmd_path)
	{
		printf("%s: command not found\n", args[0]);
		return (127);
	}
	return (fork_and_exec(cmd_path, args, env_array));
}
