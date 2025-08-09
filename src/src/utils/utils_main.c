/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabsanto <sabsanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 13:12:19 by makamins          #+#    #+#             */
/*   Updated: 2025/08/08 20:55:46 by sabsanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "garbage_collector.h"

void	save_std_fds(int *saved_stdin, int *saved_stdout)
{
	*saved_stdin = dup(STDIN_FILENO);
	*saved_stdout = dup(STDOUT_FILENO);
}

void	restore_std_fds(int saved_stdin, int saved_stdout)
{
	if (saved_stdin != -1)
	{
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
	}
	if (saved_stdout != -1)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
}

void	execute_builtin_with_redirections(t_commands *cmd, t_minishell *mini)
{
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = -1;
	saved_stdout = -1;
	if (cmd->redir)
		save_std_fds(&saved_stdin, &saved_stdout);
	if (cmd->redir && handle_redirections(cmd->redir, mini) == -1)
	{
		mini->last_exit = 1;
		restore_std_fds(saved_stdin, saved_stdout);
		return ;
	}
	mini->last_exit = execute_builtin(cmd, mini);
	restore_std_fds(saved_stdin, saved_stdout);
}

void	child_process_exec(t_commands *cmd, t_minishell *mini)
{
	char	**envp;
	char	*cmd_path;

	setup_signals_child();

	if (cmd->redir && handle_redirections(cmd->redir, mini) == -1)
		exit(1);
	envp = env_list_to_array(mini->env, &mini->gc_temp);
	if (!envp)
		exit(1);
	cmd_path = get_cmd_path(cmd->argv[0], mini->env, &mini->gc_temp);
	if (!cmd_path)
	{
		write(2, cmd->argv[0], ft_strlen(cmd->argv[0]));
		write(2, ": command not found\n", 20);
		exit(127);
	}
	execve(cmd_path, cmd->argv, envp);
	perror("execve");
	exit(126);
}

void	handle_child_exit_status(int status, t_minishell *mini)
{
	if (WIFEXITED(status))
		mini->last_exit = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		int sig = WTERMSIG(status);
		
		// Adiciona quebra de linha para sinais que interrompem o processo
		if (sig == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		else if (sig == SIGQUIT)
			write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
		
		mini->last_exit = 128 + sig;
	}
}
