#include "ft_nm.h"
#include "macho.h"

int	detect_file_type(t_file *file)
{
	uint32_t		magic;
	unsigned char	*ident;

	if (file->size < 4)
		return (0);
	magic = *(uint32_t *)file->data;
	if (magic == MH_MAGIC_64 || magic == MH_CIGAM_64)
	{
		file->is_64bit = 1;
		file->is_little_endian = (magic == MH_MAGIC_64);
		return (FILE_MACHO);
	}
	if (magic == MH_MAGIC || magic == MH_CIGAM)
	{
		file->is_64bit = 0;
		file->is_little_endian = (magic == MH_MAGIC);
		return (FILE_MACHO);
	}
	if (file->size < EI_NIDENT)
		return (0);
	ident = (unsigned char *)file->data;
	if (ident[0] == ELFMAG0 && ident[1] == ELFMAG1
		&& ident[2] == ELFMAG2 && ident[3] == ELFMAG3)
	{
		file->is_64bit = (ident[EI_CLASS] == ELFCLASS64);
		file->is_little_endian = (ident[EI_DATA] == ELFDATA2LSB);
		return (FILE_ELF);
	}
	return (0);
}
