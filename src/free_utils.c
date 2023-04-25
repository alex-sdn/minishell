/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asadanow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 18:18:45 by asadanow          #+#    #+#             */
/*   Updated: 2023/04/25 18:18:46 by asadanow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_cmd_lst(t_cmd_lst **cmd_lst)
{
	t_cmd_lst	*head;
	t_cmd_lst	*next;
	int			i;

	if (!(*cmd_lst)->head)
		head = *cmd_lst;
	else
		head = (t_cmd_lst *)((*cmd_lst)->head);
	while (head)
	{
		i = 0;
		next = head->next;
		head->head = NULL;
		while (head->cmds[i])
			free(head->cmds[i++]);
		free(head->cmds);
		i = 0;
		while (head->files[i])
			free(head->files[i++]);
		free(head->files);
		free(head->file_type);
		free(head);
		head = next;
	}
	*cmd_lst = NULL;
}

void	ft_del(void *content)
{
	if (content)
		free(content);
}

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	if (!tab)
		return ;
	while (tab[i])
		free(tab[i++]);
	free(tab[i]);
	free(tab);
}

void	free_tab_mid(char **tab, int i)
{
	while (tab[i])
		free(tab[i++]);
	free_tab(tab);
}
