/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makamins <makamins@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 12:16:50 by makamins          #+#    #+#             */
/*   Updated: 2025/07/14 15:27:53 by makamins         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "garbage_collector.h"

int	ft_echo(char **argv, t_minishell *mini)
{
	int	i;
	bool	new_line;
	
	(void)mini;
	i = 1;
	new_line = true;
	while (argv[i] && !ft_strncmp(argv[i], "-n", 3))
	{
		new_line = false;
		i++;	
	}
	while (argv[i])
	{
		write (1, argv[i], ft_strlen(argv[i]));
		if (argv[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (new_line)
		write(1, "\n", 1);
	return (0);
}
