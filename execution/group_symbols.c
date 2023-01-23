/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   group_symbols.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: monabid <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 16:44:03 by monabid           #+#    #+#             */
/*   Updated: 2023/01/18 16:36:59 by monabid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	check_is_double_symbol(char s)
{
	if (s == '|')
		return('|');
	if (s == '&')
		return('&');
	if (s == '>')
		return('>');
	if (s == '<')
		return('>');
	return (0);
}

void	join_symbols(t_list **h)
{
	t_list	*todel;
	char	*new_str;
	int		space;

	todel = (*h)->next;
	space = todel->next_is_space;
	(*h)->next = todel->next;
	new_str = ft_strjoin((*h)->content, todel->content);
	free((*h)->content);
	(*h)->content = new_str;
	(*h)->next_is_space = space;
	free(todel->content);
	free(todel);
}

void	change_chars(t_list **h)
{
	char	*new_char;

	if ((*h)->next->content[1] == 0)
	{
		join_symbols(h);
		return ;
	}
	new_char = ft_substr((*h)->next->content,
		1, ft_strlen((*h)->next->content) - 1);
	free((*h)->next->content);
	(*h)->next->content = new_char;
	free((*h)->content);
	(*h)->content = ft_substr("$?", 0, 2);
}

void	group_symbols(t_list **lst)
{
	t_list	*h;
	char	c;

	h = (*lst);
	while (h->next)
	{
		c = check_is_double_symbol(h->content[0]);
		if (h->next_is_space == 0 && h->next->content[0] != 0
			&& h->next->content[0] == c)
			join_symbols(&h);
		else if (h->next_is_space == 0 && h->next->content[0] == '?'
			&& h->content[0] == '$')
			change_chars(&h);
		if (h->next == NULL)
			break ;
		h = h->next;
	}
}
