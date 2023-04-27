/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asadanow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 18:14:12 by asadanow          #+#    #+#             */
/*   Updated: 2023/04/25 18:14:14 by asadanow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	reset_fds(int *std_in_out)
{
	close(0);
	close(1);
	dup2(std_in_out[0], 0);
	dup2(std_in_out[1], 1);
	close(std_in_out[0]);
	close(std_in_out[1]);
	printf("exit\n");
}

static int	check_maxmin(long long status, char *nbr)
{
	int	len;

	len = ft_strlen(nbr) - 1;
	while (len >= 0 && nbr[len] >= '0' && nbr[len] <= '9')
	{
		if (status % 10 != nbr[len] - 48 && status % 10 * -1 != nbr[len] - 48)
			return (1);
		status = status / 10;
		len--;
	}
	return (0);
}

static long long	ft_atoll(char *nbr)
{
	int			i;
	int			sign;
	long long	ans;

	i = 0;
	sign = 1;
	ans = 0;
	while (nbr[i] == ' ' || (nbr[i] >= 9 && nbr[i] <= 13))
		i++;
	if (nbr[i] == '+')
		i++;
	else if (nbr[i] == '-')
	{
		i++;
		sign = -1;
	}
	while (nbr[i] >= '0' && nbr[i] <= '9')
	{
		ans = ans * 10 + nbr[i] - 48;
		i++;
	}
	return (ans * sign);
}

static int	get_exit_status(char **cmds)
{
	int			i;
	long long	status;

	i = 0;
	while (cmds[1][i] == ' ' || (cmds[1][i] >= 9 && cmds[1][i] <= 13))
		i++;
	if (cmds[1][i] == '-')
		i++;
	while (cmds[1][i])
		if (ft_isdigit(cmds[1][i++]) == 0)
			return (printf_error(ERR_EXIT_1, cmds[1]), 2);
	if (cmds[2])
		return (printf_error(ERR_EXIT_2, NULL), -1);
	status = ft_atoll(cmds[1]);
	if (check_maxmin(status, cmds[1]) == 1)
		return (printf_error(ERR_EXIT_1, cmds[1]), 2);
	if (status > 256)
		return ((int)(status % 256));
	if (status < 256)
		return ((int)(status % 256) + 256);
	return ((int)status);
}

int	ft_exit(t_list **env, t_cmd_lst **cmd_lst, int status, int *std_in_out)
{
	if (cmd_lst && ft_strncmp("exit", (*cmd_lst)->cmds[0], 4) == 0
		&& ft_strlen((*cmd_lst)->cmds[0]) == 4 && (*cmd_lst)->cmds[1])
		status = get_exit_status((*cmd_lst)->cmds);
	if (status >= 0)
	{
		if (std_in_out)
			reset_fds(std_in_out);
		ft_lstclear(env, &ft_del);
		rl_clear_history();
		if (cmd_lst)
			free_cmd_lst(cmd_lst);
		exit(status);
	}
	return (1);
}
