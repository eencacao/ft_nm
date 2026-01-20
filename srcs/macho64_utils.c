#include "ft_nm.h"
#include "macho.h"

char	get_sect_type_macho(char *segname, char *sectname)
{
	if (ft_strcmp(segname, "__TEXT") == 0)
	{
		if (ft_strcmp(sectname, "__text") == 0)
			return ('T');
		return ('T');
	}
	if (ft_strcmp(segname, "__DATA") == 0)
	{
		if (ft_strcmp(sectname, "__bss") == 0)
			return ('B');
		return ('D');
	}
	if (ft_strcmp(segname, "__BSS") == 0)
		return ('B');
	return ('S');
}

char	get_symbol_type_macho64(t_nlist_64 *sym, char *sect_types, int nsects)
{
	uint8_t	type;
	char	c;

	type = sym->n_type & N_TYPE;
	if (type == N_UNDF)
	{
		if (sym->n_value != 0)
			return ('C');
		return ('U');
	}
	if (type == N_ABS)
		c = 'A';
	else if (type == N_SECT && sym->n_sect > 0 && sym->n_sect <= nsects)
		c = sect_types[sym->n_sect - 1];
	else if (type == N_INDR)
		c = 'I';
	else
		c = '?';
	if (!(sym->n_type & N_EXT) && c != 'U' && c != '?')
		c += 32;
	return (c);
}

t_symtab_command	*find_symtab_macho64(t_file *file)
{
	t_mach_header_64	*hdr;
	t_load_command		*lc;
	size_t				off;
	uint32_t			i;

	hdr = (t_mach_header_64 *)file->data;
	off = sizeof(t_mach_header_64);
	i = 0;
	while (i < hdr->ncmds)
	{
		if (check_offset(file, off, sizeof(t_load_command)) < 0)
			return (NULL);
		lc = (t_load_command *)((char *)file->data + off);
		if (lc->cmd == LC_SYMTAB)
			return ((t_symtab_command *)lc);
		off += lc->cmdsize;
		i++;
	}
	return (NULL);
}

int	count_sections_macho64(t_file *file)
{
	t_mach_header_64		*hdr;
	t_load_command			*lc;
	t_segment_command_64	*seg;
	size_t					off;
	uint32_t				i;
	int						nsects;

	hdr = (t_mach_header_64 *)file->data;
	nsects = 0;
	off = sizeof(t_mach_header_64);
	i = 0;
	while (i < hdr->ncmds)
	{
		if (check_offset(file, off, sizeof(t_load_command)) < 0)
			return (-1);
		lc = (t_load_command *)((char *)file->data + off);
		if (lc->cmd == LC_SEGMENT_64)
		{
			seg = (t_segment_command_64 *)lc;
			nsects += seg->nsects;
		}
		off += lc->cmdsize;
		i++;
	}
	return (nsects);
}
