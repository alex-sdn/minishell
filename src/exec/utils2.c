/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asadanow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 18:17:07 by asadanow          #+#    #+#             */
/*   Updated: 2023/04/25 18:17:10 by asadanow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	restore_std_in_out(int fd0, int fd1)
{
	close(0);
	close(1);
	dup2(fd0, 0);
	dup2(fd1, 1);
	close(fd0);
	close(fd1);
}

void	free_pipes(int **pipes, int size)
{
	while (size-- > 0)
		free(pipes[size]);
	free(pipes);
}

int	is_builtin(char *cmd)
{
	if (ft_strncmp(cmd, "cd", 2) == 0 && ft_strlen(cmd) == 2)
		return (FT_CD);
	if (ft_strncmp(cmd, "echo", 4) == 0 && ft_strlen(cmd) == 4)
		return (FT_ECHO);
	if (ft_strncmp(cmd, "env", 3) == 0 && ft_strlen(cmd) == 3)
		return (FT_ENV);
	if (ft_strncmp(cmd, "exit", 4) == 0 && ft_strlen(cmd) == 4)
		return (FT_EXIT);
	if (ft_strncmp(cmd, "export", 6) == 0 && ft_strlen(cmd) == 6)
		return (FT_EXPORT);
	if (ft_strncmp(cmd, "pwd", 3) == 0 && ft_strlen(cmd) == 3)
		return (FT_PWD);
	if (ft_strncmp(cmd, "unset", 5) == 0 && ft_strlen(cmd) == 5)
		return (FT_UNSET);
	return (0);
}

int	is_dotdot(char *cmd)
{
	if ((ft_strlen(cmd) == 1 && strncmp(cmd, ".", 1) == 0)
		|| (ft_strlen(cmd) == 2 && strncmp(cmd, "..", 2) == 0))
		return (1);
	return (0);
}

int	is_solo_cat(t_cmd_lst *cmd_lst)
{
	int	i;

	i = 0;
	if (ft_strncmp(cmd_lst->cmds[0], "cat", 3) != 0)
		return (0);
	while (i < cmd_lst->file_amt)
	{
		if (cmd_lst->file_type[i] == 1)
			return (0);
		i++;
	}
	return (1);
}
