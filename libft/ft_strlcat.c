/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asadanow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 12:31:30 by asadanow          #+#    #+#             */
/*   Updated: 2022/11/09 16:21:08 by asadanow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	a;
	size_t	lendest;
	size_t	lensrc;

	a = 0;
	lendest = ft_strlen(dst);
	lensrc = ft_strlen(src);
	if (size == 0 || lendest >= size)
		return (lensrc + size);
	while (src[a] != '\0' && (a + lendest) < size - 1)
	{
		dst[lendest + a] = src[a];
		a++;
	}
	dst[lendest + a] = '\0';
	return (lendest + lensrc);
}
