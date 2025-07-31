/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makamins <makamins@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 13:04:34 by makamins          #+#    #+#             */
/*   Updated: 2025/07/30 17:56:24 by makamins         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "garbage_collector.h"

int	ft_pwd(t_minishell *mini)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (!path)
	{
		perror("pwd");
		return (1);
	}
	gc_add_ptr(path, &mini->gc);
	printf("%s\n", path);
	return (0);
}
