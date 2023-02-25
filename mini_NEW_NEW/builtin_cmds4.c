/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmds4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbalahce <jbalahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 20:41:49 by jbalahce          #+#    #+#             */
/*   Updated: 2023/02/25 20:44:30 by jbalahce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	my_export_pt2(int *i, int *ret, t_cmd *cmd, t_main_args *main_args)
{
	char	*param;
	int		p;
	int		en;

	en = 1;
	param = ft_strdup((cmd->param)[*i + 1]);
	p = check_valid(param);
	if (p == 1)
	{
		en = 0;
		*ret = 1;
	}
	if (p == 2)
	{
		if (env_exist(main_args, param, append_value))
			ft_lstadd_back(&(main_args->env_lst), \
				ft_lstnew(ft_remmove_plus(param)));
	}
	else if (!(*ret) || en)
	{
		if (env_exist(main_args, param, change_value))
			ft_lstadd_back(&(main_args->env_lst), ft_lstnew(param));
	}
	else
		free(param);
}

int	my_export(t_cmd *cmd, t_main_args *main_args)
{
	static int	i;
	static int	ret;

	if (!i)
		ret = 0;
	if (main_args->ac > 1)
	{
		my_export_pt2(&i, &ret, cmd, main_args);
		i++;
		(main_args->ac)--;
		if (main_args->ac > 1)
			my_export(cmd, main_args);
		else
			i = 0;
		turn_list_env(main_args);
	}
	else
		print_export(main_args);
	return (ret);
}

int	unset_valid(char *s)
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
	while (s[i])
	{
		if (!ft_isalnum(s[i]))
		{
			write(2, "unset: ", 7);
			write(2, s, ft_strlen(s));
			write(2, " : not a valid identifier\n", 26);
			return (1);
		}
		i++;
	}
	return (0);
}

char	**mini_split(char *var)
{
	int		i;
	char	**key_value;

	i = 0;
	while (var[i] && var[i] != '=')
		i++;
	if (!var[i])
	{
		key_value = malloc(sizeof(char *) * 2);
		key_value[0] = ft_substr(var, 0, i);
		key_value[1] = NULL;
		return (key_value);
	}
	key_value = malloc(sizeof(char *) * 3);
	key_value[0] = ft_substr(var, 0, i);
	key_value[1] = ft_substr(var, i + 1, ft_strlen(var) - (i + 1));
	key_value[2] = NULL;
	return (key_value);
}

void	unset_var(t_list **lst, char *var)
{
	t_list	*tmp_free;
	t_list	*tmp;
	char	**key_value;

	tmp = *lst;
	key_value = mini_split(tmp->next->content);
	while (tmp->next && ft_strncmp(key_value[0], var, ft_strlen(key_value[0])
			+ 1))
	{
		tmp = tmp->next;
		if (tmp->next)
		{
			free_2d(key_value);
			key_value = mini_split(tmp->next->content);
		}
	}
	if (tmp->next)
	{
		tmp_free = tmp->next;
		tmp->next = tmp->next->next;
		free(tmp_free->content);
		free(tmp_free);
	}
	free_2d(key_value);
}
