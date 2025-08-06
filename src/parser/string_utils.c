/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabsanto <sabsanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 21:24:46 by sabsanto          #+#    #+#             */
/*   Updated: 2025/08/06 18:55:11 by sabsanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*join_strings(char *s1, char *s2, t_garbage **gc)
{
	char	*result;
	int		len1;
	int		len2;

	if (!s1 || !s2)
		return (s1);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	result = gc_malloc(len1 + len2 + 1, gc);
	if (!result)
		return (s1);
	ft_strlcpy(result, s1, len1 + 1);
	ft_strlcat(result, s2, len1 + len2 + 1);
	return (result);
}

char	*append_char(char *str, char c, t_garbage **gc)
{
	char	*result;
	int		len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	result = gc_malloc(len + 2, gc);
	if (!result)
		return (str);
	ft_strlcpy(result, str, len + 1);
	result[len] = c;
	result[len + 1] = '\0';
	return (result);
}
