#include "ft_nm.h"

void	print_usage(const char *prog_name)
{
	fprintf(stderr, "Usage: %s [-agurp] [file...]\n", prog_name);
}

static int	parse_opt_char(char c, int *options, const char *prog_name)
{
	if (c == 'a')
		*options |= OPT_A;
	else if (c == 'g')
		*options |= OPT_G;
	else if (c == 'u')
		*options |= OPT_U;
	else if (c == 'r')
		*options |= OPT_R;
	else if (c == 'p')
		*options |= OPT_P;
	else
	{
		fprintf(stderr, "%s: invalid option -- '%c'\n", prog_name, c);
		print_usage(prog_name);
		return (-1);
	}
	return (0);
}

int	parse_options(int argc, char **argv, int *options)
{
	int	i;
	int	j;

	*options = 0;
	i = 1;
	while (i < argc && argv[i][0] == '-' && argv[i][1] != '\0')
	{
		if (argv[i][1] == '-' && argv[i][2] == '\0')
			return (i + 1);
		j = 1;
		while (argv[i][j])
		{
			if (parse_opt_char(argv[i][j], options, argv[0]) < 0)
				return (-1);
			j++;
		}
		i++;
	}
	return (i);
}
