/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_env_alpha.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbalahce <jbalahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 20:52:13 by jbalahce          #+#    #+#             */
/*   Updated: 2023/02/11 21:25:38 by jbalahce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_end_ofvar(char *str, int i)
{
	int	j;

	j = i + 1;
	while (ft_isalnum(str[j]) == 1)
	{
		j++;
	}
	return (j - i - 1);
}

int	replace_as_alpha2(char **str, int i, int type, int j)
{
	char	*start;
	char	*var_vale;
	char	*end;

	(void)type;
	start = ft_substr(*str, 0, i);
	end = ft_substr(*str, i + 1, j);
	if (start == NULL || end == NULL)
		exit(1);
	var_vale = my_get_env(end, g_vars.args);
	free(end);
	end = ft_substr(*str, j + i + 1, ft_strlen(*str) - j + i + 1);
	if (end == NULL)
		exit(1);
	free(*str);
	j = (int)ft_strlen(end);
	*str = group_strs(start, var_vale, end);
	return ((int)ft_strlen(*str) - j);
}

int	replace_as_alpha(char **str, int i, int type)
{
	char	*start;
	char	*var_vale;
	char	*end;
	int		j;

	j = get_end_ofvar(*str, i);
	if (i == 0 && j + 1 == (int)ft_strlen(*str) && type == ' ')
	{
		start = NULL;
		end = ft_substr(*str, i + 1, j);
		if (end == NULL)
			exit(1);
		var_vale = my_get_env(end, g_vars.args);
		free(end);
		end = NULL;
		if (var_vale == NULL)
		{
			free(*str);
			*str = NULL;
			return (-1);
		}
	}
	return (replace_as_alpha2(str, i, type, j));
}
