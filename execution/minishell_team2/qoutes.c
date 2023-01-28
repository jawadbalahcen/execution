/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   qoutes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: monabid <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 16:44:03 by monabid           #+#    #+#             */
/*   Updated: 2023/01/23 16:20:12 by monabid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	insert(t_list **lst, char* str,int type, char space)
{
	t_range	*range;
	t_list	*new;

	range = malloc(sizeof(t_range));
	if (range == NULL)
		exit(1);
	range->next_is_space = space;
	range->type = type;
	range->str = str;
	new = ft_lstnew(range);
	if (range == NULL)
		exit(1);
	ft_lstadd_back(lst, new);
}

int	insert_qoutes(t_list **lst, int i, char *line)
{
	char	qoute;
	char	*str;
	int		j;

	qoute = line[i];
	j = i + 1;
	while (line[j])
	{
		if (line[j] == qoute)
		{
			str = ft_substr(line, i + 1, j - i - 1);
			if (str == NULL && i - j - 1 > 0)
				exit(1);
			insert(lst, str, qoute, is_space(line[j + 1]));
				return (j + 1);
		}
		j++;
	}
	write(2, "Error: unclosed qoutes ", 24);
	write(2, &qoute, 1);
	return (j);
}

int	insert_symbol(t_list **lst, int i, char *line)
{
	char	*str;

	if (line[i] == line[i + 1])
	{
		str = malloc(3);
		str[0] = line[i];
		str[1] = line[i + 1];
		str[2] = 0;
		insert(lst, str, 0, is_space(line[i + 2]));
		return (i + 2);
	}
	str = malloc(2);
	str[0] = line[i];
	str[1] = 0;
	insert(lst, str, 0, is_space(line[i + 1]));
	return (i + 1);
}

int	stop_here(char c)
{
	if (check_is_symbol(c) != 0)
		return (1);
	if (is_space(c) == 1)
		return (1);
	if (c == '\'' || c == '\"')
		return (1);
	return (0);
}

int	insert_string(t_list **lst, int i, char *line)
{
	char	qoute;
	char	*str;
	int		j;

	qoute = line[i];
	j = i;
	while (line[j])
	{
		if (stop_here(line[j + 1]) == 1 || line[j + 1] == 0)
		{
			str = ft_substr(line, i, j - i + 1);
			if (str == NULL)
				exit(1);
			insert(lst, str, '\"', is_space(line[j + 1]));
			return (j + 1);
		}
		j++;
	}
	return (ft_strlen(line));
}

t_list *qoutes_handling(char *line)
{
	t_list	*lst;
	int		i;

	i = 0;
	lst = NULL;
	while (line[i])
	{
		if (is_space(line[i]) == 1)
			i++;
		else if (line[i] == '\'' || line[i] == '\"')
			i = insert_qoutes(&lst, i, line);
		else if (check_is_symbol(line[i]) != 0)
			i = insert_symbol(&lst, i, line);
		else
			i = insert_string(&lst, i, line);
	}
	return (lst);
}
