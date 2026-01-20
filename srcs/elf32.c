#include "ft_nm.h"

static int	skip_symbol_32(Elf32_Sym *sym, int options, int idx)
{
	if (idx == 0)
		return (1);
	if (ELF32_ST_TYPE(sym->st_info) == STT_FILE)
		return (!(options & OPT_A));
	if (ELF32_ST_TYPE(sym->st_info) == STT_SECTION)
		return (!(options & OPT_A));
	return (0);
}

static void	fill_symbol_32(t_symbol *dst, t_extract32 *e, int j)
{
	Elf32_Shdr	*shdr;

	shdr = NULL;
	if (e->syms[e->i].st_shndx < e->ehdr->e_shnum)
		shdr = get_section_32(e->file, e->ehdr, e->syms[e->i].st_shndx);
	dst->name = (char *)e->file->data + e->strtab->sh_offset
		+ e->syms[e->i].st_name;
	dst->value = e->syms[e->i].st_value;
	dst->bind = ELF32_ST_BIND(e->syms[e->i].st_info);
	dst->sym_type = ELF32_ST_TYPE(e->syms[e->i].st_info);
	dst->shndx = e->syms[e->i].st_shndx;
	dst->type = get_symbol_type_32(&e->syms[e->i], shdr);
	dst->index = j;
}

static int	extract_symbols_32(t_nm *nm, t_extract32 *e)
{
	int	j;

	nm->symbols = malloc(sizeof(t_symbol) * e->num_syms);
	if (!nm->symbols)
		return (-1);
	j = 0;
	e->i = -1;
	while (++e->i < e->num_syms)
	{
		if (e->syms[e->i].st_name >= e->strtab->sh_size)
			continue ;
		if (skip_symbol_32(&e->syms[e->i], nm->options, e->i))
			continue ;
		fill_symbol_32(&nm->symbols[j], e, j);
		j++;
	}
	nm->sym_count = j;
	return (0);
}

static int	init_extract_32(t_nm *nm, t_extract32 *e, t_symtab32 *st)
{
	e->file = &nm->file;
	e->ehdr = (Elf32_Ehdr *)nm->file.data;
	e->symtab = st->symtab;
	e->strtab = st->strtab;
	if (check_offset(e->file, e->symtab->sh_offset, e->symtab->sh_size) < 0)
		return (-1);
	if (check_offset(e->file, e->strtab->sh_offset, e->strtab->sh_size) < 0)
		return (-1);
	e->syms = (Elf32_Sym *)((char *)e->file->data + e->symtab->sh_offset);
	e->num_syms = e->symtab->sh_size / sizeof(Elf32_Sym);
	return (0);
}

int	parse_elf32(t_nm *nm)
{
	Elf32_Ehdr	*ehdr;
	t_symtab32	st;
	t_extract32	e;

	if (check_offset(&nm->file, 0, sizeof(Elf32_Ehdr)) < 0)
		return (print_error(nm->file.filename, "file truncated"), -1);
	ehdr = (Elf32_Ehdr *)nm->file.data;
	if (check_offset(&nm->file, ehdr->e_shoff,
			ehdr->e_shnum * ehdr->e_shentsize) < 0)
		return (print_error(nm->file.filename, "file truncated"), -1);
	if (find_symtab_32(&nm->file, ehdr, &st) < 0)
		return (-1);
	if (init_extract_32(nm, &e, &st) < 0)
		return (-1);
	return (extract_symbols_32(nm, &e));
}
