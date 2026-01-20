#include "ft_nm.h"

int	filter_symbol(t_symbol *sym, int options)
{
	if (options & OPT_G)
	{
		if (sym->bind != STB_GLOBAL && sym->bind != STB_WEAK)
			return (0);
	}
	if (options & OPT_U)
	{
		if (sym->type != 'U' && sym->type != 'w')
			return (0);
	}
	return (1);
}

static void	print_symbol_32(t_symbol *sym, int options)
{
	if (options & OPT_U)
		printf("%s\n", sym->name);
	else if (sym->type == 'U' || sym->type == 'w')
		printf("         %c %s\n", sym->type, sym->name);
	else
		printf("%08lx %c %s\n", sym->value, sym->type, sym->name);
}

static void	print_symbol_64(t_symbol *sym, int options)
{
	if (options & OPT_U)
		printf("%s\n", sym->name);
	else if (sym->type == 'U' || sym->type == 'w')
		printf("                 %c %s\n", sym->type, sym->name);
	else
		printf("%016lx %c %s\n", sym->value, sym->type, sym->name);
}

void	print_symbols(t_nm *nm)
{
	int	i;

	if (nm->multi_files)
		printf("\n%s:\n", nm->file.filename);
	i = 0;
	while (i < nm->sym_count)
	{
		if (!filter_symbol(&nm->symbols[i], nm->options))
		{
			i++;
			continue ;
		}
		if (nm->file.is_64bit)
			print_symbol_64(&nm->symbols[i], nm->options);
		else
			print_symbol_32(&nm->symbols[i], nm->options);
		i++;
	}
}
