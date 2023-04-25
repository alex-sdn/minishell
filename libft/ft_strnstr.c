/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asadanow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 13:19:09 by asadanow          #+#    #+#             */
/*   Updated: 2022/11/11 19:28:48 by asadanow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	a;
	size_t	b;

	a = 0;
	if (little[0] == '\0')
		return ((char *)big);
	while (big[a] != '\0' && a < len)
	{
		b = 0;
		while (big[a] != little[b] && big[a] != '\0' && a < len)
			a++;
		while (big[a] == little[b] && a < len)
		{
			a++;
			b++;
			if (little[b] == '\0')
				return ((char *)&big[a - b]);
		}
		a = a - b + 1;
	}
	return (NULL);
}
