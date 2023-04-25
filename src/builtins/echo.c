/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asadanow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 18:14:05 by asadanow          #+#    #+#             */
/*   Updated: 2023/04/25 18:14:06 by asadanow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(t_cmd_lst *cmd_lst)
{
	int	newline;
	int	start;

	start = 1;
	if (cmd_lst->cmds[1] && ft_strlen(cmd_lst->cmds[1]) == 2
		&& ft_strncmp(cmd_lst->cmds[1], "-n", 2) == 0)
		start = 2;
	newline = start;
	while (cmd_lst->cmds[start])
	{
		ft_putstr_fd(cmd_lst->cmds[start++], 1);
		if (cmd_lst->cmds[start])
			ft_putchar_fd(' ', 1);
	}
	if (newline == 1)
		ft_putchar_fd('\n', 1);
	return (0);
}
