/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_wait.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asadanow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 18:16:45 by asadanow          #+#    #+#             */
/*   Updated: 2023/04/25 18:16:46 by asadanow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	**init_pipes(int size)
{
	int	**pipes;
	int	i;

	i = 0;
	pipes = malloc(sizeof(int *) * (size + 1));
	if (!pipes)
		return (NULL);
	while (i < size)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i])
			return (free_pipes(pipes, i), NULL);
		if (pipe(pipes[i]) < 0)
			return (perror("pipe"), free_pipes(pipes, size), NULL);
		i++;
	}
	pipes[size] = 0;
	return (pipes);
}

void	start_process(int i, int **pipes, t_cmd_lst **cmd_lst, t_list **env)
{
	int	j;
	int	fds[2];

	j = -1;
	if (i == 0)
		fds[0] = 0;
	else
		fds[0] = pipes[i - 1][0];
	if (!pipes[i])
		fds[1] = 1;
	else
		fds[1] = pipes[i][1];
	while (pipes[++j])
	{
		if (j != i - 1)
			close(pipes[j][0]);
		if (j != i)
			close(pipes[j][1]);
	}
	free_pipes(pipes, j);
	loop_process(fds, cmd_lst, env);
}

static void	close_pipes(int **pipes, int pos, int cmd_count)
{
	if (pos != 0)
		close(pipes[pos - 1][0]);
	if (pos != cmd_count - 1)
		close(pipes[pos][1]);
}

static int	wait_status(int status)
{
	if (g_sig == 0 && status == -1)
	{
		g_sig = 1;
		return (130);
	}
	g_sig = 1;
	return (status);
}

int	wait_procs(int **pipes, pid_t *proc_ids, int cmd_count, t_cmd_lst *cmd_lst)
{
	int		i;
	int		pos;
	int		status[2];
	pid_t	finished;

	i = cmd_count;
	status[1] = -1;
	while (i > 0)
	{
		pos = 0;
		finished = waitpid(-1, &status[0], 0);
		if (finished > 0)
		{
			while (finished != proc_ids[pos])
				pos++;
			close_pipes(pipes, pos, cmd_count);
			if (pos == cmd_count - 1 && g_sig == 1)
				status[1] = status[0] / 256;
			if (status[0] / 256 == 127 || status[0] / 256 == 126)
				err_access(cmd_lst, pos, status[0] / 256);
			i--;
		}
	}
	return (wait_status(status[1]));
}
