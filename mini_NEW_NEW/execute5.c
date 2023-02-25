/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute5.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbalahce <jbalahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 20:25:21 by jbalahce          #+#    #+#             */
/*   Updated: 2023/02/24 18:09:45 by jbalahce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_infile(t_fds fds)
{
	if (fds.fdin == -1)
	{
		write(2, "No such file or directory\n", 26);
		return (1);
	}
	if (dup2(fds.fdin, 0) == -1)
		perror("fdin ");
	close(fds.fdin);
	return (0);
}

int	init_outfile(t_fds fds)
{
	if (fds.fdout == -1)
	{
		write(2, "No such file or directory\n", 26);
		return (1);
	}
	if (dup2(fds.fdout, 1) == -1)
		perror("fdout");
	close(fds.fdout);
	return (0);
}

int	only_one(t_cmd *cmd, t_main_args *main_args, int *built, t_norm_sake *norm)
{
	t_fds	fds;
	t_list	*order_file;

	order_file = cmd->fles->order;
	fds.fdin = 0;
	fds.fdout = 0;
	fds.pid = 1;
	handle_builtins(cmd, built);
	if (*built == -1)
		return (0);
	open_files(order_file, &fds, norm);
	if (fds.fdin != -2)
	{
		if (init_infile(fds))
			return (1);
	}
	if (fds.fdout != -2)
	{
		if (init_outfile(fds))
			return (1);
	}
	return (arr_builtins(*built, cmd, main_args, fds.pid));
}

void	setup_g_vars(int ac, char **av, char **env, t_main_args *args)
{
	t_list	*env_lst;

	env_lst = NULL;
	args->ac = ac;
	args->av = av;
	args->env = ft_envdup(env);
	turn_env_list(args->env, &env_lst);
	args->env_lst = env_lst;
	g_vars.last_exit_sat = 0;
	g_vars.args = args;
	g_vars.her_doc = -1;
}

char	**ft_envdup(char **env)
{
	int		i;
	char	**new_env;

	i = 0;
	while (env[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (env[i])
	{
		new_env[i] = ft_strdup(env[i]);
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}
