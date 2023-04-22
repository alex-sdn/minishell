#include "minishell.h"


t_parsed_lst	*create_cmd_lst(char **cmds, char **files, int type_1, int type_2, int file_amt)
{
	t_parsed_lst	*new;
	int		*file_type;

	file_type = malloc(sizeof(int) * file_amt);

	new = malloc(sizeof(t_parsed_lst));

	new->cmds = cmds;
	new->files = files;
	if (file_amt == 1)
		file_type[0] = type_1;
	if (file_amt == 2)
	{
		file_type[0] = type_1;
		file_type[1] = type_2;
	}
	new->file_type = file_type;
	new->file_amt = file_amt;

	return (new); 
}

int	main(int ac, char **av, char **envp)
{
	t_list	*env;

	(void)ac;
	(void)av;
	env = NULL;
	env = init_env(env, envp);

	t_parsed_lst	*parsed_lst;
	t_parsed_lst	*head;

	parsed_lst = create_cmd_lst(ft_split("echo!asadanow! text   ", '!'), ft_split("outfile", '!'), 2, 0, 1);
	head = parsed_lst;
	parsed_lst->next = create_cmd_lst(ft_split("cat", '!'), ft_split("Makefile", '!'), 0, 0, 1);
	parsed_lst = parsed_lst->next;
	parsed_lst->next = create_cmd_lst(ft_split("grep.c | head -2", '!'), NULL, 0, 0, 0);
	parsed_lst = parsed_lst->next;
	parsed_lst->next = create_cmd_lst(ft_split("ls!-l ", '!'), ft_split("outfile2", '!'), 1, 0, 0);

	// printf("TEST PRINT:\n\n");
	// parsed_lst = head;
	// int	i;
	// while (parsed_lst)
	// {
	// 	i = -1;
	// 	printf("cmds = { ");
	// 	while (parsed_lst->cmds[++i])
	// 		printf("'%s' ", parsed_lst->cmds[i]);
	// 	printf("}\n");
	// 	printf("in_file = %s\nout_file = %s\nout_mode = %d\n\n", parsed_lst->in_file, parsed_lst->out_file, parsed_lst->out_mode);
	// 	parsed_lst = parsed_lst->next;
	// }

	int cmd_count = parsed_lst_size(head);
	int last = exec_main(&head, &env, cmd_count);
	printf("last = %d\n", last);

	ft_lstclear(&env, &ft_del);
	//doit free la parsed lst aussi mais osef
	return (0);
}