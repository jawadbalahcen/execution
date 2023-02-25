/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmds3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbalahce <jbalahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 20:38:54 by jbalahce          #+#    #+#             */
/*   Updated: 2023/02/11 20:39:08 by jbalahce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_valid(char *s)
{
	int	i;

	i = 1;
	if (!ft_isalpha(s[0]))
	{
		write(2, "export: ", 8);
		write(2, s, ft_strlen(s));
		write(2, " : not a valid identifier\n", 26);
		return (1);
	}
	while (s[i] && s[i] != '=')
	{
		if (!ft_isalnum(s[i]))
		{
			write(2, "export: ", 8);
			write(2, s, ft_strlen(s));
			write(2, " : not a valid identifier\n", 26);
			return (1);
		}
		i++;
	}
	return (0);
}

void	turn_list_env(t_main_args *main_args)
{
	t_list	*tmp;
	int		i;

	i = 0;
	tmp = main_args->env_lst;
	free_2d((main_args->env));
	main_args->env = malloc(sizeof(char *) * (ft_lstsize(tmp) + 1));
	while (tmp)
	{
		(main_args->env)[i] = ft_strdup(tmp->content);
		tmp = tmp->next;
		i++;
	}
	(main_args->env)[i] = NULL;
}

void	print_export(t_main_args *main_args)
{
	int		i;
	char	**key_value;

	i = 0;
	while ((main_args->env)[i])
	{
		key_value = mini_split((main_args->env)[i]);
		printf("declare -x %s", key_value[0]);
		if (key_value[1])
			printf("=\"%s\"", key_value[1]);
		printf("\n");
		free_2d(key_value);
		i++;
	}
}

int	change_value(char *var, char **key_value_var, char **key_value, t_list *tmp)
{
	if (key_value_var[1] != NULL)
	{
		if ((key_value_var[1][0] == '\0' && key_value[1] == NULL)
			|| key_value_var[1][0] != '\0')
		{
			free(tmp->content);
			tmp->content = var;
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

int	env_exist(t_main_args *main_args, char *var)
{
	int		i;
	char	**key_value;
	char	**key_value_var;
	t_list	*tmp;

	tmp = main_args->env_lst;
	i = 0;
	key_value = mini_split(tmp->content);
	key_value_var = mini_split(var);
	while (tmp && ft_strncmp(key_value[0], key_value_var[0],
			ft_strlen(key_value_var[0]) + 1))
	{
		tmp = tmp->next;
		if (tmp)
		{
			free_2d(key_value);
			key_value = mini_split(tmp->content);
		}
	}
	if (tmp)
		return (change_value(var, key_value_var, key_value, tmp));
	(free_2d(key_value), free_2d(key_value_var));
	return (1);
}
