/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conv_to_cmd_checker.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: monabid <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 16:44:03 by monabid           #+#    #+#             */
/*   Updated: 2023/01/30 17:59:40 by monabid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_return(int err, t_range *this, t_range *next)
{
	if (err == 1)
		printf("bash:syntax error near unexpected token `newline'");
	if (err == 2)
		printf("bash: syntax error near unexpected token `%s'",
			next->str);
	if (err == 3)
		printf("bash: syntax error near unexpected token `%s'",
			this->str);
	return (-1);
}

int	check_tokens2(t_range *this, t_range *next, int num)
{
	if (this->type != 0)
		return (1);
	else
	{
		if (this->str[0] == '>' || this->str[0] == '<')
		{
			if (next == NULL)
				return (check_return(1, this, next));
			if (next->type != 0)
				return (0);
			return (check_return(2, this, next));
		}
		else
		{
			if (num == 1 && next != NULL)
				return (0);
			return (check_return(3, this, next));
		}
	}
}

int	check_tokens(t_list *lst)
{
	int	ok;

	ok = 0;
	while (lst)
	{
		if (lst->next == NULL)
			ok = check_tokens2((t_range *)lst->content,
					NULL, ok);
		else
			ok = check_tokens2((t_range *)lst->content,
					((t_range *)lst->next->content), ok);
		if (ok == -1)
			return (0);
		lst = lst->next;
	}
	return (1);
}
