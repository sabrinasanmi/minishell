/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabsanto <sabsanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 14:34:21 by sabsanto          #+#    #+#             */
/*   Updated: 2025/07/10 14:55:08 by sabsanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extract_quoted_token(char *input, int *i)
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
	len = *i - start;
	token = malloc(len + 1);
	if (!token)
		return (NULL);
	ft_strlcpy(token, &input[start], len + 1);
	if (input[*i] == quote_type)
		(*i)++;
	return (token);
}
