/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdlst_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asadanow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 18:19:27 by asadanow          #+#    #+#             */
/*   Updated: 2023/04/25 18:19:28 by asadanow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cmdlst_add_back(t_cmd_lst **cmd_lst, t_cmd_lst *new)
{
	t_cmd_lst	*last;

	if (!new)
		return ;
	if (!(*cmd_lst))
		*cmd_lst = new;
	else
	{
		last = *cmd_lst;
		while (last->next)
			last = last->next;
		last->next = new;
	}
}

int	cmdlst_size(t_cmd_lst *cmd_lst)
{
	int	count;

	count = 0;
	if (cmd_lst)
		count++;
	while (cmd_lst->next)
	{
		count++;
		cmd_lst = cmd_lst->next;
	}
	return (count);
}

void	free_cmd_lst_solo(t_cmd_lst **cmd_lst, int cmds_count, int files_count)
{
	int	i;

	i = 0;
	if (cmds_count > 0)
		while (i < cmds_count - 1)
			free((*cmd_lst)->cmds[i++]);
	else
		while ((*cmd_lst)->cmds[i])
			free((*cmd_lst)->cmds[i++]);
	free((*cmd_lst)->cmds);
	i = 0;
	if (files_count > 0)
		while (i < files_count - 1)
			free((*cmd_lst)->files[i++]);
	else
		while ((*cmd_lst)->files[i])
			free((*cmd_lst)->files[i++]);
	free((*cmd_lst)->files);
	free((*cmd_lst)->file_type);
	free(*cmd_lst);
	*cmd_lst = NULL;
}

char	*malloc_no_quotes(char *str)
{
	char	*dest;
	int		len;
	int		quotes;

	len = 0;
	quotes = 0;
	while (str[len])
	{
		if (str[len] == '"' || str[len] == '\'')
		{
			len = ft_skip_quotes(str, str[len], len + 1);
			quotes += 2;
		}
		len++;
	}
	dest = malloc(sizeof(char) * (len - quotes + 1));
	return (dest);
}

char	*strdup_quotes(char *str)
{
	int		i;
	int		len;
	char	c;
	char	*dest;

	i = 0;
	len = 0;
	dest = malloc_no_quotes(str);
	if (!dest)
		return (NULL);
	while (str[len])
	{
		if (str[len] == '"' || str[len] == '\'')
		{
			c = str[len];
			len++;
			while (str[len] != c)
				dest[i++] = str[len++];
			len++;
		}
		else
			dest[i++] = str[len++];
	}
	dest[i] = '\0';
	return (dest);
}
