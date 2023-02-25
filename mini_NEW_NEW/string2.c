/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbalahce <jbalahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 16:44:03 by monabid           #+#    #+#             */
/*   Updated: 2023/02/20 16:34:33 by jbalahce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char *))
{
	size_t	i;

	i = 0;
	if (f && s)
	{
		while (s[i])
		{
			(*f)(i, s + i);
			i++;
		}
	}
}

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;

	i = 0;
	if (dstsize == 0)
		return (ft_strlen(src));
	while (i < dstsize - 1 && src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = 0;
	return (ft_strlen(src));
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*c;
	size_t	i;

	if (!s)
		return (0);
	i = ft_strlen(s);
	if (len > i)
		len = i;
	c = (char *)ft_calloc(len + 1, 1);
	if (c == NULL)
		return (0);
	if (start <= i)
		ft_strlcpy(c, s + start, len + 1);
	return (c);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == i)
		return (0);
	while (s1[i] && s2[i] && i < n)
	{
		if (s1[i] != s2[i])
			return (*((unsigned char *)s1 + i) - *((unsigned char *)s2 + i));
		i++;
	}
	if (s1[i] && !s2[i] && i < n)
		return (*((unsigned char *)s1 + i));
	if (!s1[i] && s2[i] && i < n)
		return (-*((unsigned char *)s2 + i));
	return (0);
}

int	ft_strncmp1(const char *s1, const char *s2, size_t n)
{
	unsigned int	i;

	i = 0;
	while ((ft_toupset(s1[i]) == s2[i] \
	|| s1[i] == s2[i]) && s1[i] && s2[i] && i < n)
	{
		i++;
	}
	if (i == n)
		return (0);
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
