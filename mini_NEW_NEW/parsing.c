/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbalahce <jbalahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 16:44:03 by monabid           #+#    #+#             */
/*   Updated: 2023/02/20 20:54:28 by jbalahce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_space(char s)
{
	if (s == '\f' || s == '\r' || s == '\v' || s == '\n' || s == '\t'
		|| s == ' ')
	{
		return (1);
	}
	return (0);
}

char	check_is_symbol(char s)
{
	if (s == '|')
		return ('|');
	if (s == '>')
		return ('>');
	if (s == '<')
		return ('>');
	return (0);
}

void	join_nodes(t_list **h)
{
	t_list	*todel;
	char	*new_str;
	int		space;
	t_range	*range;

	todel = (*h)->next;
	range = (t_range *)(*h)->content;
	space = ((t_range *)todel->content)->next_is_space;
	(*h)->next = todel->next;
	if (((t_range *)todel->content)->str == NULL)
	{
		if (range->str != NULL)
			new_str = ft_strdup(range->str);
		else
			new_str = NULL;
	}
	else
		new_str = ft_strjoin(range->str,
				((t_range *)todel->content)->str);
	range->next_is_space = space;
	free(range->str);
	del_range(todel->content);
	free(todel);
	range->str = new_str;
}

void	join_lines(t_list **lst)
{
	t_list	*help;

	help = *lst;
	while (help)
	{
		if (help->next == NULL)
			return ;
		if (((t_range *)help->content)->type != 0
			&& ((t_range *)help->content)->next_is_space == 0)
		{
			if (((t_range *)help->next->content)->type != 0)
			{
				join_nodes(&help);
				continue ;
			}
		}
		help = help->next;
	}
}
