/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbalahce <jbalahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 23:21:17 by jbalahce          #+#    #+#             */
/*   Updated: 2023/01/23 20:48:03 by jbalahce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	my_cd(t_cmd *cmd, t_main_args *main_args)
{
	char	*home_dir;

	home_dir = getenv("HOME");
	if (main_args->ac == 1)
		chdir(home_dir);
	else if (main_args->ac == 2)
	{
		if (chdir((cmd->param)[1]))
			perror("cd");
	}
}

void	my_echo(t_cmd *cmd, t_main_args *main_args)
{
	int	is_flag;
	int	i;

	is_flag = 0;
	i = 1;
	if (main_args->ac > 1)
	{
		if (!ft_strcmp((cmd->param)[i], "-n"))
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
}

void	my_pwd(t_main_args *main_args)
{
	printf("%s\n", my_get_env("PWD", main_args));
	// printf("%s\n", getenv("PWD"));
}

void	my_env(t_main_args *main_args)
{
	int	i;

	i = 0;
	while ((main_args->env)[i])
	{
		printf("%s\n", (main_args->env)[i]);
		i++;
	}
}

void	my_export(t_cmd *cmd, t_main_args *main_args)
{
	static int	i;
	int			count;
	char		*new_var;
	char		**new_env;

	count = 0;
	while ((main_args->env)[count])
		count++;
	new_env = malloc(sizeof(char *) * (count + 2));
	if (main_args->ac == 1)
		my_env(main_args);
	else
	{
		new_var = ft_strdup((cmd->param)[i + 1]);
		count = 0;
		while ((main_args->env)[count])
		{
			new_env[count] = (main_args->env)[count];
			count++;
		}
		new_env[count] = new_var;
		new_env[count + 1] = NULL;
		main_args->env = new_env;
	}
	(main_args->ac)--;
	i++;
	if (main_args->ac > 1)
		my_export(cmd, main_args);
	else
		i = 0;
}

int	check_valid(char *s)
{
	while (*s)
	{
		if (*s == '=')
			return (1);
		s++;
	}
	return (0);
}

void	my_unset(t_cmd *cmd, t_main_args *main_args)
{
	int			i;
	int			j;
	char		**new_env;
	static int	a;

	i = 0;
	j = 0;
	if (main_args->ac == 1)
	{
		write(2, "unset: not enough arguments\n", 28);
		return ;
	}
	if (check_valid((cmd->param)[a + 1]))
	{
		write(2, "not a valid identifier\n", 23);
		return ;
	}
	while ((main_args->env)[i] && ft_strncmp((main_args->env)[i], (cmd->param)[a
			+ 1], ft_strlen((cmd->param)[a + 1])))
		i++;
	if ((main_args->env)[i] != NULL)
	{
		while ((main_args->env)[i])
			i++;
		new_env = malloc(sizeof(char *) * i);
		i = 0;
		while ((main_args->env)[i])
		{
			if (ft_strncmp((main_args->env)[i], (cmd->param)[a + 1],
					ft_strlen((cmd->param)[a + 1])))
			{
				new_env[j] = (main_args->env)[i];
				j++;
			}
			i++;
		}
		new_env[j] = NULL;
		(main_args->env) = new_env;
	}
	(main_args->ac)--;
	a++;
	if (main_args->ac > 1)
		my_unset(cmd, main_args);
	else
		a = 0;
}

void	my_exit(void)
{
	write(1, "exit\n", 5);
	exit(0);
}