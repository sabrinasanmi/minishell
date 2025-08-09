/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_main2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabsanto <sabsanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 13:54:54 by makamins          #+#    #+#             */
/*   Updated: 2025/08/08 20:51:58 by sabsanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "garbage_collector.h"

void	execute_external_command(t_commands *cmd, t_minishell *mini)
{
	pid_t	pid;
	int		status;

	// Ignora sinais no pai durante execução do filho
	setup_signals_ignore();
	
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		mini->last_exit = 1;
		setup_signals_interactive(); // Restaura sinais
		return ;
	}
	else if (pid == 0)
		child_process_exec(cmd, mini);
	else
	{
		if (waitpid(pid, &status, 0) == -1)
		{
			perror("waitpid");
			mini->last_exit = 1;
		}
		else
			handle_child_exit_status(status, mini);
		
		// Restaura sinais interativos após processo filho terminar
		setup_signals_interactive();
	}
}

void	execute_simple_command(t_commands *cmd, t_minishell *mini)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return ;
	if (is_builtin_cmd(cmd->argv[0]))
		execute_builtin_with_redirections(cmd, mini);
	else
		execute_external_command(cmd, mini);
}
