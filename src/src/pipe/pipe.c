/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabsanto <sabsanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 16:46:32 by makamins          #+#    #+#             */
/*   Updated: 2025/08/08 22:03:15 by sabsanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "garbage_collector.h"

// Verifica se o comando é um builtin
int	is_builtin_cmd(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strncmp(cmd, "echo", 5) == 0)
		return (1);
	if (ft_strncmp(cmd, "cd", 3) == 0)
		return (1);
	if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "export", 7) == 0)
		return (1);
	if (ft_strncmp(cmd, "unset", 6) == 0)
		return (1);
	if (ft_strncmp(cmd, "env", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "exit", 5) == 0)
		return (1);
	return (0);
}

// Executa builtin no processo filho
int	execute_builtin(t_commands *cmd, t_minishell *mini)
{
	if (ft_strncmp(cmd->argv[0], "echo", 5) == 0)
		return (ft_echo(cmd->argv, mini));
	else if (ft_strncmp(cmd->argv[0], "cd", 3) == 0)
		return (ft_cd(cmd->argv, mini));
	else if (ft_strncmp(cmd->argv[0], "pwd", 4) == 0)
		return (ft_pwd(mini));
	else if (ft_strncmp(cmd->argv[0], "export", 7) == 0)
		return (ft_export(cmd->argv, mini));
	else if (ft_strncmp(cmd->argv[0], "unset", 6) == 0)
		return (ft_unset(cmd->argv, mini));
	else if (ft_strncmp(cmd->argv[0], "env", 4) == 0)
		return (ft_env(mini));
	else if (ft_strncmp(cmd->argv[0], "exit", 5) == 0)
		return (ft_exit(cmd->argv, mini));
	return (0);
}

static void	setup_pipes_and_redirections(t_commands *cmd,
	int prev_read_fd, int pipe_fd[2], t_minishell *mini)
{
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
	if (handle_redirections(cmd->redir, mini) == -1)
		exit(1);
}

void	child_procces_logic(t_commands *cmd,
	int prev_read_fd, int pipe_fd[2], t_minishell *mini)
{
	char	**envp;
	char	*cmd_path;
	int		status;

	setup_pipes_and_redirections(cmd, prev_read_fd, pipe_fd, mini);
	if (is_builtin_cmd(cmd->argv[0]))
	{
		status = execute_builtin(cmd, mini);
		exit(status);
	}
	envp = env_list_to_array(mini->env, &mini->gc_temp);
	if (!envp)
		exit(1);
	cmd_path = get_cmd_path(cmd->argv[0], mini->env, &mini->gc_temp);
	if (!cmd_path)
	{
		printf("%s: command not found\n", cmd->argv[0]);
		exit(127);
	}
	execve(cmd_path, cmd->argv, envp);
	perror(cmd->argv[0]);
	exit(127);
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
	{
		child_procces_logic(data->cmd, data->prev_read_fd,
			data->pipe_fd, mini);
	}
	else
	{
		parent_procces_logic(&data->prev_read_fd,
			data->pipe_fd, data->cmd);
		data->last_pid = data->pid;
		data->cmd = data->cmd->next;
		data->i++;
	}
	return (0);
}
