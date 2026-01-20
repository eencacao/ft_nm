#include "ft_nm.h"
#include "macho.h"

int		fill_sect_types_macho64(t_file *file, char *sect_types);
int		skip_symbol_macho64(t_nlist_64 *sym, int options);
void	fill_symbol_macho64(t_symbol *dst, t_nlist_64 *src, char *strtab,
			t_macho_ctx *ctx);

static int	init_symtab_macho64(t_nm *nm, t_symtab_command *sc)
{
	if (check_offset(&nm->file, sc->symoff, sc->nsyms * sizeof(t_nlist_64)) < 0)
		return (-1);
	if (check_offset(&nm->file, sc->stroff, sc->strsize) < 0)
		return (-1);
	nm->symbols = malloc(sizeof(t_symbol) * sc->nsyms);
	if (!nm->symbols)
		return (-1);
	return (0);
}

static int	extract_symbols_macho64(t_nm *nm, t_symtab_command *sc,
	t_macho_ctx *ctx)
{
	t_nlist_64	*syms;
	char		*strtab;
	uint32_t	i;

	if (init_symtab_macho64(nm, sc) < 0)
		return (-1);
	syms = (t_nlist_64 *)((char *)nm->file.data + sc->symoff);
	strtab = (char *)nm->file.data + sc->stroff;
	ctx->j = 0;
	i = -1;
	while (++i < sc->nsyms)
	{
		if (syms[i].n_strx >= sc->strsize)
			continue ;
		if (skip_symbol_macho64(&syms[i], nm->options))
			continue ;
		fill_symbol_macho64(&nm->symbols[ctx->j], &syms[i], strtab, ctx);
		ctx->j++;
	}
	nm->sym_count = ctx->j;
	return (0);
}

int	parse_macho64(t_nm *nm)
{
	t_symtab_command	*sc;
	t_macho_ctx			ctx;

	if (check_offset(&nm->file, 0, sizeof(t_mach_header_64)) < 0)
		return (print_error(nm->file.filename, "file truncated"), -1);
	sc = find_symtab_macho64(&nm->file);
	if (!sc)
		return (print_error(nm->file.filename, "no symbols"), -1);
	ctx.nsects = count_sections_macho64(&nm->file);
	if (ctx.nsects < 0)
		return (-1);
	ctx.sect_types = malloc(ctx.nsects + 1);
	if (!ctx.sect_types)
		return (-1);
	if (fill_sect_types_macho64(&nm->file, ctx.sect_types) < 0)
		return (free(ctx.sect_types), -1);
	if (extract_symbols_macho64(nm, sc, &ctx) < 0)
		return (free(ctx.sect_types), -1);
	free(ctx.sect_types);
	return (0);
}
