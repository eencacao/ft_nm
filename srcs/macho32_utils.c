#include "ft_nm.h"
#include "macho.h"

t_symtab_command	*find_symtab_macho32(t_file *file)
{
	t_mach_header	*hdr;
	t_load_command	*lc;
	size_t			off;
	uint32_t		i;

	hdr = (t_mach_header *)file->data;
	off = sizeof(t_mach_header);
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

int	count_sections_macho32(t_file *file)
{
	t_mach_header		*hdr;
	t_load_command		*lc;
	t_segment_command	*seg;
	size_t				off;
	uint32_t			i;
	int					nsects;

	hdr = (t_mach_header *)file->data;
	nsects = 0;
	off = sizeof(t_mach_header);
	i = 0;
	while (i < hdr->ncmds)
	{
		if (check_offset(file, off, sizeof(t_load_command)) < 0)
			return (-1);
		lc = (t_load_command *)((char *)file->data + off);
		if (lc->cmd == LC_SEGMENT)
		{
			seg = (t_segment_command *)lc;
			nsects += seg->nsects;
		}
		off += lc->cmdsize;
		i++;
	}
	return (nsects);
}

char	get_symbol_type_macho32(t_nlist *sym, char *sect_types, int nsects)
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

static int	fill_segment_types_32(t_segment_command *seg, char *sect_types,
	int idx)
{
	t_section	*sect;
	uint32_t	j;

	sect = (t_section *)((char *)seg + sizeof(*seg));
	j = -1;
	while (++j < seg->nsects)
		sect_types[idx++] = get_sect_type_macho(sect[j].segname,
			sect[j].sectname);
	return (idx);
}

int	fill_sect_types_macho32(t_file *file, char *sect_types)
{
	t_mach_header		*hdr;
	t_load_command		*lc;
	size_t				off;
	uint32_t			i;
	int					idx;

	hdr = (t_mach_header *)file->data;
	idx = 0;
	off = sizeof(t_mach_header);
	i = -1;
	while (++i < hdr->ncmds)
	{
		lc = (t_load_command *)((char *)file->data + off);
		if (lc->cmd == LC_SEGMENT)
			idx = fill_segment_types_32((t_segment_command *)lc,
				sect_types, idx);
		off += lc->cmdsize;
	}
	return (0);
}
