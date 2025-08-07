/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabsanto <sabsanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:25:19 by makamins          #+#    #+#             */
/*   Updated: 2025/08/06 20:55:15 by sabsanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Converte int para string em buffer fornecido
void	ft_itoa_buf(int n, char *buf)
{
	int		i;
	int		sign;
	long	num;

	num = n;
	sign = 0;
	if (num < 0)
	{
		sign = 1;
		num = -num;
	}
	
	i = 0;
	if (num == 0)
		buf[i++] = '0';
	else
	{
		char temp[12];
		int j = 0;
		
		while (num > 0)
		{
			temp[j++] = (num % 10) + '0';
			num /= 10;
		}
		
		if (sign)
			buf[i++] = '-';
		
		while (j > 0)
			buf[i++] = temp[--j];
	}
	buf[i] = '\0';
}

// Escreve número no fd
void	ft_putnbr_fd(int n, int fd)
{
	char	buf[12];
	
	ft_itoa_buf(n, buf);
	write(fd, buf, ft_strlen(buf));
}
