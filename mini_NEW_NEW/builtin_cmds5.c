/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buitin_cmd5.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbalahce <jbalahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 20:40:32 by jbalahce          #+#    #+#             */
/*   Updated: 2023/02/25 20:41:06 by jbalahce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	change_value(char *var, char **key_value_var, char **key_value, t_list *tmp)
{
	if (key_value_var[1] != NULL)
	{
		free(tmp->content);
		tmp->content = var;
	}
	else
		free(var);
	free_2d(key_value_var);
	free_2d(key_value);
	return (0);
}

int	append_value(char *var, char **key_value_var, char **key_value, t_list *tmp)
{
	char	*help;

	if (key_value_var[1] != NULL)
	{
		if ((key_value_var[1][0] == '\0' && key_value[1] == NULL)
			|| key_value_var[1][0] != '\0')
		{
			help = ft_strjoin(tmp->content, key_value_var[1]);
			free(tmp->content);
			free(var);
			tmp->content = help;
		}
		else
			free(var);
	}
	else
		free(var);
	free_2d(key_value_var);
	free_2d(key_value);
	return (0);
}

void	ft_help(char **c)
{
	char	*h;

	if (c[0][ft_strlen(c[0]) - 1] == '+')
	{
		h = c[0];
		c[0] = ft_substr(h, 0, ft_strlen(c[0]) - 1);
		free(h);
	}
}
