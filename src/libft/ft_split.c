/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makamins <makamins@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 20:55:45 by makamins          #+#    #+#             */
/*   Updated: 2025/08/07 19:26:45 by makamins         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_words(const char *s, char c)
{
	int	count;
	int	i;

	if (!s)
		return (0);
	count = 1;
	i = 0;
	while (s[i])
	{
		if (s[i] == c)
			count++;
		i++;
	}
	return (count);
}

static char	*copy_word(const char *start, int len)
{
	char	*word;
	int		i;

	word = (char *)malloc((len + 1) * sizeof(char));
	if (!word)
		return (NULL);
	i = 0;
	while (i < len)
	{
		word[i] = start[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

static void	free_split(char **array, int i)
{
	while (i > 0)
	{
		i--;
		free(array[i]);
	}
	free(array);
}

static int	extract_words(char **result, const char *s, char c)
{
	int	i;
	int	len;

	i = 0;
	while (*s)
	{
		len = 0;
		while (s[len] && s[len] != c)
			len++;
		result[i] = copy_word(s, len);
		if (!result[i])
		{
			free_split(result, i);
			return (0);
		}
		i++;
		s += len;
		if (*s == c)
			s++;
	}
	result[i] = NULL;
	return (1);
}

char	**ft_split(char const *s, char c)
{
	char	**result;
	int		words_count;

	if (!s)
		return (NULL);
	words_count = count_words(s, c);
	result = (char **)malloc((words_count + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	if (!extract_words(result, s, c))
	{
		return (NULL);
	}
	return (result);
}
