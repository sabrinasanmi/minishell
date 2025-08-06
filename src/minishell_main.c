/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makamins <makamins@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:49:25 by sabsanto          #+#    #+#             */
/*   Updated: 2025/07/30 17:37:00 by makamins         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "garbage_collector.h"

int	main(void)
{
	char	*input;

	while (1)
	{
		input = readline("minishell> ");
		if (!input)
		{
			printf("\nexit\n");
			break ;
		}
		if (*input)
		{
			add_history(input); // adiciona ao histórico se não for vazio
			printf("Comando digitado: %s\n", input);
			printf("Tokens encontrados:\n");
			tokenize(input); // parser
		}
		free(input);
	}
	return (0);
}
