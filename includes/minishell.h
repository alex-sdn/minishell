/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asadanow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 15:40:22 by asadanow          #+#    #+#             */
/*   Updated: 2023/03/08 15:40:26 by asadanow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <stdio.h>
# include <string.h>  //strerror
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <termios.h>

# define FT_CD 1
# define FT_ECHO 2
# define FT_ENV 3
# define FT_EXIT 4
# define FT_EXPORT 5
# define FT_PWD 6
# define FT_UNSET 7

# define S_DEFAULT 1
# define S_HEREDOC 2
# define S_CAT 3

# define BUFFER_SIZE 1024

# define ERR_SNTX "syntax error near unexpected token `%s'\n"
# define ERR_HOME "cd: HOME not set\n"
# define ERR_EXPORT "export: `%s': not a valid identifier\n"
# define ERR_CMD "%s: command not found\n"
# define ERR_PERM "%s: Permission denied\n"
# define ERR_EXIT_1 "exit: %s: numeric argument required\n"
# define ERR_EXIT_2 "exit: too many arguments\n"

typedef struct s_cmd_lst {
	char	**cmds;
	char	**files;
	int		*file_type;
	int		file_amt;
	void	*head;
	void	*next;
}			t_cmd_lst;

/**   MAIN   **/
void	prompt_loop(t_list **env, t_cmd_lst *cmd_lst, int status);

/**   ENV   **/
t_list	*init_env(t_list *env, char **envp);
t_list	*init_env_i(t_list *env);
int		replace_env(t_list **env, char *env_var, char *new_content);
int		add_env(t_list **env, char *new_var);
char	*ft_getenv(t_list *env, char *var);

/**   SIGNAL   **/
void	handle_sig_dfl(int sig);
void	handle_sig_heredoc(int sig);
void	handle_sig_cat(int sig);
void	init_signal(int type);

/**   BUILTINS   **/
int		ft_cd(t_list ** env, t_cmd_lst *cmd_lst);
int		ft_echo(t_cmd_lst *cmd_lst);
int		ft_env(t_list *env, t_cmd_lst *cmd_lst);
int		ft_exit(t_list **env, t_cmd_lst **cmd_lst, int status, int fake);
int		ft_export(t_list **env, t_cmd_lst *cmd_lst);
int		ft_pwd(void);
int		ft_unset(t_list **env, t_cmd_lst *cmd_lst);

/**   UTILS   **/
int		is_special(char c);
int		is_spaces(char *str);
char	*triple_strjoin(char *str1, char *str2, char *str3);
void	printf_error(char *str, char *arg);

/**   FREE_UTILS   **/
void	free_cmd_lst(t_cmd_lst **cmd_lst);
void	ft_del(void *content);
void	free_tab(char **tab);
void	free_tab_mid(char **tab, int i);

/**   PARSING   **/
void	*free_parsing(char **tab, t_list **list, t_cmd_lst **cmd_lst);
int		has_redirections(char *str);
t_cmd_lst	*init_parsing(char *input, t_list **env, int status);

int		check_pipes(char *input, int i);
int		check_quotes(char *input);
int		check_redirec(t_list *lst);
int		ft_skip_quotes(char *str, char c, int i);
int		is_redirec(char *str);

char	*replace_env_vars(char *str, t_list *env, int status);

int		fill_heredoc(int fd, char *limiter);

int		fill_redirec(t_list *lst1, t_cmd_lst **cmd_lst, int files_count, int idx);
int 	fill_cmd_lst(t_list *lst1, t_cmd_lst **cmd_lst, int idx);
int		malloc_cmd_lst(t_cmd_lst **cmd_lst, t_cmd_lst **head, int len1, int len2);
t_cmd_lst	*create_cmd_lst(t_list *lst_split, t_cmd_lst **first, int idx);

void	cmdlst_add_back(t_cmd_lst **cmd_lst, t_cmd_lst *new);
int		cmdlst_size(t_cmd_lst *cmd_lst);
void	free_cmd_lst_solo(t_cmd_lst **cmd_lst, int cmds_count, int files_count);
char	*malloc_no_quotes(char *str);
char	*strdup_quotes(char *str);

t_list	*lst_split_quotes(char *str, char c);
int		get_redirec_pos(char *str);
int		lst_split_redirec(t_list **head, int i);

char	**ft_split_quotes(char *s, char c);

/**   EXEC   **/
int		open_files(int *in_fd, int *out_fd, t_cmd_lst *cmd_lst);
void	err_access(t_cmd_lst *cmd_lst, int pos, int err);
char	*check_access(char *cmd, t_list **env, int i);
int		exec_pipes(int **pipes, t_cmd_lst **cmd_lst, t_list **env);
int		exec_main(t_cmd_lst **cmd_lst, t_list **env, int cmd_count);

void	loop_process(int fd_in, int fd_out, t_cmd_lst **cmd_lst, t_list **env);
int		solo_process(t_cmd_lst **cmd_lst, t_list **env);
int		exec_builtin(t_cmd_lst **cmd_lst, t_list **env, int solo);

int		**init_pipes(int size);
void	free_pipes(int **pipes, int size);
void	start_process(int i, int **pipes, t_cmd_lst **cmd_lst, t_list **env);
int		wait_procs(int **pipes, pid_t *proc_ids, int cmd_count, t_cmd_lst *cmd_lst);

char	**get_paths(t_list **env);
char	**create_env_tab(t_list *env);
char	*get_last_str(char **tab);
int		convert_status(int status);

void	restore_std_in_out(int fd0, int fd1);
int		is_builtin(char *cmd);
int		is_solo_cat(t_cmd_lst *cmd_lst);

char	**ft_split_pipex(char const *s, char c);

/**   GNL   **/
char	*ft_strcat(char *dest, char *src);
void	*ft_exit_error(char **str);
void	*ft_exit_error_duo(char **str1, char **str2);
int		ft_check_line(ssize_t rd, char *str);
char	*ft_dupcat(char *dest, char *src);
char	*ft_dup_line(char *str);
char	*ft_remove_line(char *save);
char	*get_next_line(int fd);

extern int sig_global;

#endif
