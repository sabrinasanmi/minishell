/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_buf.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makamins <makamins@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:16:38 by makamins          #+#    #+#             */
/*   Updated: 2025/08/08 14:17:07 by makamins         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_reverse_str(char *buf)
{
	int		i;
	int		j;
	char	tmp;

	i = 0;
	while (buf[i] != '\0')
		i++;
	i--;
	j = 0;
	while (j < i)
	{
		tmp = buf[j];
		buf[j] = buf[i];
		buf[i] = tmp;
		j++;
		i--;
	}
}

static int	ft_putnbr_buf(long num, char *buf)
{
	int	i;

	i = 0;
	if (num == 0)
		buf[i++] = '0';
	else
	{
		while (num > 0)
		{
			buf[i++] = (num % 10) + '0';
			num /= 10;
		}
	}
	return (i);
}

void	ft_itoa_buf(int n, char *buf)
{
	long	num;
	int		i;

	num = n;
	i = 0;
	if (num < 0)
	{
		num = -num;
		buf[i++] = '-';
		i += ft_putnbr_buf(num, buf + 1);
		ft_reverse_str(buf + 1);
		return ;
	}
	i = ft_putnbr_buf(num, buf);
	buf[i] = '\0';
	ft_reverse_str(buf);
}
