/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asadanow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 13:38:05 by asadanow          #+#    #+#             */
/*   Updated: 2022/11/11 17:34:04 by asadanow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *nptr)
{
	int	i;
	int	sign;
	int	ans;

	i = 0;
	sign = 1;
	ans = 0;
	while (nptr[i] == ' ' || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if (nptr[i] == '+')
		i++;
	else if (nptr[i] == '-')
	{
		i++;
		sign = -1;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		ans = ans * 10 + nptr[i] - 48;
		i++;
	}
	return (ans * sign);
}
