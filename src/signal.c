#include "minishell.h"

void	handle_sig_dfl(int sig)
{
	int	in_cmd;

	in_cmd = 0;
	if (g_sig > 0)
		in_cmd = 1;
	if (sig == SIGINT)
	{
		g_sig = 0;
		rl_replace_line("", 0);
		write(1, "\n", 1);
		rl_on_new_line();
		if (!in_cmd)
		{
			rl_redisplay();
			g_sig = -1;
		}
	}
}

void	handle_sig_heredoc(int sig)
{
	if (sig == SIGINT)
	{
		g_sig = 0;
		write(2, "^C", 2);
		rl_replace_line("", 0);
		write(1, "\n", 1);
		rl_on_new_line();
	}
}

void	handle_sig_cat(int sig)
{
	(void)sig;
	write(2, "Quit\n", 5); //verif message
}

static void	init_signal_slash(int type)
{
	struct sigaction	sa_slash;

	if (type == S_CAT)
	{
		sa_slash.sa_handler = &handle_sig_cat;
		sigaction(SIGQUIT, &sa_slash, NULL);
	}
	else
	{
		sa_slash.sa_handler = SIG_IGN;
		sigemptyset(&sa_slash.sa_mask);
    	sa_slash.sa_flags = SA_RESTART;
		sigaction(SIGQUIT, &sa_slash, NULL);
	}
}

void	init_signal(int type)
{
	struct sigaction	sa_c;

	if (type == S_DEFAULT)
	{
		sa_c.sa_handler = &handle_sig_dfl;
		sigemptyset(&sa_c.sa_mask);
		sa_c.sa_flags = 0;
		sigaction(SIGINT, &sa_c, NULL);
	}
	else if (type == S_HEREDOC)
	{
		sa_c.sa_handler = &handle_sig_heredoc;
		sigemptyset(&sa_c.sa_mask);
		sa_c.sa_flags = 0;
		sigaction(SIGINT, &sa_c, NULL);
	}
	init_signal_slash(type);
}
