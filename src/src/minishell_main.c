/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabsanto <sabsanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:49:25 by sabsanto          #+#    #+#             */
/*   Updated: 2025/08/08 21:02:57 by sabsanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "garbage_collector.h"

volatile sig_atomic_t	g_signal_received = 0;

static void	process_command_line(char *input, t_minishell *mini)
{
	t_token		*tokens;
	t_commands	*cmd_list;

	tokens = tokenize(input, mini);
	if (!tokens)
		return ;
	cmd_list = parse_tokens_to_commands(tokens, &mini->gc_temp, mini);
	if (!cmd_list)
		return ;
	mini->commands = cmd_list;
	if (cmd_list->next)
		mini->last_exit = execute_pipeline(cmd_list, mini);
	else
		execute_simple_command(cmd_list, mini);
}

static void	init_minishell(t_minishell *mini, char **envp)
{
	mini->gc_persistent = NULL;
	mini->gc_temp = NULL;
	mini->commands = NULL;
	mini->env = NULL;
	mini->last_exit = 0;
	mini->in_fd = dup(STDIN_FILENO);
	mini->out_fd = dup(STDOUT_FILENO);
	init_env_list(mini, envp);
	setup_signals_interactive();
}

static int	process_input(t_minishell *mini)
{
	char	*input;

	input = readline("minishell> ");
	if (!input)
	{
		write(1, "exit\n", 5);
		return (0);
	}
	if (*input == '\0')
	{
		free(input);
		return (1);
	}
	add_history(input);
	process_command_line(input, mini);
	free(input);
	/*if (g_signal_received)
	{
		if (g_signal_received == SIGINT)
			mini->last_exit = 130;
		g_signal_received = 0;
	}*/
	return (1);
}

static void	run_minishell(t_minishell *mini)
{
	while (1)
	{
		gc_free_all(&mini->gc_temp);
		if (!process_input(mini))
			break ;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell	mini;

	(void)argc;
	(void)argv;
	init_minishell(&mini, envp);
	run_minishell(&mini);
	gc_free_all(&mini.gc_persistent);
	gc_free_all(&mini.gc_temp);
	dup2(mini.in_fd, STDIN_FILENO);
	dup2(mini.out_fd, STDOUT_FILENO);
	close(mini.in_fd);
	close(mini.out_fd);
	return (mini.last_exit);
}
