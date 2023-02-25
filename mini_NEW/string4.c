/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbalahce <jbalahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 16:44:03 by monabid           #+#    #+#             */
/*   Updated: 2023/02/20 16:35:00 by jbalahce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isfound(char *set, char c)
{
	size_t	j;

	j = 0;
	while (set[j])
	{
		if (set[j] == c)
			return (1);
		j++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	i;
	int		j;
	char	*c;
	int		len;

	i = 0;
	len = 0;
	if (!s1)
		return (0);
	if (!set)
		return ((char *)s1);
	while (s1[i] && ft_isfound((char *)set, s1[i]) == 1)
		i++;
	j = (int)ft_strlen(s1) - 1;
	while (j > 0 && ft_isfound((char *)set, s1[j]) == 1)
	{
		j--;
	}
	if (j > 0)
		len = j - i + 1;
	c = ft_substr(s1, i, len);
	if (c == NULL)
		return (0);
	c[len] = 0;
	return (c);
}

char	*ft_strdup(const char *s1)
{
	size_t	i;
	char	*c;

	i = ft_strlen(s1);
	c = (char *)malloc(i + 1);
	if (c == NULL)
		return (NULL);
	ft_strlcpy(c, s1, i + 1);
	return (c);
}

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
