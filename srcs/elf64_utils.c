#include "ft_nm.h"

static char	get_section_type_64(Elf64_Shdr *shdr)
{
	if (shdr->sh_type == SHT_NOBITS && (shdr->sh_flags & SHF_ALLOC))
		return ('B');
	if (shdr->sh_type == SHT_PROGBITS && (shdr->sh_flags & SHF_EXECINSTR))
		return ('T');
	if (shdr->sh_type == SHT_PROGBITS && (shdr->sh_flags & SHF_WRITE))
		return ('D');
	if (shdr->sh_type == SHT_PROGBITS || shdr->sh_type == SHT_NOTE)
		return ('R');
	if (shdr->sh_type == SHT_DYNAMIC)
		return ('D');
	if (shdr->sh_type == SHT_INIT_ARRAY || shdr->sh_type == SHT_FINI_ARRAY
		|| shdr->sh_type == SHT_PREINIT_ARRAY)
		return ('D');
	if (shdr->sh_flags & SHF_ALLOC)
	{
		if (shdr->sh_flags & SHF_WRITE)
			return ('D');
		return ('R');
	}
	return ('N');
}

static char	apply_binding_64(Elf64_Sym *sym, char c)
{
	if (ELF64_ST_BIND(sym->st_info) == STB_LOCAL && c != 'U' && c != '?')
		c += 32;
	if (ELF64_ST_BIND(sym->st_info) == STB_WEAK)
	{
		if (sym->st_shndx == SHN_UNDEF)
			return ('w');
		if (ELF64_ST_TYPE(sym->st_info) == STT_OBJECT)
			return ('V');
		return ('W');
	}
	if (ELF64_ST_BIND(sym->st_info) == STB_GNU_UNIQUE)
		return ('u');
	return (c);
}

char	get_symbol_type_64(Elf64_Sym *sym, Elf64_Shdr *shdr)
{
	char	c;

	if (sym->st_shndx == SHN_UNDEF)
		return ('U');
	if (ELF64_ST_TYPE(sym->st_info) == STT_GNU_IFUNC)
	{
		if (ELF64_ST_BIND(sym->st_info) == STB_LOCAL)
			return ('i');
		return ('I');
	}
	if (sym->st_shndx == SHN_ABS)
		c = 'A';
	else if (sym->st_shndx == SHN_COMMON)
		c = 'C';
	else if (!shdr)
		return ('?');
	else
		c = get_section_type_64(shdr);
	return (apply_binding_64(sym, c));
}

Elf64_Shdr	*get_section_64(t_file *file, Elf64_Ehdr *ehdr, int idx)
{
	size_t	offset;

	if (idx < 0 || idx >= ehdr->e_shnum)
		return (NULL);
	offset = ehdr->e_shoff + idx * ehdr->e_shentsize;
	if (check_offset(file, offset, sizeof(Elf64_Shdr)) < 0)
		return (NULL);
	return ((Elf64_Shdr *)((char *)file->data + offset));
}

int	find_symtab_64(t_file *file, Elf64_Ehdr *ehdr, t_symtab64 *st)
{
	Elf64_Shdr	*shdr;
	int			i;

	st->symtab = NULL;
	st->strtab = NULL;
	i = -1;
	while (++i < ehdr->e_shnum)
	{
		shdr = get_section_64(file, ehdr, i);
		if (!shdr)
			return (-1);
		if (shdr->sh_type == SHT_SYMTAB)
		{
			st->symtab = shdr;
			st->strtab = get_section_64(file, ehdr, shdr->sh_link);
			break ;
		}
	}
	if (!st->symtab || !st->strtab)
		return (print_error(file->filename, "no symbols"), -1);
	return (0);
}
