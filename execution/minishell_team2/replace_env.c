/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbalahce <jbalahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 16:44:03 by monabid           #+#    #+#             */
/*   Updated: 2023/01/26 02:00:59 by jbalahce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid(char c)
{
	if (check_is_symbol(c) != 0)
		return (1);
	if (is_space(c) == 1)
		return (1);
	if (c == '\'' || c == '\"')
		return (1);
	return (0);
}

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

int	replace_as_digit(char **str, int i)
{
	char	*start;
	char	*var_vale;
	char	*end;

	if (i == 0)
		start = ft_calloc(1, 1);
	else
		start = ft_substr(*str, 0, i);
	end = ft_substr(*str, i + 1, 1);
	if (start == NULL || end == NULL)
		exit(1);
	var_vale = getenv(end);
	free(end);
	end = ft_substr(*str, i + 2, ft_strlen(*str));
	free(*str);
	*str = group_strs(start, var_vale, end);
	return (i + 2);
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
	return (i + 2);
}

int	get_end_ofvar(char *str, int i)
{
	int	j;

	j = i + 1;
	while (str[j])
	{
		if (ft_isalnum(str[j]) == 1)
			j++;
		else
			break ;
	}
	return (j);
}

int	replace_as_alpha(char **str, int i)
{
	char	*start;
	char	*var_vale;
	char	*end;
	int		j;

	start = ft_substr(*str, 0, i);
	j = get_end_ofvar(*str, i);
	end = ft_substr(*str, i + 1, j);
	if (start == NULL || end == NULL)
		exit(1);
	// var_vale = getenv(end);
	var_vale = my_get_env(end, &vars.args);
	free(end);
	end = ft_substr(*str, j, ft_strlen(*str) - j);
	if (end == NULL)
		exit(1);
	free(*str);
	j = (int)ft_strlen(end);
	*str = group_strs(start, var_vale, end);
	return ((int)ft_strlen(*str) - j);
}

void	change_str(char **str)
{
	int	i;

	i = 0;
	while ((*str)[i])
	{
		if ((*str)[i] == '$')
		{
			if (ft_isdigit((*str)[i + 1]) == 1)
				i = replace_as_digit(str, i);
			else if (ft_isalpha((*str)[i + 1]) == 1)
				i = replace_as_alpha(str, i);
			else if ((*str)[i + 1] == '?')
				i = replace_as_exit_status(str, i);
			else
				i++;
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
		if (((t_range *)help->content)->type == '\"')
		{
			change_str(&(((t_range *)help->content)->str));
		}
		help = help->next;
	}
}
