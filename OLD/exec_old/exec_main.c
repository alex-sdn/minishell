#include "minishell.h"

//opens all files for this cmd in order
int	open_files(int *in_fd, int *out_fd, t_parsed_lst *cmd_lst)
{
	int	i;

	i = 0;
	while (i < cmd_lst->file_amt)
	{
		if (cmd_lst->file_type[i] == 1)
		{
			close(*in_fd);
			*in_fd = open(cmd_lst->files[i], O_RDONLY);
			if (*in_fd < 0)
				return (perror(cmd_lst->files[i]), 1);
		}
		else
		{
			close(*out_fd);
			if (cmd_lst->file_type[i] == 2)
				*out_fd = open(cmd_lst->files[i], O_CREAT | O_RDWR | O_TRUNC, 0644);
			else if (cmd_lst->file_type[i] == 3)
				*out_fd = open(cmd_lst->files[i], O_CREAT | O_RDWR | O_APPEND, 0644);
			if (*out_fd < 0)
				return (perror(cmd_lst->files[i]), 1);
		}
		i++;
	}
	return (0);
}

int	mid_loop(int pipe1[], int pipe2[], t_parsed_lst **cmd_lst, t_list **env)
{
	int		i;
	pid_t	proc_id;

	i = 1;
	while ((*cmd_lst)->next)
	{
		if (i % 2 != 0 && pipe(pipe2) < 0)
			return (perror("pipe"), -1);   //fix error mgmt
		else if (i % 2 == 0 && pipe(pipe1) < 0)
			return (perror("pipe"), -1);   //same
		proc_id = fork();
		if (proc_id < 0)
			return (perror("fork"), -1);   //same
		if (proc_id == 0)
		{
			if (i % 2 != 0)
				mid_process(pipe1, pipe2, *cmd_lst, env);
			else
				mid_process(pipe2, pipe1, *cmd_lst, env);
		}
		wait(NULL);
		i++;
		*cmd_lst = (*cmd_lst)->next;
	}
	return (i);
}

void	exec_cmds(t_parsed_lst **cmd_lst, t_list **env)
{
	int		pipe1[2];
	int		pipe2[2];
	pid_t	proc_id;
	int		i;

	//first
	if (pipe(pipe1) < 0)
		return ;//(perror("pipe"));  //faire error mgmt correct  // just un exit avec bon code?
	proc_id = fork();
	if (proc_id < 0)
		return ;//(perror("fork"));  //same
	if (proc_id == 0)
		first_process(0, pipe1, *cmd_lst, env);
	wait(NULL);
	*cmd_lst = (*cmd_lst)->next;
	//mid short
	i = mid_loop(pipe1, pipe2, cmd_lst, env);
	// last
	if (i % 2 != 0)
		last_process(pipe1, 1, *cmd_lst, env);
	else
		last_process(pipe2, 1, *cmd_lst, env);
}


int	exec_main(t_parsed_lst **cmd_lst, t_list **env, int cmd_count)
{
	int		status;
	pid_t	process_id;

	status = 0;
	if (cmd_count == 1)  // faire une fonction exec_solo ?
		replace_env(env, "_", get_last_str((*cmd_lst)->cmds));

	process_id = fork();   //les builtins qui modifie l'env marche pas apres fork !!!!!!!
	if (process_id < 0)
		return (-1);  //error fork!
	if (process_id == 0)  //conditions supplementaires ?
	{
		if (cmd_count == 1)
			solo_process(*cmd_lst, env);
		exec_cmds(cmd_lst, env);
	}
	else
		wait(&status);  //pas besoin de else si tout exit //renvoyer un status special pour exit ? pratique pour free

	return (status);
}