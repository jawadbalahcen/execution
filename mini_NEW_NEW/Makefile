NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror #-fsanitize=address -g3
RLFLAGS = -L/Users/jbalahce/.local/opt/homebrew/opt/readline/lib -lreadline
INCLUDES = -I./minishell.h -I/Users/jbalahce/.local/opt/homebrew/opt/readline/include
FILES = minishell.c \
	terminal_utills.c \
	get_next_line.c \
	string1.c \
	string2.c \
	string3.c \
	string4.c \
	string5.c \
	string6.c \
	linked_list.c \
	linked_list2.c \
	linked_list3.c \
	parsing.c \
	parsing1.c \
	qoutes.c \
	qoutes2.c \
	replace_env.c \
	replace_env_alpha.c \
	replace_env_helper.c \
	conv_to_cmd.c \
	deleters.c \
	conv_to_cmd_init.c \
	conv_to_cmd_checker.c \
	conv_to_cmd_geters.c \
	ft_atoi.c \

FILES_EXEC = execute.c execute1.c execute2.c execute3.c execute4.c execute5.c execute6.c \
	builtin_cmds.c builtin_cmds1.c builtin_cmds2.c builtin_cmds3.c builtin_cmds4.c builtin_cmds5.c \
	heredoc_stuff.c heredoc_stuff2.c heredoc_stuff_utills.c
	 
OFILES = $(FILES:.c=.o)
OFILES_EXEC = $(FILES_EXEC:.c=.o)

all : $(NAME)

$(NAME) : $(OFILES) $(OFILES_EXEC)
	$(CC) $(CFLAGS) $(RLFLAGS) $(OFILES) $(OFILES_EXEC) $(INCLUDES) -o $(NAME)

%.o : %.c
	$(CC) $(CFLAGS) -c $^ $(INCLUDES)

clean :
	rm -f $(OFILES) $(OBFILES) $(OFILES_EXEC)

fclean : clean
	rm -f $(NAME)

re : fclean all
