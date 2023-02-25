/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conv_to_cmd_checker.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbalahce <jbalahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 16:44:03 by monabid           #+#    #+#             */
/*   Updated: 2023/02/12 16:59:11 by jbalahce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	error_print1(char *s)
{
	write(2, "syntax error near unexpected token ", 35);
	write(2, s, ft_strlen(s));
	write(2, "\n", 1);
}

int	check_return(int err, t_range *this, t_range *next)
{
	if (err == 1)
		error_print1("newline");
	if (err == 2)
		error_print1(next->str);
	if (err == 3)
		error_print1(this->str);
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
