/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makamins <makamins@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 12:16:50 by makamins          #+#    #+#             */
/*   Updated: 2025/08/07 19:36:23 by makamins         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "garbage_collector.h"

static int	check_n_flag(char **argv)
{
	int		i;
	int		j;

	i = 1;
	while (argv[i] && ft_strncmp(argv[i], "-n", 2) == 0)
	{
		j = 1;
		while (argv[i][j] == 'n')
			j++;
		if (argv[i][j] == '\0')
			i++;
		else
			break ;
	}
	return (i);
}

int	ft_echo(char **argv, t_minishell *mini)
{
	int		i;
	bool	newline;

	(void)mini;
	i = check_n_flag(argv);
	newline = true;
	if (i > 1)
		newline = false;
	while (argv[i])
	{
		write(STDOUT_FILENO, argv[i], ft_strlen(argv[i]));
		if (argv[i + 1])
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	if (newline)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}
