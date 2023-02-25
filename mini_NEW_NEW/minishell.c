/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbalahce <jbalahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 16:44:03 by monabid           #+#    #+#             */
/*   Updated: 2023/02/25 15:30:55 by jbalahce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sigint_handler(int sig)
{
	sig = 1;
	if (g_vars.her_doc != -1)
	{
		if (g_vars.her_doc == 0)
			exit(1);
	}
	write(1, "\n", 1);
	if (g_vars.line_handled == 0)
	{
		rl_replace_line("", 1);
		rl_on_new_line();
		rl_redisplay();
	}
	g_vars.last_exit_sat = 1;
}

void	sigauit_handler(int sig)
{
	if (g_vars.her_doc != -1)
	{
		if (g_vars.her_doc == 0)
			exit(1);
	}
	printf("Quit: 3\n");
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

int	main(int ac, char **av, char **env)
{
	char		*line;
	t_list		*lines;
	t_main_args	args;

	lines = NULL;
	setup_g_vars(ac, av, env, &args);
	init_signals();
	while (1337)
	{
		g_vars.line_handled = 0;
		line = get_line();
		if (line == NULL)
		{
			write(2, "exit\n", 5);
			if (g_vars.last_exit_sat == 1)
				exit(g_vars.last_exit_sat);
			exit(WEXITSTATUS(g_vars.last_exit_sat));
		}
		if (*line != 0)
			add_history(line);
		g_vars.line_handled = 1;
		handle_line(line, &args);
		free(line);
	}
	return (0);
}
