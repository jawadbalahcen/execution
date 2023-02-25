/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbalahce <jbalahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 20:15:17 by jbalahce          #+#    #+#             */
/*   Updated: 2023/02/20 18:03:11 by jbalahce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count(char **param, int c)
{
	while (param[c])
		(c)++;
	return (c);
}

int	get_index(char *s, char c)
{
	int	i;
	int	ret;

	i = 0;
	ret = -1;
	while (s[i])
	{
		if (s[i] == c)
			ret = i;
		i++;
	}
	return (ret);
}

int	arr_builtins(int i, t_cmd *cmd, t_main_args *main_args, int pid)
{
	t_f	builtin[8];

	builtin[0] = my_echo;
	builtin[1] = my_export;
	builtin[2] = my_env;
	builtin[3] = my_unset;
	builtin[4] = my_cd;
	builtin[5] = NULL;
	builtin[6] = my_pwd;
	if (i == 5)
		return (my_exit(cmd, main_args, pid));
	return (builtin[i](cmd, main_args));
}

void	handle_builtins(t_cmd *cmd, int *built)
{
	int		i;
	char	*cmd1;

	i = 0;
	cmd->cmd && (i = get_index(cmd->cmd, '/'));
	if (if_not_file(i, cmd->cmd, built))
		return ;
	cmd1 = &((cmd->cmd)[i + 1]);
	if (cmd->cmd && !ft_strncmp1(cmd1, "echo", 5))
		*built = 0;
	else if (cmd->cmd && !ft_strncmp1(cmd1, "export", 7))
		*built = 1;
	else if (cmd->cmd && !ft_strncmp1(cmd1, "env", 4))
		*built = 2;
	else if (cmd->cmd && !ft_strncmp1(cmd1, "unset", 6))
		*built = 3;
	else if (cmd->cmd && !ft_strncmp1(cmd1, "cd", 3))
		*built = 4;
	else if (cmd->cmd && !ft_strncmp1(cmd1, "exit", 5))
		*built = 5;
	else if (cmd->cmd && !ft_strncmp1(cmd1, "pwd", 4))
		*built = 6;
	else
		*built = -1;
}

char	*my_get_env(char *var, t_main_args *main_args)
{
	int	i;

	i = 0;
	while (main_args->env[i])
	{
		if (ft_strncmp(main_args->env[i], var, ft_strlen(var) + 1) == '=')
			return (main_args->env[i] + ft_strlen(var) + 1);
		i++;
	}
	return (NULL);
}
