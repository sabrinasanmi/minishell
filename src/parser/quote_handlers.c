/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabsanto <sabsanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 21:21:58 by sabsanto          #+#    #+#             */
/*   Updated: 2025/07/21 21:23:21 by sabsanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_single_quotes(char *input, int *i)
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
	token = malloc(len + 1);
	if (!token)
		return (NULL);
	ft_strlcpy(token, &input[start], len + 1);
	(*i)++;
	return (token);
}

char	*handle_double_quotes(char *input, int *i, t_minishell *mini)
{
	int		start;
	int		len;
	char	*raw_token;
	char	*expanded_token;

	(*i)++;
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
		return (NULL);
	}
	len = *i - start;
	raw_token = extract_raw_content(input, start, len, &mini->gc);
	if (!raw_token)
		return (NULL);
	expanded_token = expand_variables(raw_token, mini);
	(*i)++;
	return (expanded_token);
}
