#include "minishell.h"

void	loop_process(int fd_in, int fd_out, t_cmd_lst **cmd_lst, t_list **env)
{
	char	*pathcmd;

	if (open_files(&fd_in, &fd_out, *cmd_lst) == 1)
		exit(1);
	if (dup2(fd_in, 0) < 0 || dup2(fd_out, 1) < 0)
	{
		perror("dup2");
		exit(errno);
	}
	if (is_builtin((*cmd_lst)->cmds[0]) > 0)
		exit(exec_builtin(cmd_lst, env, 0));
	pathcmd = check_access((*cmd_lst)->cmds[0], env, -1);
	if (pathcmd == NULL)
		ft_exit(env, cmd_lst, convert_status(errno), 1);
	execve(pathcmd, (*cmd_lst)->cmds, create_env_tab(*env));
	exit(0);
}

int	solo_process(t_cmd_lst **cmd_lst, t_list **env)
{
	char	*pathcmd;
	pid_t	proc_id;
	int		status;
	int		fd_in_out[2];

	fd_in_out[0] = 0;
	fd_in_out[1] = 1;
	if (open_files(&fd_in_out[0], &fd_in_out[1], *cmd_lst) == 1)
		return (1);
	if (dup2(fd_in_out[0], 0) < 0 || dup2(fd_in_out[1], 1) < 0)
		return (perror("dup2"), errno);
	if (is_builtin((*cmd_lst)->cmds[0]) > 0)
		return (exec_builtin(cmd_lst, env, 1));
	pathcmd = check_access((*cmd_lst)->cmds[0], env, -1);
	if (pathcmd == NULL)
		return (convert_status(errno));
	proc_id = fork();
	if (proc_id < 0)
		return (perror("fork"), errno);
	if (proc_id == 0)
		execve(pathcmd, (*cmd_lst)->cmds, create_env_tab(*env));
	while (waitpid(proc_id, &status, WNOHANG) == 0);
	if (sig_global == 0)
		return (free(pathcmd), 130);
	return (free(pathcmd), convert_status(status));
}

int	exec_builtin(t_cmd_lst **cmd_lst, t_list **env, int solo)
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
	else if (builtin_id == FT_EXIT)
		status = ft_exit(env, cmd_lst, 0, 0);
	else if (builtin_id == FT_EXPORT)
		status = ft_export(env, *cmd_lst);
	else if (builtin_id == FT_PWD)
		status = ft_pwd();
	else if (builtin_id == FT_UNSET)
		status = ft_unset(env, *cmd_lst);
	if (solo == 0)
		ft_exit(env, cmd_lst, status, 1);
	return (status);
}
