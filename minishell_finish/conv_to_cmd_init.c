/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conv_to_cmd_init.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbalahce <jbalahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 16:44:03 by monabid           #+#    #+#             */
/*   Updated: 2023/02/11 21:01:53 by jbalahce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_help(t_help *h)
{
	h->nodes = 0;
	h->infiles = 0;
	h->outfiles = 0;
	h->params = 0;
}

char	**init_param(int count)
{
	char	**param;

	param = (char **)malloc(sizeof(char *) * (count + 1));
	if (param == NULL)
		exit(1);
	param[count] = NULL;
	return (param);
}

void	count_until_token(t_help *h, t_list *lst)
{
	while (lst)
	{
		if (((t_range *)lst->content)->str != NULL)
		{
			if (((t_range *)lst->content)->type != 0)
				h->params++;
			else if (((t_range *)lst->content)->str[0] == '|'
				|| ((t_range *)lst->content)->str[0] == '&')
				break ;
			else if (((t_range *)lst->content)->str[0] == '>'
				|| ((t_range *)lst->content)->str[0] == '<')
			{
				h->nodes++;
				if (((t_range *)lst->content)->str[0] == '<')
					h->infiles++;
				else
					h->outfiles++;
				lst = lst->next;
			}
		}
		h->nodes++;
		lst = lst->next;
	}
}
