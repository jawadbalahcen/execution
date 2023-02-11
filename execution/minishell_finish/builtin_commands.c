/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbalahce <jbalahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 23:21:17 by jbalahce          #+#    #+#             */
/*   Updated: 2023/02/11 17:19:52 by jbalahce         ###   ########.fr       */
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
		write(2, "No such file or directory\n", 26);
		return (1);
	}
	else
	{
		cwd = getcwd(NULL, 0);
		if (!cwd)
		{
			perror("cd: error retrieving current directory: getcwd: cannot access parent directories");
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
		while ((cmd->param)[i] && check_flag((cmd->param)[i]))
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
	printf("%s\n", cwd);
	free(cwd);
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

int	my_export(t_cmd *cmd, t_main_args *main_args)
{
	static int	i;
	static int	ret;
	char		*param;

	if (!i)
		ret = 0;
	if (main_args->ac > 1)
	{
		param = ft_strdup((cmd->param)[1 + i]);
		if (check_valid(param))
			ret = 1;
		if (!ret)
		{
			if (env_exist(main_args, param))
				ft_lstadd_back(&(main_args->env_lst), ft_lstnew(param));
		}
		else
			free(param);
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

int	my_exit(t_cmd *cmd, t_main_args *main_args)
{
	int	i;

	i = 0;
	write(2, "exit\n", 5);
	if (main_args->ac != 1)
	{
		if (cmd->param[1][i] == '-' || cmd->param[1][i] == '+')
			i++;
		while (ft_isdigit(cmd->param[1][i]))
			i++;
		if (cmd->param[1][i])
		{
			write(2, "numeric argument required\n", 26);
			exit(255);
		}
	}
	if (main_args->ac == 2)
		exit(ft_atoi(cmd->param[1]));
	else if (main_args->ac > 2)
	{
		write(2, "exit: too many arguments\n", 25);
		return (1);
	}
	else
	{
		if (vars.last_exit_sat == 1)
			exit(vars.last_exit_sat);
		exit(WEXITSTATUS(vars.last_exit_sat));
	}
	return (0);
}
