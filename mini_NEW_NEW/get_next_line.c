/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbalahce <jbalahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 13:22:01 by jbalahce          #+#    #+#             */
/*   Updated: 2023/02/22 21:58:32 by jbalahce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*after_nl(char *s)
{
	char	*p;
	int		i;
	int		j;

	i = 0;
	j = ft_strlen(s);
	if (!s)
		return (NULL);
	while (s[i] != '\n' && s[i])
		i++;
	if (s[i] == '\0')
	{
		free(s);
		return (NULL);
	}
	p = malloc(sizeof(char) * (j - i));
	if (!p)
		return (NULL);
	j = 0;
	i++;
	while (s[i])
		p[j++] = s[i++];
	p[j] = '\0';
	free (s);
	return (p);
}

char	*befor_nl(char *s)
{
	char	*p;
	int		i;

	i = 0;
	if (!s)
		return (NULL);
	while (s[i] != '\n' && s[i])
		i++;
	if (s[0] == '\0')
		return (NULL);
	p = malloc(sizeof(char) * i + 2);
	if (!p)
		return (NULL);
	i = 0;
	while (s[i] != '\n' && s[i])
	{
		p[i] = s[i];
		i++;
	}
	if (s[i] == '\n')
		p[i++] = '\n';
	p[i] = '\0';
	return (p);
}

int	check_nl(char *s)
{
	if (!s)
		return (0);
	while (*s)
	{
		if (*s == '\n')
			return (1);
		s++;
	}
	return (0);
}

char	*ft_read(int fd, char *buff, char *s, char *temp)
{
	int	i;

	i = 1;
	while (i != 0)
	{
		i = read(fd, buff, BUFFER_SIZE);
		if (i == -1)
		{
			free(s);
			s = 0;
			free(buff);
			return (NULL);
		}
		buff[i] = '\0';
		temp = s;
		if (!temp)
			temp = ft_strdup("");
		s = ft_strjoin(temp, buff);
		free(temp);
		if (check_nl(s))
			break ;
	}
	free(buff);
	return (s);
}

char	*get_next_line(int fd)
{
	static char	*s;
	char		*buff;
	char		*bnl;
	char		*temp;

	temp = NULL;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buff = malloc(sizeof(char) * (BUFFER_SIZE + (size_t)1));
	if (!buff)
		return (NULL);
	s = ft_read(fd, buff, s, temp);
	if (!s)
		return (NULL);
	bnl = befor_nl(s);
	s = after_nl(s);
	return (bnl);
}
