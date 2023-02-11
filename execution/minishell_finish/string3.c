/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: monabid <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 16:44:03 by monabid           #+#    #+#             */
/*   Updated: 2023/01/16 18:56:17 by monabid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	numofstrs(char const *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] == c && s[i + 1] != c && s[i + 1])
			count++;
		i++;
	}
	if (s[0] != c && s[0])
		count++;
	return (count);
}

void	ft_addstrs(char **r, const char *s, char c)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (s[i])
	{
		if (s[i] != c)
			j++;
		if (s[i] == c && j != 0)
		{
			*r = ft_substr(s, i - j, j);
			j = 0;
			r++;
		}
		i++;
	}
	if (j != 0)
		*r = ft_substr(s, i - j, j);
}

char	**ft_split(char const *s, char c)
{
	char	**r;
	int		strnum;

	if (!s)
		return (0);
	strnum = numofstrs(s, c);
	r = (char **)malloc(sizeof(char *) * (strnum + 1));
	if (!s || r == NULL)
		return (NULL);
	r[strnum] = 0;
	ft_addstrs(r, s, c);
	return (r);
}
