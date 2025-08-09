/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makamins <makamins@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 16:53:15 by sabsanto          #+#    #+#             */
/*   Updated: 2025/08/08 14:05:44 by makamins         ###   ########.fr       */
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
static int	handle_redirection(t_token **current,
	t_commands *cmd, t_garbage **gc)
{
	t_redir_type	redir_type;
	t_redir			*new_redir;
	t_token			*redir_token;
	t_token			*file_token;

	redir_token = *current;
	redir_type = get_redir_type(redir_token->type);
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
	*current = file_token->next;
	return (0);
}

static int	process_command_tokens(t_token **current,
	t_commands *cmd, t_garbage **gc, t_minishell *mini)
{
	char	*expanded;

	while (*current && (*current)->type != T_PIPE)
	{
		if ((*current)->type == T_WORD)
		{
			expanded = expand_variables((*current)->value, mini);
			if (!expanded)
				return (-1);
			if (add_arg_to_command(cmd, expanded, gc) != 0)
				return (-1);
			*current = (*current)->next;
		}
		else if ((*current)->type >= T_REDIR_IN
			&& (*current)->type <= T_REDIR_HEREDOC)
		{
			if (handle_redirection(current, cmd, gc) != 0)
				return (-1);
		}
		else
			*current = (*current)->next;
	}
	return (0);
}

static t_commands	*parse_single_command(t_token **tokens,
	t_garbage **gc, t_minishell *mini)
{
	t_commands	*cmd;
	t_token		*current;

	cmd = create_command_node(gc);
	if (!cmd)
		return (NULL);
	current = *tokens;
	if (process_command_tokens(&current, cmd, gc, mini) != 0)
		return (NULL);
	*tokens = current;
	return (cmd);
}

// Função principal para parsear tokens em uma lista de comandos
t_commands	*parse_tokens_to_commands(t_token *tokens,
	t_garbage **gc, t_minishell *mini)
{
	t_commands	*cmd_list;
	t_commands	*new_cmd;
	t_token		*current;

	cmd_list = NULL;
	current = tokens;
	while (current)
	{
		new_cmd = parse_single_command(&current, gc, mini);
		if (!new_cmd)
			return (NULL);
		add_command_to_list(&cmd_list, new_cmd);
		if (current && current->type == T_PIPE)
			current = current->next;
	}
	return (cmd_list);
}
