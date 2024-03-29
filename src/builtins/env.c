/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asadanow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 18:14:09 by asadanow          #+#    #+#             */
/*   Updated: 2023/04/25 18:14:50 by asadanow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(t_list *env, t_cmd_lst *cmd_lst)
{
	if (cmd_lst->cmds[1])
		return (write(2, "env: too many arguments\n", 24), 1);
	while (env && env->content)
	{
		ft_putstr_fd((char *)env->content, 1);
		ft_putchar_fd('\n', 1);
		env = env->next;
	}
	return (0);
}
