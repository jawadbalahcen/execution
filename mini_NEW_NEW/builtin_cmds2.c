/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmds2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbalahce <jbalahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 20:36:43 by jbalahce          #+#    #+#             */
/*   Updated: 2023/02/25 17:27:01 by jbalahce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_flag(char *s)
{
	int	i;

	i = 0;
	if (s[i] == '-')
	{
		i++;
		while (s[i] && s[i] == 'n')
			i++;
	}
	if (s[i] || i == 1)
		return (0);
	return (1);
}

int	my_echo(t_cmd *cmd, t_main_args *main_args)
{
	int	is_flag;
	int	i;

	is_flag = 0;
	i = 1;
	if (main_args->ac > 1)
	{
		while ((cmd->param)[i] && (cmd->param)[i][0]
			&& check_flag((cmd->param)[i]))
		{
			is_flag = 1;
			i++;
		}
		while ((cmd->param)[i])
		{
			write(1, (cmd->param)[i], ft_strlen((cmd->param)[i]));
			if ((cmd->param)[i + 1])
				write(1, " ", 1);
			i++;
		}
	}
	if (!is_flag)
		write(1, "\n", 1);
	return (0);
}

int	my_pwd(t_cmd *cmd, t_main_args *main_args)
{
	char	*cwd;

	(void)main_args;
	(void)cmd;
	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		printf("%s\n", cwd);
		free(cwd);
	}
	else
	{
		cwd = my_get_env("PWD", main_args);
		printf("%s\n", cwd);
	}
	return (0);
}

int	there_equal(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '=')
			return (i);
		i++;
	}
	return (-1);
}

int	my_env(t_cmd *cmd, t_main_args *main_args)
{
	int	i;

	(void)cmd;
	i = 0;
	while ((main_args->env)[i])
	{
		if (there_equal((main_args->env)[i]) != -1)
			printf("%s\n", (main_args->env)[i]);
		i++;
	}
	return (0);
}
