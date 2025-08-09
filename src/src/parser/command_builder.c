/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_builder.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makamins <makamins@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 16:55:13 by sabsanto          #+#    #+#             */
/*   Updated: 2025/08/08 20:32:42 by makamins         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Cria um novo nó de comando
t_commands	*create_command_node(t_garbage **gc)
{
	t_commands	*cmd;

	cmd = gc_malloc(sizeof(t_commands), gc);
	if (!cmd)
		return (NULL);
	cmd->argv = NULL;
	cmd->redir = NULL;
	cmd->next = NULL;
	return (cmd);
}

// Cria um novo nó de redirecionamento
t_redir	*create_redir_node(t_redir_type type, char *file, t_garbage **gc)
{
	t_redir	*redir;

	redir = gc_malloc(sizeof(t_redir), gc);
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->file = file;
	redir->next = NULL;
	return (redir);
}

// Adiciona um redirecionamento ao comando
void	add_redir_to_command(t_commands *cmd, t_redir *new_redir)
{
	t_redir	*last;

	if (!cmd || !new_redir)
		return ;
	if (!cmd->redir)
	{
		cmd->redir = new_redir;
		return ;
	}
	last = cmd->redir;
	while (last->next)
		last = last->next;
	last->next = new_redir;
}

// Adiciona um comando ao final da lista de comandos
void	add_command_to_list(t_commands **list, t_commands *new_cmd)
{
	t_commands	*last;

	if (!list || !new_cmd)
		return ;
	if (!*list)
	{
		*list = new_cmd;
		return ;
	}
	last = *list;
	while (last->next)
		last = last->next;
	last->next = new_cmd;
}
