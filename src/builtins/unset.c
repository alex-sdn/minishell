/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asadanow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 18:14:32 by asadanow          #+#    #+#             */
/*   Updated: 2023/04/25 18:14:34 by asadanow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_unset(t_list **env, t_cmd_lst *cmd_lst)
{
	t_list	*tmp;
	t_list	*prev;
	int		i;

	i = 0;
	prev = NULL;
	while (*env && cmd_lst->cmds[++i])
	{
		tmp = *env;
		while (tmp && tmp->content && ft_strncmp(cmd_lst->cmds[i],
				(char*)tmp->content, ft_strlen(cmd_lst->cmds[i])) != 0)
		{
			prev = tmp;
			tmp = tmp->next;
		}
		if (tmp)
		{
			if (prev)
				prev->next = tmp->next;
			else
				*env = (*env)->next;
			ft_lstdelone(tmp, &ft_del);
		}
	}
	return (0);
}
