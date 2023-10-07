/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asadanow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 18:16:49 by asadanow          #+#    #+#             */
/*   Updated: 2023/04/25 18:16:49 by asadanow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	loop_process(int *fds, t_cmd_lst **cmd_lst, t_list **env)
{
	char	*pathcmd;

	if (open_files(&fds[0], &fds[1], *cmd_lst, 0) == 1)
	{
		double_close(fds[0], fds[1]);
		ft_exit(env, cmd_lst, 1, NULL);
	}
	if (dup2(fds[0], 0) < 0 || dup2(fds[1], 1) < 0)
	{
		perror("dup2");
		ft_exit(env, cmd_lst, convert_status(errno), NULL);
	}
	if (is_builtin((*cmd_lst)->cmds[0]) > 0)
		exit(exec_builtin(cmd_lst, env, 0, fds));
	pathcmd = check_access((*cmd_lst)->cmds[0], env, -1);
	if (pathcmd == NULL)
	{
		double_close(fds[0], fds[1]);
		ft_exit(env, cmd_lst, convert_status(errno), NULL);
	}
	if (is_solo_cat(*cmd_lst) == 1)
		init_signal(S_CAT);
	execve(pathcmd, (*cmd_lst)->cmds, create_env_tab(*env));
	exit(0);
}

static int	solo_proc_2(t_cmd_lst **cmd_lst, t_list **env, char *path, int *fd)
{
	pid_t	proc_id;
	int		status;

	if (is_solo_cat(*cmd_lst) == 1)
		init_signal(S_CAT);
	proc_id = fork();
	if (proc_id < 0)
		return (perror("fork"), free(path), 128);
	if (proc_id == 0)
	{
		close(fd[0]);
		close(fd[1]);
		execve(path, (*cmd_lst)->cmds, create_env_tab(*env));
	}
	while (waitpid(proc_id, &status, WNOHANG) == 0)
		;
	init_signal(S_DEFAULT);
	if (g_sig == 0)
		return (free(path), 130);
	return (free(path), convert_status(status));
}

int	solo_process(t_cmd_lst **cmd_lst, t_list **env, int *std_in_out)
{
	char	*pathcmd;
	int		fd_in_out[2];

	fd_in_out[0] = 0;
	fd_in_out[1] = 1;
	if (open_files(&fd_in_out[0], &fd_in_out[1], *cmd_lst, 0) == 1)
		return (1);
	if (dup2(fd_in_out[0], 0) < 0 || dup2(fd_in_out[1], 1) < 0)
		return (perror("dup2"), errno);
	if (is_builtin((*cmd_lst)->cmds[0]) > 0)
		return (exec_builtin(cmd_lst, env, 1, std_in_out));
	pathcmd = check_access((*cmd_lst)->cmds[0], env, -1);
	if (pathcmd == NULL)
		return (convert_status(errno));
	return (solo_proc_2(cmd_lst, env, pathcmd, std_in_out));
}

int	exec_builtin(t_cmd_lst **cmd_lst, t_list **env, int solo, int *fds)
{
	int	builtin_id;
	int	status;

	status = 0;
	builtin_id = is_builtin((*cmd_lst)->cmds[0]);
	if (builtin_id == FT_CD)
		status = ft_cd(env, *cmd_lst);
	else if (builtin_id == FT_ECHO)
		status = ft_echo(*cmd_lst);
	else if (builtin_id == FT_ENV)
		status = ft_env(*env, *cmd_lst);
	else if (builtin_id == FT_EXPORT)
		status = ft_export(env, *cmd_lst);
	else if (builtin_id == FT_PWD)
		status = ft_pwd();
	else if (builtin_id == FT_UNSET)
		status = ft_unset(env, *cmd_lst);
	if (!solo)
	{
		double_close(fds[0], fds[1]);
		ft_exit(env, cmd_lst, status, NULL);
	}
	else if (builtin_id == FT_EXIT)
		status = ft_exit(env, cmd_lst, 0, fds);
	return (status);
}
