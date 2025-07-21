/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabsanto <sabsanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 20:04:28 by sabsanto          #+#    #+#             */
/*   Updated: 2025/07/10 14:36:47 by sabsanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	tokenize(char *input)
{
	int	i;
	int	len;
	int	tok_len;
	int	start;
	char	*token;

	i = 0;
	len = ft_strlen(input);
	while (i < len)
	{
		if (is_space(input[i]))
		{
			i++;
			continue ;
		}
		if (is_operator(input[i]))
		{
			if ((input[i] == '>' || input[i] == '<') && input[i + 1] == input[i])
			{
				printf("Token: %c%c\n", input[i], input[i + 1]);
				i += 2;
			}
			else
			{
				printf("Token: %c\n", input[i]);
				i++;
			}
		}
		else if (input[i] == '\'' || input[i] == '"')
		{
			token = extract_quoted_token(input, &i);
			if (!token)
				return ;
			printf("Token: %s\n", token);
			free(token);
		}
		else
		{
			start = i;
			while (i < len && !is_space(input[i]) && !is_operator(input[i])
				&& input[i] != '\'' && input[i] != '"')
				i++;
			tok_len = i - start;
			token = malloc(tok_len + 1);
			if (!token)
				return ;
			ft_strlcpy(token, &input[start], tok_len + 1);
			printf("Token: %s\n", token);
			free(token);
		}
	}
}
