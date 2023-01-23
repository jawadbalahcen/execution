/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_by_sptial_chars.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: monabid <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 16:44:03 by monabid           #+#    #+#             */
/*   Updated: 2023/01/18 16:11:48 by monabid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	check_is_symbol(char s)
{
	if (s == '|')
		return('|');
	if (s == '&')
		return('&');
	if (s == '>')
		return('>');
	if (s == '<')
		return('>');
	if (s == '\'')
		return('\'');
	if (s == '\"')
		return('\"');
	if (s == '$')
		return('$');
	if (s == '*')
		return('*');
	return (0);
}

void	add_to_lst(t_list	**lst, char *c, int alloc, int is_space)
{
	t_list	*new;
	char	*n;

	new = malloc(sizeof(new));
	if (new == 0)
		exit(1);
	if (alloc == 1)
	{
		if (c[1] == 0)
			is_space = 1;
		n = ft_calloc(2, 1);
		if (c == NULL)
			exit(1);
		*n = *c;
		c = n;
	}
	new = ft_lstnew(c, is_space);
	if (new == NULL)
		exit (1);
	ft_lstadd_back(lst, new);
}

void	alloc_add(t_list **new, char *s, int i, int j)
{
	char	*l;

	l = ft_substr(s, i - j, j);
	if (l == NULL)
		exit(1);
	if (i == (int)ft_strlen(s))
		add_to_lst(new, l, 0, 1);
	else
		add_to_lst(new, l, 0, 0);
}

t_list	*convert(char *s)
{
	t_list	*new;
	int		i;
	int		j;

	i = 0;
	j = 0;
	new = NULL;
	while (s[i])
	{
		if (check_is_symbol(s[i]) == 0)
			j++;
		else
		{
			if (i == 0 || j == 0)
				add_to_lst(&new, s + i, 1, 0);
			else
			{
				alloc_add(&new, s, i, j);
				i--;
			}
			j = 0;
		}
		i++;
	}
	if (j != 0)
		alloc_add(&new, s, i, j);
	return (new);
}

t_list	*split_by_special_char(char **split)
{
	t_list	*str;
	int		i;

	str = NULL;
	i = 0;
	while (split[i])
	{
		ft_lstadd_back(&str, convert(split[i]));
		free(split[i]);
		i++;
	}
	free(split);
	return (str);
}
