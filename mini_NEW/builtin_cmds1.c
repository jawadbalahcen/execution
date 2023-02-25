/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmds1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbalahce <jbalahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 20:33:12 by jbalahce          #+#    #+#             */
/*   Updated: 2023/02/20 15:25:46 by jbalahce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_2d(char **s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		free(s[i]);
		i++;
	}
	free(s);
}

int	my_set_env(char *var, char *value, t_main_args *main_args)
{
	int		i;
	char	*tmp;

	i = 0;
	while (main_args->env[i] && ft_strncmp(main_args->env[i], var,
			ft_strlen(var)))
		i++;
	if (main_args->env[i])
	{
		var = ft_strjoin(var, "=");
		tmp = var;
		free((main_args->env)[i]);
		(main_args->env)[i] = ft_strjoin(var, value);
		free(tmp);
		return (1);
	}
	return (0);
}

int	cd_home(char *home_dir, char *prev_dir, t_main_args *main_args)
{
	char	*cwd;

	if (chdir(home_dir) == -1)
	{
		write(2, "cd: HOME not set\n", 17);
		return (1);
	}
	else
	{
		cwd = getcwd(NULL, 0);
		if (!cwd)
		{
			perror("Can't get the current working directory");
			return (1);
		}
		else
		{
			if (prev_dir)
				my_set_env("OLDPWD", prev_dir, main_args);
			my_set_env("PWD", cwd, main_args);
		}
		free(cwd);
	}
	return (0);
}

int	cd_some_where(char *dir, char *prev_dir, t_main_args *main_args)
{
	char	*cwd;

	if (chdir(dir) == -1)
	{
		write(2, dir, ft_strlen(dir));
		write(2, " : No such file or directory\n", 29);
		return (1);
	}
	else
	{
		cwd = getcwd(NULL, 0);
		if (!cwd)
		{
			perror(CD_ER ROR);
			return (1);
		}
		else
		{
			if (prev_dir)
				my_set_env("OLDPWD", prev_dir, main_args);
			my_set_env("PWD", cwd, main_args);
		}
		free(cwd);
	}
	return (0);
}

int	my_cd(t_cmd *cmd, t_main_args *main_args)
{
	char	*home_dir;
	char	*prev_dir;
	int		ret;

	prev_dir = getcwd(NULL, 0);
	home_dir = my_get_env("HOME", main_args);
	if (main_args->ac == 1)
	{
		ret = cd_home(home_dir, prev_dir, main_args);
	}
	else
	{
		ret = cd_some_where((cmd->param)[1], prev_dir, main_args);
	}
	free(prev_dir);
	return (ret);
}
