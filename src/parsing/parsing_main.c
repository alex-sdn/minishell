/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asadanow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 18:19:43 by asadanow          #+#    #+#             */
/*   Updated: 2023/04/25 18:19:43 by asadanow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*free_parsing(char **tab, t_list **list, t_cmd_lst **cmd_lst)
{
	if (tab)
		free_tab(tab);
	if (list)
		ft_lstclear(list, &ft_del);
	if (cmd_lst && *cmd_lst)
		free_cmd_lst(cmd_lst);
	return (NULL);
}

int	has_redirections(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '>' || str[i] == '<')
		{
			if ((i == 0 && str[i + 1] == '\0')
				|| (i == 0 && str[i + 1] == str[i] && str[i + 2] == '\0'))
				return (0);
			return (1);
		}
		else if (str[i] == '\'' || str[i] == '"')
			i = ft_skip_quotes(str, str[i], i + 1);
		i++;
	}
	return (0);
}

static int	parsing_2(t_cmd_lst **cmd_lst, t_list **lst_split, int idx)
{
	t_list			*tmp;
	t_cmd_lst		*new;

	tmp = *lst_split;
	while (tmp)
	{
		if (has_redirections(tmp->content) == 1)
			if (lst_split_redirec(&tmp, 0) == 1)
				return (1);
		tmp = tmp->next;
	}
	if (check_redirec(*lst_split) == 1)
		return (1);
	new = create_cmd_lst(*lst_split, cmd_lst, idx);
	if (!new)
		return (1);
	cmdlst_add_back(cmd_lst, new);
	return (0);
}

t_cmd_lst	*init_parsing(char *input, t_list **env, int status)
{
	t_cmd_lst		*cmd_lst;
	t_list			*lst_split;
	char			**split_1;
	int				i;

	i = -1;
	cmd_lst = NULL;
	split_1 = ft_split_quotes(input, '|');
	if (!split_1)
		return (NULL);
	while (split_1[++i])
	{
		split_1[i] = replace_env_vars(split_1[i], *env, status);
		if (!split_1[i])
			return (free_tab_mid(split_1, i), free_cmd_lst(&cmd_lst), NULL);
		lst_split = lst_split_quotes(split_1[i], ' ');
		if (!lst_split)
			return (free_parsing(split_1, NULL, &cmd_lst));
		if (parsing_2(&cmd_lst, &lst_split, i) == 1)
			return (free_parsing(split_1, &lst_split, &cmd_lst));
		ft_lstclear(&lst_split, &ft_del);
	}
	free_tab(split_1);
	return (cmd_lst);
}
