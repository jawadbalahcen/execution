/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbalahce <jbalahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 23:21:17 by jbalahce          #+#    #+#             */
/*   Updated: 2023/02/22 14:18:09 by jbalahce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	my_unset(t_cmd *cmd, t_main_args *main_args)
{
	static int	i;
	static int	ret;

	if (!i)
		ret = 0;
	if (main_args->ac > 1)
	{
		if (unset_valid((cmd->param)[1 + i]))
			ret = 1;
		if (!ret)
			unset_var(&(main_args->env_lst), (cmd->param)[1 + i]);
		i++;
		(main_args->ac)--;
		if (main_args->ac > 1)
			my_unset(cmd, main_args);
		else
			i = 0;
	}
	turn_list_env(main_args);
	return (ret);
}

void	my_exit_pt2(t_cmd *cmd)
{
	int	i;

	i = 0;
	if (cmd->param[1][i] == '-' || cmd->param[1][i] == '+')
		i++;
	while (ft_isdigit(cmd->param[1][i]))
		i++;
	if (cmd->param[1][i])
	{
		write(2, "exit: ", 6);
		write(2, cmd->param[1], ft_strlen(cmd->param[1]));
		write(2, ": numeric argument required\n", 28);
		exit(255);
	}
}

int	my_exit(t_cmd *cmd, t_main_args *main_args, int pid)
{
	if (pid != 0)
		write(2, "exit\n", 5);
	if (main_args->ac != 1)
		my_exit_pt2(cmd);
	if (main_args->ac == 2)
		exit(ft_atoi(cmd->param[1]));
	else if (main_args->ac > 2)
	{
		write(2, "exit: too many arguments\n", 25);
		return (1);
	}
	else
	{
		if (pid != 0)
		{
			if (g_vars.last_exit_sat == 1)
				exit(g_vars.last_exit_sat);
			exit(WEXITSTATUS(g_vars.last_exit_sat));
		}
		else
			exit(0);
	}
	return (0);
}

void	print_error(char *s)
{
	write(2, s, ft_strlen(s));
	write(2, "\n", 1);
	exit(1);
}
