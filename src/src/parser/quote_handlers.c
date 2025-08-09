/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabsanto <sabsanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 21:21:58 by sabsanto          #+#    #+#             */
/*   Updated: 2025/08/08 18:31:54 by sabsanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_single_quotes(char *input, int *i, t_minishell *mini)
{
	int		start;
	int		len;
	char	*token;

	(*i)++;
	start = *i;
	while (input[*i] && input[*i] != '\'')
		(*i)++;
	if (input[*i] != '\'')
	{
		write(2, "minishell: syntax error: unclosed quotes\n", 42);
		return (NULL);
	}
	len = *i - start;
	token = gc_malloc(len + 2, &mini->gc_temp);
	if (!token)
		return (NULL);
	token[0] = '\1';
	ft_strlcpy(token + 1, &input[start], len + 1);
	(*i)++;
	return (token);
}

static int	find_closing_double_quote(char *input, int *i)
{
	int	start;

	start = *i;
	while (input[*i] && input[*i] != '"')
	{
		if (input[*i] == '\\' && input[*i + 1] == '"')
			*i += 2;
		else
			(*i)++;
	}
	if (input[*i] != '"')
	{
		write(2, "minishell: syntax error: unclosed quotes\n", 42);
		return (-1);
	}
	return (*i - start);
}

char	*handle_double_quotes(char *input, int *i, t_minishell *mini)
{
	int		start;
	int		len;
	char	*raw_token;
	char	*expanded_token;

	(*i)++;
	start = *i;
	len = find_closing_double_quote(input, i);
	if (len == -1)
		return (NULL);
	raw_token = extract_raw_content(input, start, len, &mini->gc_temp);
	if (!raw_token)
		return (NULL);
	expanded_token = expand_variables(raw_token, mini);
	(*i)++;
	return (expanded_token);
}
