#include "ft_nm.h"
#include "macho.h"

t_symtab_command	*find_symtab_macho32(t_file *file);
int					count_sections_macho32(t_file *file);
char				get_symbol_type_macho32(t_nlist *sym, char *st, int ns);
int					fill_sect_types_macho32(t_file *file, char *sect_types);

static int	skip_symbol_macho32(t_nlist *sym, int options)
{
	if (sym->n_type & N_STAB)
		return (!(options & OPT_A));
	return (0);
}

static void	fill_symbol_macho32(t_symbol *dst, t_nlist *src, char *strtab,
	t_macho_ctx *ctx)
{
	dst->name = strtab + src->n_strx;
	dst->value = src->n_value;
	dst->type = get_symbol_type_macho32(src, ctx->sect_types, ctx->nsects);
	dst->bind = (src->n_type & N_EXT) ? STB_GLOBAL : STB_LOCAL;
	dst->sym_type = 0;
	dst->shndx = src->n_sect;
	dst->index = ctx->j;
}

static int	init_symtab_macho32(t_nm *nm, t_symtab_command *sc)
{
	if (check_offset(&nm->file, sc->symoff, sc->nsyms * sizeof(t_nlist)) < 0)
		return (-1);
	if (check_offset(&nm->file, sc->stroff, sc->strsize) < 0)
		return (-1);
	nm->symbols = malloc(sizeof(t_symbol) * sc->nsyms);
	if (!nm->symbols)
		return (-1);
	return (0);
}

static int	extract_symbols_macho32(t_nm *nm, t_symtab_command *sc,
	t_macho_ctx *ctx)
{
	t_nlist		*syms;
	char		*strtab;
	uint32_t	i;

	if (init_symtab_macho32(nm, sc) < 0)
		return (-1);
	syms = (t_nlist *)((char *)nm->file.data + sc->symoff);
	strtab = (char *)nm->file.data + sc->stroff;
	ctx->j = 0;
	i = -1;
	while (++i < sc->nsyms)
	{
		if (syms[i].n_strx >= sc->strsize)
			continue ;
		if (skip_symbol_macho32(&syms[i], nm->options))
			continue ;
		fill_symbol_macho32(&nm->symbols[ctx->j], &syms[i], strtab, ctx);
		ctx->j++;
	}
	nm->sym_count = ctx->j;
	return (0);
}

int	parse_macho32(t_nm *nm)
{
	t_symtab_command	*sc;
	t_macho_ctx			ctx;

	if (check_offset(&nm->file, 0, sizeof(t_mach_header)) < 0)
		return (print_error(nm->file.filename, "file truncated"), -1);
	sc = find_symtab_macho32(&nm->file);
	if (!sc)
		return (print_error(nm->file.filename, "no symbols"), -1);
	ctx.nsects = count_sections_macho32(&nm->file);
	if (ctx.nsects < 0)
		return (-1);
	ctx.sect_types = malloc(ctx.nsects + 1);
	if (!ctx.sect_types)
		return (-1);
	if (fill_sect_types_macho32(&nm->file, ctx.sect_types) < 0)
		return (free(ctx.sect_types), -1);
	if (extract_symbols_macho32(nm, sc, &ctx) < 0)
		return (free(ctx.sect_types), -1);
	free(ctx.sect_types);
	return (0);
}
