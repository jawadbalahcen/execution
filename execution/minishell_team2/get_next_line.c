/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: monabid <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 11:57:08 by monabid           #+#    #+#             */
/*   Updated: 2023/01/16 16:42:25 by monabid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_append_buffer(int fd, char **buffer, int *i)
{
	int		num;
	char	*h;
	char	*new;

	*i = 0;
	h = ft_calloc(BUFFER_SIZE + 1, 1);
	num = read(fd, h, BUFFER_SIZE);
	if (num > 0)
	{
		*i = 1;
		new = ft_strjoin(*buffer, h);
		free(*buffer);
		*buffer = new;
	}
	free(h);
}

void	remove_line_from_buffer(char **buffer)
{
	char	*new;
	char	*h;
	int		i;
	int		j;

	if (*buffer != NULL)
	{
		h = ft_strchr(*buffer, '\n');
		if (h != NULL)
		{
			if (h[1])
			{
				i = ft_strlen(h + 1);
				j = 0;
				new = ft_strjoin(NULL, h + 1);
				free(*buffer);
				*buffer = new;
				return ;
			}
		}
		free(*buffer);
		*buffer = NULL;
	}
}

char	*get_line_from_buffer(char **buffer, int fd)
{
	char	*h;
	char	*line;
	int		i;

	i = 1;
	line = NULL;
	h = ft_strchr(*buffer, '\n');
	while (h == NULL && i == 1)
	{
		ft_append_buffer(fd, buffer, &i);
		h = ft_strchr(*buffer, '\n');
	}
	if (h != NULL)
	{
		i = -1;
		line = malloc(h - *buffer + 2);
		while (i++ < h - *buffer)
			line[i] = (*buffer)[i];
		line[i] = 0;
	}
	else
		line = ft_strjoin(NULL, *buffer);
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*buffer;
	char		*line;
	int			i;

	i = 0;
	line = NULL;
	if (buffer == NULL)
	{
		ft_append_buffer(fd, &buffer, &i);
		if (i == 0)
			return (NULL);
	}
	if (buffer != NULL)
		line = get_line_from_buffer(&buffer, fd);
	remove_line_from_buffer(&buffer);
	return (line);
}
