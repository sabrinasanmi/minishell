/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabsanto <sabsanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:49:25 by sabsanto          #+#    #+#             */
/*   Updated: 2025/07/31 18:38:34 by sabsanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "garbage_collector.h"


// Função temporária para executar comandos simples (sem pipes)
static void	execute_simple_command(t_commands *cmd, t_minishell *mini)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return ;

	// Verifica se é um builtin
	if (ft_strncmp(cmd->argv[0], "echo", 5) == 0)
		mini->last_exit = ft_echo(cmd->argv, mini);
	else if (ft_strncmp(cmd->argv[0], "cd", 3) == 0)
		mini->last_exit = ft_cd(cmd->argv, mini);
	else if (ft_strncmp(cmd->argv[0], "pwd", 4) == 0)
		mini->last_exit = ft_pwd(mini);
	else if (ft_strncmp(cmd->argv[0], "export", 7) == 0)
		mini->last_exit = ft_export(cmd->argv, mini);
	else if (ft_strncmp(cmd->argv[0], "unset", 6) == 0)
		mini->last_exit = ft_unset(cmd->argv, mini);
	else if (ft_strncmp(cmd->argv[0], "env", 4) == 0)
		mini->last_exit = ft_env(mini);
	else if (ft_strncmp(cmd->argv[0], "exit", 5) == 0)
		mini->last_exit = ft_exit(cmd->argv, mini);
	else
		// Comando externo
		mini->last_exit = exec_cmd(cmd->argv, mini->env, &mini->gc);
}

// Processa a linha de comando
static void	process_command_line(char *input, t_minishell *mini)
{
	t_token		*tokens;
	t_commands	*commands;

	// Tokeniza a entrada (usando a função refatorada)
	tokens = tokenize(input, mini);
	if (!tokens)
		return ;
	
	// Parseia tokens em comandos
	commands = parse_tokens_to_commands(tokens, &mini->gc);
	if (!commands)
		return ;
	
	// Por enquanto, executa apenas comandos simples (sem pipes)
	if (commands && !commands->next)
		execute_simple_command(commands, mini);
	else if (commands && commands->next)
		printf("Pipes ainda não implementados!\n");
}

// limpeza libreadline e libtinfo 
static void cleanup_readline(void)
{
    clear_history();
    rl_clear_pending_input();
    rl_cleanup_after_signal();
    rl_reset_terminal(NULL);
}

int	main(void)
{
	char		*input;
	t_minishell	mini;

	// Inicializa a estrutura minishell
	mini = (t_minishell){0};
	mini.last_exit = 0;
	mini.in_fd = STDIN_FILENO;
	mini.out_fd = STDOUT_FILENO;
	init_env_list(&mini, __environ);

	while (1)
	{
		input = readline("minishell> ");
		if (!input)
		{
			printf("\nexit\n");
			break ;
		}
		if (*input)
		{
			add_history(input);
			process_command_line(input, &mini);
		}
		free(input);
		
		// Limpa o garbage collector a cada comando
		//gc_free_all(&mini.gc);
		// Re-inicializa o ambiente após limpar o GC
		//init_env_list(&mini, __environ);
	}
	
	// Chama a função de limpeza
    cleanup_readline();
	// Limpa tudo antes de sair
	gc_free_all(&mini.gc);
	return (mini.last_exit);
}

