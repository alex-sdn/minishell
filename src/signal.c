#include "minishell.h"

void	handle_sig(int sig)
{
	int	in_cmd;

	in_cmd = 0;
	if (sig_global > 0)
		in_cmd = 1;
	if (sig == SIGINT)
	{
		sig_global = 0;
		rl_replace_line("", 0);
		write(1, "\n", 1);
		rl_on_new_line();
		if (!in_cmd)
		{
			rl_redisplay();
			sig_global = -1;
		}
	}
}

void	init_signal(void)
{
	struct sigaction	sa1;
	struct sigaction	sa2;

	sa1.sa_handler = &handle_sig;
	sigemptyset(&sa1.sa_mask);
	sa1.sa_flags = 0;
	sigaction(SIGINT, &sa1, NULL);
	sa2.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa2, NULL);
}
