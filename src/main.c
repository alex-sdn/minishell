/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asadanow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 18:19:15 by asadanow          #+#    #+#             */
/*   Updated: 2023/04/25 18:19:17 by asadanow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_sig;

void	prompt_loop(t_list **env, t_cmd_lst *cmd_lst, int status)
{
	char		*input;

	while (1)
	{
		g_sig--;
		input = readline("\033[1;32mminishell$\x1B[0m ");
		if (g_sig < 0)
			status = 130;
		if (!input)
			ft_exit(env, NULL, status, 0);
		add_history(input);
		if (is_spaces(input) == 1)
			g_sig = 1;
		else if (check_pipes(input, 0) == 1 || check_quotes(input) == 1)
			status = 2;
		else
			cmd_lst = init_parsing(input, env, status);
		free(input);
		if (cmd_lst)
		{
			g_sig = 1;
			status = exec_main(&cmd_lst, env, cmdlst_size(cmd_lst));
			free_cmd_lst(&cmd_lst);
		}
	}
}

int	main(int ac, char **av, char **envp)
{
	t_list	*env;

	(void)ac;
	(void)av;
	g_sig = 1;
	env = init_env(NULL, envp);
	if (!env)
		return (1);
	init_signal(S_DEFAULT);
	prompt_loop(&env, NULL, 0);
	return (0);
}
