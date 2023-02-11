/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbalahce <jbalahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 11:57:56 by monabid           #+#    #+#             */
/*   Updated: 2023/02/11 14:43:28 by jbalahce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;

typedef struct s_fds
{
	int				fdin;
	int				fdout;
	int				pid;
}					t_fds;
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

typedef struct s_io
{
	char			*file;
	char			*type;
}					t_io;

typedef struct s_in_out
{
	t_list			*input;
	t_list			*output;
	t_list			*errfile;
}					t_in_out;

typedef struct s_cmd
{
	char			*cmd;
	char			**param;
	t_in_out		*fles;
	char			*token;
	struct s_cmd	*next;
}					t_cmd;

typedef struct s_expend
{
	char			*start;
	t_list			*match;
	char			*dir;
}					t_expend;

typedef struct s_main_args
{
	int				ac;
	char			**av;
	char			**env;
	t_list			*env_lst;
}					t_main_args;

typedef struct s_vars
{
	t_main_args		*args;
	int				last_exit_sat;
}					t_vars;

typedef struct s_help
{
	int				params;
	int				infiles;
	int				outfiles;
	int				nodes;
}					t_help;

t_vars				vars;

typedef int			(*t_f)(t_cmd *cmd, t_main_args *main_args);

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
int					ft_strl9lawi(char *s1, char *s2);
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
int					ft_lstsize(t_list *lst);
//read line
void				handle_line(char *line , t_main_args *args);
//step 1
char				check_is_symbol(char s);
int					is_space(char s);
t_list				*qoutes_handling(char *line);
//step 2
void				replace_env(t_list **lst);
char				*group_strs(char *start, char *var_vale, char *end);
int					replace_as_alpha(char **str, int i, int type);
//link2
t_cmd				*ft_lstlast2(t_cmd *lst);
void				ft_lstadd_back2(t_cmd **lst, t_cmd *new);
void				ft_lstclear2(t_cmd **lst, void (*del)(void *));
t_cmd				*ft_lstnew2(char *cmd, char **param);
void				free_arr(t_cmd **arr, void (*del)(void *));

//builtin_commands
int					my_cd(t_cmd *cmd, t_main_args *main_args);
int					my_echo(t_cmd *cmd, t_main_args *main_args);
int					my_pwd(t_cmd *cmd, t_main_args *main_args);
int					my_env(t_cmd *cmd, t_main_args *main_args);
int					my_export(t_cmd *cmd, t_main_args *main_args);
int					my_unset(t_cmd *cmd, t_main_args *main_args);
int					my_exit(t_cmd *cmd, t_main_args *main_args);
// linked_list_pids
t_pids				*ft_lstnew3(int pid);
void				ft_lstclear3(t_pids **lst, void (*del)(void *));
void				ft_lstadd_back3(t_pids **lst, t_pids *new);
t_pids				*ft_lstlast3(t_pids *lst);

//execute
void				execute(t_cmd *cmd, t_main_args *main_args, int num_cmds);
char				*bring_path(t_main_args *main_args, char *cmd);
void				handle_builtins(t_cmd *cmd, int *built);
char				*my_get_env(char *var, t_main_args *main_args);
void				print_error(char *s);
int					size_cmd(t_cmd *cmd);
char				**mini_split(char *var);

//conv to cmd
t_cmd				*conv_to_cmd(t_list **lst);
//inits
void				init_help(t_help *h);
char				**init_param(int count);
void				count_until_token(t_help *h, t_list *lst);
//checker
int					check_tokens(t_list *lst);
//value geter
t_list				*get_output_files(int count, t_list *lst);
t_list				*get_input_files(int count, t_list *lst);
char				**get_params(int count, t_list *lst);
//deleters
void				free_arr(t_cmd **arr, void (*del)(void *));
void				free_io(void *ios);
void				free_range_arr(void *rang);
void				del_range(void *range);
//ft_atoi
int					ft_atoi(const char *str);
#endif
