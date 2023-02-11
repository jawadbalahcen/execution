/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbalahce <jbalahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 16:44:03 by monabid           #+#    #+#             */
/*   Updated: 2023/02/11 14:44:10 by jbalahce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sigint_handler(int sig)
{
	sig = 1;
	//exit(1); code if read line dont exist
	write(1, "\n", 1);
	rl_replace_line("", 1);
	rl_on_new_line();
	rl_redisplay();
	vars.last_exit_sat = 1;
}

void	sigauit_handler(int sig)
{
	(void)sig;
}

void	init_signals(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, sigauit_handler);
}

void	turn_env_list(char **env, t_list **env_lst)
{
	int	i;

	i = 0;
	while (env[i])
	{
		ft_lstadd_back(env_lst, ft_lstnew(ft_strdup(env[i])));
		i++;
	}
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

void	setup_vars(int ac, char **av, char **env ,t_main_args *args)
{
	t_list	*env_lst;

	env_lst = NULL;
	args->ac = ac;
	args->av = av;
	args->env = ft_envdup(env);
	turn_env_list(args->env, &env_lst);
	args->env_lst = env_lst;
	vars.last_exit_sat = 0;
	vars.args = args;
}

int	main(int ac, char **av, char **env)
{
	char	*line;
	t_list	*lines;
	t_main_args	args;

	lines = NULL;
	setup_vars(ac, av, env, &args);
	init_signals();
	while (1337)
	{
		line = readline(("minishell $> "));
		if (line == NULL)
		{
			write(1, "exit\n", 5);
			if (vars.last_exit_sat == 1)
				exit(vars.last_exit_sat);
			exit(WEXITSTATUS(vars.last_exit_sat));
		}
		if (*line != 0)
			add_history(line);
		handle_line(line, &args);
		free(line);
	}
	//code if read line dont exist
	// while (1337)
	// {
	// 	write(1, "minishell $> ", 13);
	// 	line = get_next_line(0);
	// 	if (line != 0)
	// 	{
	// 		if (line[0] != '\n')
	// 			handle_line(line);
	// 		free(line);
	// 	}
	// 	else
	// 	{
	// 		write(1, "exit\n", 5);
	// 		exit(0);
	// 	}
	// }
	return (0);
}
