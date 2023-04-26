/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asadanow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 18:22:29 by asadanow          #+#    #+#             */
/*   Updated: 2023/04/25 18:22:31 by asadanow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_special(char c)
{
	if (c < 48 || (c > 57 && c < 65) || (c > 90 && c < 95)
		|| c == 96 || c > 122)
		return (1);
	return (0);
}

int	is_spaces(char *str)
{
	int	i;

	i = 0;
	while (str[i] && (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13)))
		i++;
	if (!str[i])
		return (1);
	return (0);
}

char	*triple_strjoin(char *str1, char *str2, char *str3)
{
	char	*new1;
	char	*new2;

	if (str1)
	{
		new1 = ft_strjoin(str1, str2);
		if (!new1)
			return (NULL);
	}
	else
		new1 = str2;
	if (str3)
	{
		new2 = ft_strjoin(new1, str3);
		free(new1);
		return (new2);
	}
	else
		return (new1);
}

void	printf_error(char *str, char *arg)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '%')
		{
			ft_putstr_fd(arg, 2);
			i++;
		}
		else
			ft_putchar_fd(str[i], 2);
		i++;
	}
}
