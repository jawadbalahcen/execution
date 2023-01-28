/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbalahce <jbalahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 11:57:56 by monabid           #+#    #+#             */
/*   Updated: 2023/01/28 11:28:18 by jbalahce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;
typedef struct s_pids
{
	int				pid;
	struct s_pids	*next;
}					t_pids;
typedef struct s_range
{
	char			*str;
	int				type;
	int				next_is_space;
}					t_range;

typedef struct s_cmd
{
	char			*cmd;
	char			**param;
	struct s_cmd	*next;
}					t_cmd;
typedef struct s_main_args
{
	int				ac;
	char			**av;
	char			**env;
}					t_main_args;

typedef struct s_vars
{
	t_main_args		args;
	pid_t			curent_pid;
	int				last_exit_sat;
}					t_vars;

t_vars				vars;

// strings
size_t				ft_strlen(const char *s);
char				*ft_strjoin(char const *s1, char const *s2);
void				*ft_calloc(size_t count, size_t size);
char				*ft_strchr(const char *s, int c);
size_t				ft_strlcpy(char *dst, const char *src, size_t dstsize);
char				*ft_substr(char const *s, unsigned int start, size_t len);
char				**ft_split(char const *s, char c);
void				ft_striteri(char *s, void (*f)(unsigned int, char *));
int					ft_strncmp(const char *s1, const char *s2, size_t n);
int					ft_strncmp1(const char *s1, const char *s2, size_t n);
char				*get_next_line(int fd);
//string4
char				*ft_strtrim(char const *s1, char const *set);
char				*ft_strdup(const char *s1);
int					ft_strcmp(const char *s1, const char *s2);
//string5

int					ft_isdigit(int c);
int					ft_isalpha(int c);
int					ft_isalnum(int c);
char				*ft_itoa(int n);
//linked list
t_list				*ft_lstnew(void *content);
void				ft_lstclear(t_list **lst, void (*del)(void *));
void				ft_lstadd_back(t_list **lst, t_list *new);
t_list				*ft_lstlast(t_list *lst);
//read line
void				handle_line(char *line);
//step 1
char				check_is_symbol(char s);
int					is_space(char s);
t_list				*qoutes_handling(char *line);
void				replace_env(t_list **lst);
t_cmd				*ft_lstlast2(t_cmd *lst);
void				ft_lstadd_back2(t_cmd **lst, t_cmd *new);
void				ft_lstclear2(t_cmd **lst, void (*del)(void *));
t_cmd				*ft_lstnew2(char *cmd, char **param);

//builtin_commands
void				my_cd(t_cmd *cmd, t_main_args *main_args);
void				my_echo(t_cmd *cmd, t_main_args *main_args);
void				my_pwd(t_main_args *main_args);
void				my_env(t_main_args *main_args);
void				my_export(t_cmd *cmd, t_main_args *main_args);
void				my_unset(t_cmd *cmd, t_main_args *main_args);
void				my_exit(void);
// linked_list_pids
t_pids				*ft_lstnew3(int pid);
void				ft_lstclear3(t_pids **lst, void (*del)(void *));
void				ft_lstadd_back3(t_pids **lst, t_pids *new);
t_pids				*ft_lstlast3(t_pids *lst);

//execute
void				execute(t_cmd *cmd, t_main_args *main_args);
char				*bring_path(t_main_args *main_args, char *cmd);
void				handle_builtins(t_cmd *cmd, t_main_args *main_args,
						int *built);
char				*my_get_env(char *var, t_main_args *main_args);
//conv to cmd
t_cmd				*conv_to_cmd(t_list **lst);

#endif
