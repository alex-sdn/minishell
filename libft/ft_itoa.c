/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asadanow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 16:16:46 by asadanow          #+#    #+#             */
/*   Updated: 2022/11/09 18:00:50 by asadanow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_reslen(int n)
{
	int	count;

	count = 0;
	if (n == 0)
		return (1);
	if (n < 0)
		count++;
	while (n != 0)
	{
		count++;
		n = n / 10;
	}
	return (count);
}

static char	*ft_alloc(int n)
{
	char	*dest;

	dest = malloc(sizeof(char) * (ft_reslen(n) + 1));
	if (!dest)
		return (NULL);
	if (n < 0)
		dest[0] = '-';
	dest[ft_reslen(n)] = '\0';
	return (dest);
}

static char	*ft_revtab(int size, char *dest)
{
	int		i;
	char	tmp;

	i = 0;
	if (dest[i] == '-')
		i++;
	while (--size > i)
	{
		tmp = dest[i];
		dest[i] = dest[size];
		dest[size] = tmp;
		i++;
	}
	return (dest);
}

char	*ft_itoa(int n)
{
	int				i;
	unsigned int	new;
	char			*dest;

	i = 0;
	dest = ft_alloc(n);
	if (!dest)
		return (NULL);
	if (n == 0)
		dest[0] = '0';
	if (n < 0)
	{
		new = -n;
		i++;
	}
	else
		new = n;
	while (new != 0)
	{
		dest[i] = (new % 10) + 48;
		new = new / 10;
		i++;
	}
	return (ft_revtab(i, dest));
}
