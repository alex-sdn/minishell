#include "minishell.h"

static int	lst_split_quotes_2(t_list **head, char *str, int i, int j)
{
	t_list	*new;
	char	*content;

	content = ft_substr(str, j, i - j);
	if (!content)
		return (1);
	new = ft_lstnew((void *)content);
	if (!new)
		return (free(content), 1);
	ft_lstadd_back(head, new);
	return (0);
}

t_list	*lst_split_quotes(char *str, char c)
{
	t_list  *head;
	int		i;
	int		j;

	i = 0;
	head = NULL;
	while (str[i] == c)
			i++;
	while (str[i])
	{
		j = i;
		while (str[i] != c && str[i])
		{
			if (str[i] == '"' || str[i] == '\'')
				i = ft_skip_quotes(str, str[i], i + 1);
			i++;
		}
		if (lst_split_quotes_2(&head, str, i, j) == 1)
			return (ft_lstclear(&head, &ft_del), NULL);
		while (str[i] == c)
			i++;
	}
	return (head);
}

int	get_redirec_pos(char *str)
{
	int	i;

	i = 0;
	while (!(str[i] == '<' || str[i] == '>') && str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
			i = ft_skip_quotes(str, str[i], i + 1);
		i++;
	}
	if (i == 0 && str[i])
	{
		i++;
		if (str[i] == str[i - 1])
			i++;
		if (!str[i])
			return (-1);
		return (i);
	}
	if (!str[i])
		return (-1);
	return (i);
}

static int	lst_split_redirec_2(t_list **tmp, char *old, int i)
{
	t_list *nxt;
	char	*content;

	nxt = (*tmp)->next;
	content = ft_substr(old, i, ft_strlen(old) - i);
	if (!content)
		return (1);
	(*tmp)->next = ft_lstnew((void *)content);
	(*tmp)->next->next = nxt;
	if (!(*tmp)->next)
		return (ft_lstclear(&nxt, &ft_del), 1);
	return (0);
}

int	lst_split_redirec(t_list **head, int i)
{
	t_list	*tmp;
	char	*old;

	tmp = *head;
	while (tmp->content && i == 0)
	{
		i = get_redirec_pos((char *)tmp->content);
		if (i > 0)
		{
			old = ft_strdup((char *)tmp->content);
			if (!old)
				return (1);
			free(tmp->content);
			tmp->content = ft_substr(old, 0, i);
			if (!tmp->content)
				return (free(old), 1);
			if (i < (int)ft_strlen(old))
				if (lst_split_redirec_2(&tmp, old, i) == 1)
					return (free(old), 1);
			free(old);
			tmp = tmp->next;
			i = 0;	
		}
	}
	return (0);
}
