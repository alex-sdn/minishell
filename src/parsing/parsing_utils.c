/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asadanow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 18:20:03 by asadanow          #+#    #+#             */
/*   Updated: 2023/04/25 18:20:04 by asadanow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_pipes(char *input, int i)
{
	while (input[i] == ' ')
		i++;
	if (input[i] == '|')
		return (printf_error(ERR_SNTX, "|"), 1);
	while (input[i])
	{
		while (input[i] && input[i] != '|')
			i++;
		if (input[i] == '|')
		{
			i++;
			while (input[i] == ' ' || input[i] == 9)
				i++;
			if (input[i] == '|')
				return (printf_error(ERR_SNTX, "|"), 1);
		}
	}
	i--;
	while ((input[i] == ' ' || input[i] == 9) && i > 0)
		i--;
	if (input[i] == '|')
		return (printf_error(ERR_SNTX, "|"), 1);
	return (0);
}

int	check_quotes(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '\'')
		{
			i = ft_skip_quotes(input, input[i], i + 1);
			if (i < 0)
				return (printf_error(ERR_SNTX, "'"), 1);
		}
		else if (input[i] == '"')
		{
			i = ft_skip_quotes(input, input[i], i + 1);
			if (i < 0)
				return (printf_error(ERR_SNTX, "\""), 1);
		}
		i++;
	}
	return (0);
}

int	check_redirec(t_list *lst)
{
	while (lst && lst->content)
	{
		if (is_redirec(lst->content) > 0)
		{
			if (!lst->next || is_spaces(lst->next->content) == 1)
				return (printf_error(ERR_SNTX, "newline"), 1);
			else if (is_redirec(lst->next->content) > 0)
			{
				if (is_redirec(lst->next->content) == 1)
					return (printf_error(ERR_SNTX, "<"), 1);
				if (is_redirec(lst->next->content) == 2)
					return (printf_error(ERR_SNTX, ">"), 1);
				if (is_redirec(lst->next->content) == 3)
					return (printf_error(ERR_SNTX, ">>"), 1);
				if (is_redirec(lst->next->content) == 4)
					return (printf_error(ERR_SNTX, "<<"), 1);
			}
		}
		lst = lst->next;
	}
	return (0);
}

int	ft_skip_quotes(char *str, char c, int i)
{
	while (str[i] && str[i] != c)
		i++;
	if (!str[i])
		return (-10);
	return (i);
}

int	is_redirec(char *str)
{
	if (str[0] == '<')
	{
		if (!str[1])
			return (1);
		else
			return (4);
	}
	if (str[0] == '>')
	{
		if (!str[1])
			return (2);
		else
			return (3);
	}
	return (0);
}
