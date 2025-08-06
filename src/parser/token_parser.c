/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabsanto <sabsanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 16:53:15 by sabsanto          #+#    #+#             */
/*   Updated: 2025/08/06 17:20:31 by sabsanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Determina o tipo de redirecionamento baseado no token
static t_redir_type	get_redir_type(t_tokens token_type)
{
	if (token_type == T_REDIR_IN)
		return (REDIR_IN);
	else if (token_type == T_REDIR_OUT)
		return (REDIR_OUT);
	else if (token_type == T_REDIR_APPEND)
		return (REDIR_APPEND);
	else if (token_type == T_REDIR_HEREDOC)
		return (REDIR_HEREDOC);
	return (REDIR_IN);
}

// Processa um token de redirecionamento
static int	handle_redirection(t_token **current, t_commands *cmd, t_garbage **gc)
{
	t_redir_type	redir_type;
	t_redir			*new_redir;
	t_token			*redir_token;
	t_token			*file_token;

	redir_token = *current;
	redir_type = get_redir_type(redir_token->type);

	// O próximo token deve ser o arquivo/delimitador
	file_token = redir_token->next;
	if (!file_token || file_token->type != T_WORD)
	{
		write(2, "minishell: syntax error near unexpected token\n", 47);
		return (1);
	}
	
	new_redir = create_redir_node(redir_type, file_token->value, gc);
	if (!new_redir)
		return (1);
		
	add_redir_to_command(cmd, new_redir);
	
	// Avança para o próximo token após o arquivo
	*current = file_token->next;
	return (0);
}

// Processa tokens para um único comando
static t_commands	*parse_single_command(t_token **tokens, t_garbage **gc)
{
	t_commands	*cmd;
	t_token		*current;

	cmd = create_command_node(gc);
	if (!cmd)
		return (NULL);

	current = *tokens;
	while (current && current->type != T_PIPE)
	{
		if (current->type == T_WORD)
		{
			if (add_arg_to_command(cmd, current->value, gc) != 0)
				return (NULL);
			current = current->next;
		}
		else if (current->type >= T_REDIR_IN && current->type <= T_REDIR_HEREDOC)
		{
			if (handle_redirection(&current, cmd, gc) != 0)
				return (NULL);
		}
		else
			current = current->next;
	}
	
	*tokens = current;
	return (cmd);
}

// Função principal para parsear tokens em uma lista de comandos
t_commands	*parse_tokens_to_commands(t_token *tokens, t_garbage **gc)
{
	t_commands	*cmd_list;
	t_commands	*new_cmd;
	t_token		*current;

	cmd_list = NULL;
	current = tokens;

	while (current)
	{
		new_cmd = parse_single_command(&current, gc);
		if (!new_cmd)
			return (NULL);
		add_command_to_list(&cmd_list, new_cmd);
		
		// Se encontrou um pipe, avança para o próximo comando
		if (current && current->type == T_PIPE)
			current = current->next;
	}
	
	return (cmd_list);
}

// Função auxiliar para debug - imprime a estrutura de comandos
void	print_command_structure(t_commands *cmd_list)
{
	t_commands	*cmd;
	t_redir		*redir;
	int			cmd_num;
	int			arg_num;

	cmd_num = 1;
	cmd = cmd_list;
	
	printf("\n=== Command Structure ===\n");
	
	while (cmd)
	{
		printf("Command %d:\n", cmd_num);
		
		// Imprime argumentos
		if (cmd->argv)
		{
			printf("  Arguments:\n");
			arg_num = 0;
			while (cmd->argv[arg_num])
			{
				printf("    [%d] %s\n", arg_num, cmd->argv[arg_num]);
				arg_num++;
			}
		}
		else
		{
			printf("  No arguments\n");
		}
		
		// Imprime redirecionamentos
		if (cmd->redir)
		{
			printf("  Redirections:\n");
			redir = cmd->redir;
			while (redir)
			{
				printf("    Type: ");
				if (redir->type == REDIR_IN)
					printf("< (input)");
				else if (redir->type == REDIR_OUT)
					printf("> (output)");
				else if (redir->type == REDIR_APPEND)
					printf(">> (append)");
				else if (redir->type == REDIR_HEREDOC)
					printf("<< (heredoc)");
				printf(", File: %s\n", redir->file);
				redir = redir->next;
			}
		}
		else
		{
			printf("  No redirections\n");
		}
		
		// Indica se há mais comandos (pipe)
		if (cmd->next)
			printf("  | (pipe to next command)\n");
			
		cmd = cmd->next;
		cmd_num++;
	}
	
	printf("=== End of Structure ===\n\n");
}
