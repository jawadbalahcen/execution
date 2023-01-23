/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbalahce <jbalahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/21 00:37:56 by jbalahce          #+#    #+#             */
/*   Updated: 2023/01/21 00:39:27 by jbalahce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	unsigned int	i;

	i = 0;
	while (s1[i] == s2[i] && s1[i] && s2[i])
	{
		i++;
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
char	*ft_strdup(const char *s1)
{
	int		lenght;
	char	*copy;

	lenght = 0;
	while (s1[lenght] != '\0')
		lenght++;
	copy = malloc(sizeof(char) * (lenght + 1));
	if (!(copy))
		return (NULL);
	lenght = 0;
	while (s1[lenght])
	{
		copy[lenght] = s1[lenght];
		lenght++;
	}
	copy[lenght] = '\0';
	return (copy);
}