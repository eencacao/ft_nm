#include "ft_nm.h"
#include "macho.h"

static int	fill_segment_types_64(t_segment_command_64 *seg, char *sect_types,
	int idx)
{
	t_section_64	*sect;
	uint32_t		j;

	sect = (t_section_64 *)((char *)seg + sizeof(*seg));
	j = -1;
	while (++j < seg->nsects)
		sect_types[idx++] = get_sect_type_macho(sect[j].segname,
			sect[j].sectname);
	return (idx);
}

int	fill_sect_types_macho64(t_file *file, char *sect_types)
{
	t_mach_header_64	*hdr;
	t_load_command		*lc;
	size_t				off;
	uint32_t			i;
	int					idx;

	hdr = (t_mach_header_64 *)file->data;
	idx = 0;
	off = sizeof(t_mach_header_64);
	i = -1;
	while (++i < hdr->ncmds)
	{
		lc = (t_load_command *)((char *)file->data + off);
		if (lc->cmd == LC_SEGMENT_64)
			idx = fill_segment_types_64((t_segment_command_64 *)lc,
				sect_types, idx);
		off += lc->cmdsize;
	}
	return (0);
}

void	fill_symbol_macho64(t_symbol *dst, t_nlist_64 *src, char *strtab,
	t_macho_ctx *ctx)
{
	dst->name = strtab + src->n_strx;
	dst->value = src->n_value;
	dst->type = get_symbol_type_macho64(src, ctx->sect_types, ctx->nsects);
	dst->bind = (src->n_type & N_EXT) ? STB_GLOBAL : STB_LOCAL;
	dst->sym_type = 0;
	dst->shndx = src->n_sect;
	dst->index = ctx->j;
}
