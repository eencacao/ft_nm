#include "ft_nm.h"

int	map_file(t_file *file, const char *filename)
{
	int			fd;
	struct stat	st;

	file->filename = filename;
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (print_error(filename, "No such file"), -1);
	if (fstat(fd, &st) < 0)
		return (close(fd), print_error(filename, "fstat failed"), -1);
	if (S_ISDIR(st.st_mode))
		return (close(fd), print_error(filename, "Is a directory"), -1);
	if (st.st_size == 0)
		return (close(fd), print_error(filename, "file format not recognized"),
			-1);
	file->size = st.st_size;
	file->data = mmap(NULL, file->size, PROT_READ, MAP_PRIVATE, fd, 0);
	close(fd);
	if (file->data == MAP_FAILED)
		return (print_error(filename, "mmap failed"), -1);
	return (0);
}

void	unmap_file(t_file *file)
{
	if (file->data && file->data != MAP_FAILED)
		munmap(file->data, file->size);
	file->data = NULL;
}

int	check_offset(t_file *file, size_t offset, size_t size)
{
	if (offset > file->size || size > file->size)
		return (-1);
	if (offset + size > file->size)
		return (-1);
	return (0);
}

int	validate_elf(t_file *file)
{
	unsigned char	*ident;

	if (file->size < EI_NIDENT)
		return (print_error(file->filename, "file format not recognized"), -1);
	ident = (unsigned char *)file->data;
	if (ident[EI_MAG0] != ELFMAG0 || ident[EI_MAG1] != ELFMAG1
		|| ident[EI_MAG2] != ELFMAG2 || ident[EI_MAG3] != ELFMAG3)
		return (print_error(file->filename, "file format not recognized"), -1);
	if (ident[EI_CLASS] != ELFCLASS32 && ident[EI_CLASS] != ELFCLASS64)
		return (print_error(file->filename, "invalid ELF class"), -1);
	file->is_64bit = (ident[EI_CLASS] == ELFCLASS64);
	file->is_little_endian = (ident[EI_DATA] == ELFDATA2LSB);
	return (0);
}
