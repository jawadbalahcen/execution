/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_env_helper.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbalahce <jbalahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 14:42:06 by jbalahce          #+#    #+#             */
/*   Updated: 2023/02/25 14:57:19 by jbalahce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_expandale2(t_list *lst)
{
	t_range	*range;

	while (lst)
	{
		range = (t_range *)lst->content;
		range->expendable = 1;
		if (range->next_is_space == 1)
			break ;
		lst = lst->next;
	}
}

void	set_expandale(t_list **lst)
{
	t_list	*help;
	t_range	*range;

	del_dollar(lst);
	help = *lst;
	while (help)
	{
		range = (t_range *)help->content;
		if (range->type == 0 && ft_strcmp("<<", range->str) == 0
			&& help->next != NULL)
		{
			set_expandale2(help->next);
		}
		help = help->next;
	}
}

void	replace_str(t_list *lst)
{
	char	*new;
	t_range	*range;

	range = (t_range *)lst->content;
	if (ft_strlen(range->str) == 1)
	{
		free(range->str);
		range->str = ft_calloc(1, 1);
		return ;
	}
	new = ft_substr(range->str, 0, ft_strlen(range->str) - 1);
	if (new == NULL)
		exit(1);
	free(range->str);
	range->str = new;
}

void	del_dollar(t_list **lst)
{
	t_list	*help;
	t_range	*range;
	t_range	*range2;
	int		i;

	help = *lst;
	i = 0;
	while (help)
	{
		range = (t_range *)help->content;
		if (range->type == ' ' && range->next_is_space == 0
			&& range->str[ft_strlen(range->str) - 1] == '$')
		{
			if (help->next != NULL)
			{
				range2 = (t_range *)help->next->content;
				if (range2->type == '\'' || range2->type == '\"')
					replace_str(help);
			}
		}
		help = help->next;
	}
}
