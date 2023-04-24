#include "minishell.h"

static void	swap_termios(void)
{
	struct termios	termi_tmp;

	tcgetattr(0, &termi_tmp);
	termi_tmp.c_lflag &= ~ECHOCTL;
	tcsetattr(0, TCSAFLUSH, &termi_tmp);
}

static char	*gnl_heredoc(char *limiter, int loop)
{
	char			*line;
	char			*new;
	struct termios	termi_og;

	sig_global = 1;
	tcgetattr(0, &termi_og);
	swap_termios();
	init_signal(S_HEREDOC);
	line = get_next_line(0);
	init_signal(S_DEFAULT);
	tcsetattr(0, TCSANOW, &termi_og);
	if (!line && !sig_global)
		return (NULL);
	else if (!line && !loop) //msg additionel ? (dans stderr)
		return (write(1, "\n", 1), ft_strjoin(limiter, "\n"));
	else if (!line)
		return (ft_strjoin("", gnl_heredoc(limiter, 1)));
	if (line[ft_strlen(line) - 1] != '\n')
	{
		new = ft_strjoin(line, gnl_heredoc(limiter, 1));
		free(line);
		return (new);
	}
	return (line);
}

//devrait etre ok. mettre dans fichier sep
int	fill_heredoc(int fd, char *limiter)
{
	char	*line;

	write(1, "> ", 2);
	line = gnl_heredoc(limiter, 0);
	while (line && !(ft_strncmp(line, limiter, ft_strlen(limiter)) == 0
		&& ft_strlen(line) == ft_strlen(limiter) + 1))
	{
		ft_putstr_fd(line, fd);
		free(line);
		write(1, "> ", 2);
		line = gnl_heredoc(limiter, 0);
	}
	close(fd);
	if (!line)
		return (1);
	free(line);
	return (0);
}
