#include "minishell.h"

char	*check_access(char *cmd, t_list **env)
{
	char	**paths;
	char	*pathcmd;
	int		i;

	i = 0;
	if (is_builtin(cmd) > 0)
		return (NULL);
	paths = get_paths(env);
	if (!paths)
		return (NULL);  //malloc error
	while (paths[i])
	{
		pathcmd = ft_strjoin(paths[i], cmd);
		if (!pathcmd)
			return (NULL);  //malloc error
		if (access(pathcmd, X_OK) == 0)
			break ;
		free(pathcmd);
		i++;
	}
	if (!paths[i])
		return (free_tab(paths), perror(cmd), NULL);
	return (free_tab(paths), pathcmd);
}

void	first_process(int fd_in, int pipewr[], t_parsed_lst *cmd_lst, t_list **env)
{
	char	*pathcmd;

	close(pipewr[0]);
	//loop open files in order:
	if (open_files(&fd_in, &pipewr[1], cmd_lst) == 1)
		exit(1); // ERR_FILE
	//check if builtin or accessible bin
	pathcmd = check_access(cmd_lst->cmds[0], env);
	if (pathcmd == NULL && is_builtin(cmd_lst->cmds[0]) == 0)
		exit(1);  // ERR_CMD
	//dup
	if (dup2(fd_in, 0) < 0)
		exit(0);  // revoir errno
	if (dup2(pipewr[1], 1) < 0)
		exit(0);
	//exec cmd
	if (is_builtin(cmd_lst->cmds[0]) > 0)
		exec_builtin(cmd_lst, env);  //besoin de passer les pipes pour les fermer si jamais exit ? jsp
	execve(pathcmd, cmd_lst->cmds, create_env_tab(*env));  //doit cree un env en char** avec la list
	exit(0); // normalement arrive pas la
}

void	mid_process(int piperd[], int pipewr[], t_parsed_lst *cmd_lst, t_list **env)
{
	char	*pathcmd;

	close(piperd[1]);
	close(pipewr[0]);
	//loop open files in order:
	if (open_files(&piperd[0], &pipewr[1], cmd_lst) == 1)
		exit(1); // ERR_FILE
	//check if builtin or accessible bin
	pathcmd = check_access(cmd_lst->cmds[0], env);
	if (pathcmd == NULL && is_builtin(cmd_lst->cmds[0]) == 0)
		exit(1);  // ERR_CMD
	//dup
	if (dup2(piperd[0], 0) < 0)
		exit(0);  // revoir errno
	if (dup2(pipewr[1], 1) < 0)
		exit(0);
	//exec cmd
	if (is_builtin(cmd_lst->cmds[0]) > 0)
		exec_builtin(cmd_lst, env);  //besoin de passer les pipes pour les fermer si jamais exit ? jsp
	execve(pathcmd, cmd_lst->cmds, create_env_tab(*env));
	exit(0); // normalement arrive pas la + pathcmd free par execve
}

void	last_process(int piperd[], int fd_out, t_parsed_lst *cmd_lst, t_list **env)
{
	char	*pathcmd;

	close(piperd[1]);
	//loop open files in order:
	if (open_files(&piperd[0], &fd_out, cmd_lst) == 1)
		exit(1); // ERR_FILE
	//check if builtin or accessible bin
	pathcmd = check_access(cmd_lst->cmds[0], env);
	if (pathcmd == NULL && is_builtin(cmd_lst->cmds[0]) == 0)
		exit(1);  // ERR_CMD
	//dup
	if (dup2(piperd[0], 0) < 0)
		exit(0);  // revoir errno
	if (dup2(fd_out, 1) < 0)
		exit(0);
	//exec cmd
	if (is_builtin(cmd_lst->cmds[0]) > 0)
		exec_builtin(cmd_lst, env);  //besoin de passer les pipes pour les fermer si jamais exit ? jsp
	execve(pathcmd, cmd_lst->cmds, create_env_tab(*env));
	exit(0); // normalement arrive pas la + pathcmd free par execve
}

void	solo_process(t_parsed_lst *cmd_lst, t_list **env)
{
	char	*pathcmd;
	int		fd_in;
	int		fd_out;

	fd_in = 0;
	fd_out = 1;
	//loop open files in order:
	if (open_files(&fd_in, &fd_out, cmd_lst) == 1)
		exit(1); // ERR_FILE
	//check if builtin or accessible bin
	pathcmd = check_access(cmd_lst->cmds[0], env);
	if (pathcmd == NULL && is_builtin(cmd_lst->cmds[0]) == 0)
		exit(1);  // ERR_CMD
	//dup
	if (dup2(fd_in, 0) < 0)
		exit(0);  // revoir errno
	if (dup2(fd_out, 1) < 0)
		exit(0);
	//exec cmd
	if (is_builtin(cmd_lst->cmds[0]) > 0)
		exec_builtin(cmd_lst, env);  //besoin de passer les pipes pour les fermer si jamais exit ? jsp
	execve(pathcmd, cmd_lst->cmds, create_env_tab(*env));
	exit(0); // normalement arrive pas la + pathcmd free par execve
}
