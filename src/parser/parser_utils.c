/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabsanto <sabsanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 14:34:21 by sabsanto          #+#    #+#             */
/*   Updated: 2025/07/22 20:43:43 by sabsanto         ###   ########.fr       */
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
	token = gc_malloc(len + 1, &mini->gc);
	if (!token)
		return (NULL);
	ft_strlcpy(token, &input[start], len + 1);
	if (input[*i] == quote_type)
		(*i)++;
	return (token);
}
