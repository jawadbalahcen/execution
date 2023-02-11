#include "minishell.h"

int	main(int argc, char **argv)
{
	char	*s;
	s = getcwd(NULL, 0);
	system("leaks a.out");
	return (0);
}
