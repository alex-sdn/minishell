#include "minishell.h"

int	**init_pipes(int size)
{
	int **pipes;
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

void	free_pipes(int **pipes, int size)
{
	while (size-- > 0)
		free(pipes[size]);
	free(pipes);
}

void	start_process(int i, int **pipes, t_cmd_lst **cmd_lst, t_list **env)
{
	int j;
	int fd[2];

	j = -1;
	if (i == 0)
		fd[0] = 0;
	else
		fd[0] = pipes[i - 1][0];
	if (!pipes[i])
		fd[1] = 1;
	else
		fd[1] = pipes[i][1];
	while (pipes[++j])
	{
		if (j != i - 1)
			close(pipes[j][0]);
		if (j != i)
			close(pipes[j][1]);
	}
	free_pipes(pipes, j);
	loop_process(fd[0], fd[1], cmd_lst, env);
}

static void	close_pipes(int **pipes, int pos, int cmd_count)
{
	if (pos != 0)
		close(pipes[pos - 1][0]);
	if (pos != cmd_count - 1)
		close(pipes[pos][1]);
}

int	wait_procs(int **pipes, pid_t *proc_ids, int cmd_count, t_cmd_lst *cmd_lst)
{
	int		i;
	int		pos;
	int		status[2];
	pid_t	finished;

	i = cmd_count;
	while (i > 0)
	{
		pos = 0;
		finished = waitpid(-1, &status[0], 0);
		if (finished > 0)
		{
			while (finished != proc_ids[pos])
				pos++;
			close_pipes(pipes, pos, cmd_count);
			if (pos == cmd_count - 1)
				status[1] = status[0] / 256;
			if (status[0] / 256 == 127 || status[0] / 256 == 126)
				err_access(cmd_lst, pos, status[0] / 256);
			i--;
		}
	}
	if (sig_global == 0)
		status[1] = 130;
	return (status[1]);
}
