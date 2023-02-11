/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: monabid <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 16:44:03 by monabid           #+#    #+#             */
/*   Updated: 2023/01/29 20:16:43 by monabid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
//xx delete is_valid
char	*group_strs(char *start, char *var_vale, char *end)
{
	char	*join1;
	char	*new_str;

	if (var_vale == NULL)
	{
		new_str = ft_strjoin(start, end);
		if (new_str == NULL)
			exit(1);
		free(start);
		free(end);
		return (new_str);
	}
	join1 = ft_strjoin(start, var_vale);
	if (join1 == NULL)
		exit(1);
	free(start);
	new_str = ft_strjoin(join1, end);
	if (new_str == NULL)
		exit(1);
	free(join1);
	free(end);
	return (new_str);
}

int	replace_as_digit(char **str, int i, int type)
{
	if (type == ' ')
	{
		*str = NULL;
		return (-1);
	}
	else
		*str = ft_calloc(1, 1);
	return (i);
}

int	replace_as_exit_status(char **str, int i)
{
	char	*start;
	char	*var_vale;
	char	*end;

	start = ft_substr(*str, 0, i);
	end = ft_substr(*str, i + 1, 1);
	if (start == NULL || end == NULL)
		exit(1);
	if (WIFEXITED(vars.last_exit_sat))
		var_vale = ft_itoa(WEXITSTATUS(vars.last_exit_sat));
	else
		var_vale = ft_itoa(vars.last_exit_sat);
	free(end);
	end = ft_substr(*str, i + 2, ft_strlen(*str) - (i + 1));
	free(*str);
	*str = group_strs(start, var_vale, end);
	free(var_vale);
	return (i);
}

void	change_str(char **str, int type)
{
	int	i;

	i = 0;
	while ((*str)[i])
	{
		if ((*str)[i] == '$')
		{
			if (ft_isdigit((*str)[i + 1]) == 1)
				i = replace_as_digit(str, i, type);
			else if (ft_isalpha((*str)[i + 1]) == 1)
				i = replace_as_alpha(str, i, type);
			else if ((*str)[i + 1] == '?')
				i = replace_as_exit_status(str, i);
			else
				i++;
			if (i == -1)
				return ;
		}
		else
			i++;
	}
}

void	replace_env(t_list **lst)
{
	t_list	*help;

	help = *lst;
	while (help)
	{
		if (((t_range *)help->content)->type == '\"' ||
			((t_range *)help->content)->type == ' ')
		{
			change_str(&(((t_range *)help->content)->str),
			((t_range *)help->content)->type);
		}
		help = help->next;
	}
}
