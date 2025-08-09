/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabsanto <sabsanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 15:35:04 by makamins          #+#    #+#             */
/*   Updated: 2025/08/08 22:57:41 by sabsanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "garbage_collector.h"

char	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r'
		|| c == '\f' || c == '\v');
}

char	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '&' || c == ';' || c == '\\');
}

int	ft_atol(const char *nbr)
{
	long	i;
	int		isminus;
	long	res;

	i = 0;
	while (nbr[i] == 32 || (nbr[i] >= 9 && nbr[i] <= 13))
		i++;
	isminus = 1;
	if (nbr[i] == '-' || nbr[i] == '+')
	{
		if (nbr[i] == '-')
			isminus *= -1;
		i++;
	}
	res = 0;
	while (nbr[i] >= '0' && nbr[i] <= '9')
	{
		res = res * 10 + (nbr[i] - '0');
		i++;
	}
	return (res * isminus);
}

bool	is_numeric_arg(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (false);
	if (str[0] == '-' || str[0] == '+')
		i++;
	if (str[i] == '\0')
		return (false);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (false);
		i++;
	}
	return (true);
}

void	print_export_error(const char *arg)
{
	write(2, "export: `", 9);
	write(2, arg, ft_strlen(arg));
	write(2, "': not a valid identifier\n", 27);
}
