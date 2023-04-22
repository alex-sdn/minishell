#include "minishell.h"

int	is_special(char c)
{
	if (c == '\'' || c == '"' || c == '<' || c == '>' || c == '$'
		|| c == ' ' || c == '\0')
		return (1);
	return (0);
}

int	is_spaces(char *str)
{
	int	i;

	i = 0;
	while (str[i] && (str[i] == ' ' || str[i] == 9))
		i++;
	if (!str[i])
		return (1);
	return (0);
}

char	*triple_strjoin(char *str1, char *str2, char *str3)
{
	char	*new1;
	char	*new2;

	if (str1)
	{
		new1 = ft_strjoin(str1, str2);
		if (!new1)
			return (NULL);
	}
	else
		new1 = str2;
	if (str3)
	{
		new2 = ft_strjoin(new1, str3);
		free(new1);
		return (new2);
	}
	else
		return (new1);
}

char	*ft_cdcat(char *curr, char *new)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (curr[i])
		i++;
	curr[i++] = '/';
	while (new[j])
		curr[i++] = new[j++];
	curr[i] = '\0';
	return (curr);
}

void	ft_del(void *content)
{
	if (content)
		free(content);
}

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	if (!tab)
		return ;
	while (tab[i])
		free(tab[i++]);
	free(tab[i]);
	free(tab);
}

void	free_tab_mid(char **tab, int i)
{
	while (tab[i])
		free(tab[i++]);
	free_tab(tab);
}

void	printf_error(char *str, char *arg)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '%')
		{
			ft_putstr_fd(arg, 2);
			i++;
		}
		else
			ft_putchar_fd(str[i], 2);
		i++;
	}
}
