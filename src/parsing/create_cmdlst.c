/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_cmdlst.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asadanow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 18:19:46 by asadanow          #+#    #+#             */
/*   Updated: 2023/04/25 18:19:47 by asadanow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_doc_name(int idx)
{
	char	*doc;
	char	*str_idx;

	str_idx = ft_itoa(idx);
	if (!str_idx)
		return (NULL);
	doc = ft_strjoin(".infile_tmp", str_idx);
	free(str_idx);
	return (doc);
}

int	fill_redirec(t_list *lst1, t_cmd_lst **cmd_lst, int f_cnt, int idx)
{
	int		fd;
	char	*doc;

	if (is_redirec(lst1->content) == 4)
	{
		doc = get_doc_name(idx);
		if (!doc)
			return (1);
		fd = open(doc, O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (fd < 0)
			return (free(doc), 1);
		(*cmd_lst)->file_type[f_cnt] = 4;
		if (fill_heredoc(fd, lst1->next->content) == 1)
			return (free(doc), 1);
		(*cmd_lst)->files[f_cnt] = ft_strdup(doc);
		free(doc);
	}
	else if (is_redirec(lst1->content) > 0)
	{
		(*cmd_lst)->file_type[f_cnt] = is_redirec(lst1->content);
		(*cmd_lst)->files[f_cnt] = strdup_quotes((char *)lst1->next->content);
	}
	if (!(*cmd_lst)->files[f_cnt])
		return (1);
	return (0);
}

int	fill_cmd_lst(t_list *lst1, t_cmd_lst **cmds, int idx)
{
	int	cmds_count;
	int	files_cnt;

	cmds_count = 0;
	files_cnt = 0;
	while (lst1)
	{
		if (is_redirec(lst1->content) > 0)
		{
			if (fill_redirec(lst1, cmds, files_cnt++, idx) == 1)
				return (free_cmd_lst_solo(cmds, cmds_count, files_cnt - 1), 1);
			lst1 = lst1->next;
		}
		else
		{
			(*cmds)->cmds[cmds_count] = strdup_quotes((char *)lst1->content);
			if (!(*cmds)->cmds[cmds_count++])
				return (free_cmd_lst_solo(cmds, cmds_count - 1, files_cnt), 1);
		}
		lst1 = lst1->next;
	}
	return (0);
}

int	malloc_cmd_lst(t_cmd_lst **cmd_lst, t_cmd_lst **head, int len1, int len2)
{
	(*cmd_lst)->cmds = malloc(sizeof(char *) * (len1 + 1));
	if (!(*cmd_lst)->cmds)
		return (free(*cmd_lst), 1);
	(*cmd_lst)->cmds[len1] = NULL;
	(*cmd_lst)->files = malloc(sizeof(char *) * (len2 + 1));
	if (!(*cmd_lst)->files)
		return (free((*cmd_lst)->cmds), free(*cmd_lst), 1);
	(*cmd_lst)->files[len2] = NULL;
	(*cmd_lst)->file_type = malloc(sizeof(int) * len2);
	if (!(*cmd_lst)->file_type)
	{
		free((*cmd_lst)->files);
		free((*cmd_lst)->cmds);
		free(*cmd_lst);
		return (1);
	}
	(*cmd_lst)->file_amt = len2;
	(*cmd_lst)->head = (void *)*head;
	(*cmd_lst)->next = NULL;
	return (0);
}

t_cmd_lst	*create_cmd_lst(t_list *lst_split, t_cmd_lst **first, int idx)
{
	t_list			*head;
	t_cmd_lst		*cmd_lst;
	int				len_tab;
	int				len_files;

	len_tab = 0;
	len_files = 0;
	head = lst_split;
	cmd_lst = malloc(sizeof(t_cmd_lst));
	if (!cmd_lst)
		return (NULL);
	while (lst_split)
	{
		if (is_redirec(lst_split->content) > 0)
		{
			lst_split = lst_split->next;
			len_files += 1;
		}
		else
			len_tab++;
		lst_split = lst_split->next;
	}
	if (malloc_cmd_lst(&cmd_lst, first, len_tab, len_files) == 0)
		fill_cmd_lst(head, &cmd_lst, idx);
	return (cmd_lst);
}
