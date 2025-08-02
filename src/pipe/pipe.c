/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makamins <makamins@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 16:46:32 by makamins          #+#    #+#             */
/*   Updated: 2025/07/31 18:30:35 by makamins         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "garbage_collector.h"

void	setup_initial_vars(t_exec_data *data, t_commands *cmd_list)
{
	data->num_cmds = count_commands(cmd_list);
	data->i = 0;
	data->prev_read_fd = -1;
	data->cmd = cmd_list;
}

int	create_pipe_if_needed(t_commands *cmd, int pipe_fd[2])
{
	if (cmd->next)
	{
		if (pipe(pipe_fd) == -1)
		{
			perror("pipe");
			return (-1);
		}
	}
	return (0);
}

void	child_procces_logic(t_commands *cmd, int prev_read_fd, 
	int pipe_fd[2], t_minishell *mini)
{
	char	**envp;

	if (prev_read_fd != -1)
	{
		dup2(prev_read_fd, STDIN_FILENO);
		close(prev_read_fd);
	}
	if (cmd->next)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
	}
	if (handle_redirections(cmd->redir) == -1)
		exit(1);
	envp = env_list_to_array(mini->env, &mini->gc);
	if (!envp)
		exit(1);
	execve(cmd->argv[0], cmd->argv, envp);
	perror(cmd->argv[0]);
	exit(127);
}

void	parent_procces_logic(int *prev_read_fd,
	int pipe_fd[2], t_commands *cmd)
{
	if (*prev_read_fd != -1)
		close(*prev_read_fd);
	if (cmd->next)
	{
		close(pipe_fd[1]);
		*prev_read_fd = pipe_fd[0];
	}
}

int	exec_single_command(t_exec_data *data, t_minishell *mini)
{
	if (create_pipe_if_needed(data->cmd, data->pipe_fd) == -1)
		return (1);
	data->pid = fork();
	if (data->pid < 0)
	{
		perror("fork");
		return (1);
	}
	else if (data->pid == 0)
		child_procces_logic(data->cmd, data->prev_read_fd,
			data->pipe_fd, mini);
	else
			parent_procces_logic(&data->prev_read_fd,
				data->pipe_fd, data->cmd);
	data->cmd = data->cmd->next;
	data->i++;
	return (0);
}
