#include "minishell.h"

// pipe[0] == read side
// pipe[1] == write side

//faire toute la boucle de fork/pipe/execve, oublie pas les builtins
void	exec_cmds(t_parsed_lst *cmd_lst, t_list **env)
{
	int		pipe1[2];
	int		pipe2[2];
	pid_t	process_id;
	int		i;

	i = 1;
	// first
	if (pipe(pipe1) < 0)
		return (perror("pipe"));  //faire error mgmt correct  // just un exit avec bon code?
	process_id = fork();
	if (process_id < 0)
		return (perror("fork"));  //same
	if (process_id == 0)
		first_process(0, pipe1, cmd_lst, env);
	wait(NULL);
	cmd_lst = cmd_lst->next;
	// mid
	while (cmd_lst->next)
	{
		if (i % 2 != 0 && pipe(pipe2) < 0)
			return (perror("pipe"));
		else if (i % 2 == 0 && pipe(pipe1) < 0)
			return (perror("pipe"));
		process_id = fork();
		if (process_id < 0)
			return (perror("fork"));
		if (process_id == 0)
		{
			if (i % 2 != 0)
				mid_process(pipe1, pipe2, cmd_lst, env);  // probleme trop d'arg, doit metter fd_in_out dans cmd_lst
			else
				mid_process(pipe2, pipe1, cmd_lst, env);
		}
		wait(NULL);
		i++;
		cmd_lst = cmd_lst->next;
	}
	// last
	if (i % 2 != 0)
		last_process(pipe1, 1, cmd_lst, env);
	else
		last_process(pipe2, 1, cmd_lst, env);
}

void	first_process(int fd_in, int pipewr[], t_parsed_lst *cmd_lst, t_list **env)
{
	char	**paths;
	char	*pathcmd;
	int		i = 0;

	// if (piperd)
	// 	close(piperd[1]);
	close(pipewr[0]);
	if (cmd_lst->in_fd < 0 || cmd_lst->out_fd < 0)
		exit(EXIT_FAILURE);
	
	fd_in = cmd_lst->in_fd;
	if (cmd_lst->out_fd > 0)
		pipewr[1] = cmd_lst->out_fd;
	if (dup2(fd_in, 0) < 0)
		exit(0);  // a revoir
	if (dup2(pipewr[1], 1) < 0)
		exit(0);
	if (is_builtin(cmd_lst->cmds[0]) > 0)
		exec_builtin(cmd_lst, env);  //besoin de passer les pipes pour les fermer si jamais exit ? jsp
	// V mettre dans fct exec_binary
	paths = get_paths(env);
	if (!paths)
		exit(0); // malloc error
	while (paths[i])
	{
		pathcmd = ft_strjoin(paths[i], cmd_lst->cmds[0]);
		if (!pathcmd)
			exit(0); // FREE PATHS + malloc err
		execve(pathcmd, cmd_lst->cmds, NULL);    // peut pas envoyer mon env....
		i++;
		free(pathcmd);
	}
	exit(0); // WRONG CMD ERRNO + free(paths)
}

void	mid_process(int piperd[], int pipewr[], t_parsed_lst *cmd_lst, t_list **env)
{
	char	**paths;
	char	*pathcmd;
	int		i = 0;

	close(piperd[1]);
	close(pipewr[0]);
	if (cmd_lst->in_fd < 0 || cmd_lst->out_fd < 0)
		exit(EXIT_FAILURE);

	if (cmd_lst->in_fd > 0)
		piperd[0] = cmd_lst->in_fd;
	if (cmd_lst->out_fd > 0)
		pipewr[1] = cmd_lst->out_fd;
	if (dup2(piperd[0], 0) < 0)
		exit(0);  // a revoir
	if (dup2(pipewr[1], 1) < 0)
		exit(0);
	if (is_builtin(cmd_lst->cmds[0]) > 0)
		exec_builtin(cmd_lst, env);  //besoin de passer les pipes pour les fermer si jamais exit ? jsp
	// V mettre dans fct exec_binary
	paths = get_paths(env);
	if (!paths)
		exit(0); // malloc error
	while (paths[i])
	{
		pathcmd = ft_strjoin(paths[i], cmd_lst->cmds[0]);
		if (!pathcmd)
			exit(0); // FREE PATHS + malloc err
		execve(pathcmd, cmd_lst->cmds, NULL);    // peut pas envoyer mon env....
		i++;
		free(pathcmd);
	}
	exit(0); // WRONG CMD ERRNO + free(paths)
}

void	last_process(int piperd[], int fd_out, t_parsed_lst *cmd_lst, t_list **env)
{
	char	**paths;
	char	*pathcmd;
	int		i = 0;

	close(piperd[1]);
	if (cmd_lst->in_fd < 0 || cmd_lst->out_fd < 0)
		exit(EXIT_FAILURE);

	if (cmd_lst->in_fd > 0)
		piperd[0] = cmd_lst->in_fd;
	fd_out = cmd_lst->out_fd;
	if (dup2(piperd[0], 0) < 0)
		exit(0);  // a revoir
	if (dup2(fd_out, 1) < 0)
		exit(0);
	if (is_builtin(cmd_lst->cmds[0]) > 0)
		exec_builtin(cmd_lst, env);  //besoin de passer les pipes pour les fermer si jamais exit ? jsp
	// V mettre dans fct exec_binary
	paths = get_paths(env);
	if (!paths)
		exit(0); // malloc error
	while (paths[i])
	{
		pathcmd = ft_strjoin(paths[i], cmd_lst->cmds[0]);
		if (!pathcmd)
			exit(0); // FREE PATHS + malloc err
		execve(pathcmd, cmd_lst->cmds, NULL);    // peut pas envoyer mon env....
		i++;
		free(pathcmd);
	}
	exit(0); // WRONG CMD ERRNO + free(paths)
}
