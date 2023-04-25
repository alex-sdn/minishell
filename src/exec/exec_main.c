/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asadanow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 18:16:41 by asadanow          #+#    #+#             */
/*   Updated: 2023/04/25 18:16:42 by asadanow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_files(int *in_fd, int *out, t_cmd_lst *cmds)
{
	int	i;

	i = 0;
	while (i < cmds->file_amt)
	{
		if (cmds->file_type[i] == 1)
		{
			close(*in_fd);
			*in_fd = open(cmds->files[i], O_RDONLY);
			if (*in_fd < 0)
				return (perror(cmds->files[i]), 1);
		}
		else
		{
			close(*out);
			if (cmds->file_type[i] == 2)
				*out = open(cmds->files[i], O_CREAT | O_RDWR | O_TRUNC, 0644);
			else if (cmds->file_type[i] == 3)
				*out = open(cmds->files[i], O_CREAT | O_RDWR | O_APPEND, 0644);
			if (*out < 0)
				return (perror(cmds->files[i]), 1);
		}
		i++;
	}
	return (0);
}

void	err_access(t_cmd_lst *cmd_lst, int pos, int err)
{
	while (pos-- > 0)
		cmd_lst = cmd_lst->next;
	if (!(cmd_lst->cmds[0][0]))
		printf_error(ERR_CMD, "\'\'");
	else if (err == 126 && cmd_lst->cmds[0])
		printf_error(ERR_PERM, cmd_lst->cmds[0]);
	else
		printf_error(ERR_CMD, cmd_lst->cmds[0]);
}

char	*check_access(char *cmd, t_list **env, int i)
{
	char	**paths;
	char	*pathcmd;

	if (!cmd || !cmd[0])
	{
		errno = 127;
		if (!cmd)
			errno = 0;
		return (NULL);
	}
	paths = get_paths(env);
	if (!paths)
		return (NULL);
	while (paths[++i])
	{
		pathcmd = ft_strjoin(paths[i], cmd);
		if (!pathcmd)
			return (NULL);
		if (access(pathcmd, X_OK) == 0)
			break ;
		free(pathcmd);
		pathcmd = NULL;
	}
	return (free_tab(paths), pathcmd);
}

int	exec_pipes(int **pipes, t_cmd_lst **cmd_lst, t_list **env)
{
	int		i;
	int		cmd_count;
	pid_t	*proc_ids;

	i = 0;
	cmd_count = cmdlst_size(*cmd_lst);
	proc_ids = malloc(sizeof(pid_t) * cmd_count);
	if (!proc_ids)
		return (1);
	while (i < cmd_count)
	{
		proc_ids[i] = fork();
		if (proc_ids[i] < 0)
			return (perror("fork"), errno);
		if (proc_ids[i] == 0)
		{
			free(proc_ids);
			start_process(i, pipes, cmd_lst, env);
		}
		if ((*cmd_lst)->next)
			*cmd_lst = (*cmd_lst)->next;
		i++;
	}
	i = wait_procs(pipes, proc_ids, cmd_count, (*cmd_lst)->head);
	return (free(proc_ids), i);
}

int	exec_main(t_cmd_lst **cmd_lst, t_list **env, int cmd_count)
{
	int		**pipes;
	int		std_in_out[2];
	int		status;

	if (cmd_count == 1)
	{
		std_in_out[0] = dup(0);
		std_in_out[1] = dup(1);
		replace_env(env, "_", get_last_str((*cmd_lst)->cmds));
		status = solo_process(cmd_lst, env);
		if (status == 127 || status == 126)
			err_access(*cmd_lst, 0, status);
		restore_std_in_out(std_in_out[0], std_in_out[1]);
	}
	else
	{
		pipes = init_pipes(cmd_count - 1);
		if (!pipes)
			return (1);
		status = exec_pipes(pipes, cmd_lst, env);
		free_pipes(pipes, cmd_count - 1);
	}
	return (status);
}
