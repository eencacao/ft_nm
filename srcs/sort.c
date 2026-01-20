#include "ft_nm.h"

static int	compare_symbols(t_symbol *a, t_symbol *b, int options)
{
	int	cmp;

	cmp = ft_strcmp_nocase(a->name, b->name);
	if (cmp == 0)
		cmp = ft_strcmp(a->name, b->name);
	if (cmp == 0)
		cmp = a->index - b->index;
	if (options & OPT_R)
		cmp = -cmp;
	return (cmp);
}

static void	swap_symbols(t_symbol *a, t_symbol *b)
{
	t_symbol	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	sort_symbols(t_nm *nm)
{
	int	i;
	int	j;
	int	min;

	if (nm->options & OPT_P)
		return ;
	i = 0;
	while (i < nm->sym_count - 1)
	{
		min = i;
		j = i + 1;
		while (j < nm->sym_count)
		{
			if (compare_symbols(&nm->symbols[j], &nm->symbols[min],
					nm->options) < 0)
				min = j;
			j++;
		}
		if (min != i)
			swap_symbols(&nm->symbols[i], &nm->symbols[min]);
		i++;
	}
}
