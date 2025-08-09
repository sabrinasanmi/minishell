/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabsanto <sabsanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 14:34:21 by sabsanto          #+#    #+#             */
/*   Updated: 2025/08/08 23:00:24 by sabsanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "garbage_collector.h"

char	*extract_quoted_token(char *input, int *i, t_minishell *mini)
{
	int		start;
	int		len;
	char	quote_type;
	char	*token;

	quote_type = input[*i];
	(*i)++;
	start = *i;
	while (input[*i] && input[*i] != quote_type)
		(*i)++;
	if (input[*i] != quote_type)
	{
		write(2, "minishell: syntax error: unclosed quotes\n", 42);
		return (NULL);
	}
	len = *i - start;
	token = gc_malloc(len + 1, &mini->gc_temp);
	if (!token)
		return (NULL);
	ft_strlcpy(token, &input[start], len + 1);
	if (input[*i] == quote_type)
		(*i)++;
	return (token);
}

// Cria um novo token
t_token	*create_token(char *value, t_tokens type, t_garbage **gc)
{
	t_token	*token;

	token = gc_malloc(sizeof(t_token), gc);
	if (!token)
		return (NULL);
	token->value = value;
	token->type = type;
	token->next = NULL;
	return (token);
}

// Adiciona token à lista
void	add_token_to_list(t_token **list, t_token *new_token)
{
	t_token	*last;

	if (!list || !new_token)
		return ;
	if (!*list)
	{
		*list = new_token;
		return ;
	}
	last = *list;
	while (last->next)
		last = last->next;
	last->next = new_token;
}

// Determina o tipo do operador
t_tokens	get_operator_type(char *input, int pos)
{
	if (input[pos] == '|')
		return (T_PIPE);
	else if (input[pos] == '<')
	{
		if (input[pos + 1] == '<')
			return (T_REDIR_HEREDOC);
		return (T_REDIR_IN);
	}
	else if (input[pos] == '>')
	{
		if (input[pos + 1] == '>')
			return (T_REDIR_APPEND);
		return (T_REDIR_OUT);
	}
	return (T_WORD);
}

static int	is_invalid_operator(char *input, int pos)
{
	// Detecta && e || como operadores inválidos
	if (input[pos] == '&' && input[pos + 1] == '&')
		return (1);
	if (input[pos] == '|' && input[pos + 1] == '|')
		return (1);
	if (input[pos] == ';')
		return (1);
	if (input[pos] == '\\')
		return (1);
	return (0);
}

void	process_operator_token(char *input, int *i,
		t_token **tokens, t_garbage **gc)
{
	t_tokens	type;
	t_token		*new_token;

	// CORRIGIDO: avança o ponteiro e sai da função
	if (is_invalid_operator(input, *i))
	{
		write(2, "minishell: syntax error near unexpected token\n", 47);
		// IMPORTANTE: avançar o ponteiro para evitar loop infinito
		if (input[*i] == '&' && input[*i + 1] == '&')
			*i += 2;
		else if (input[*i] == '|' && input[*i + 1] == '|')
			*i += 2;
		else
			(*i)++;
		return ;
	}

	type = get_operator_type(input, *i);
	if ((input[*i] == '>' || input[*i] == '<') && input[*i + 1] == input[*i])
		*i += 2;
	else
		(*i)++;
	new_token = create_token(NULL, type, gc);
	if (new_token)
		add_token_to_list(tokens, new_token);
}
