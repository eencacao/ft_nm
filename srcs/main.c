#include "ft_nm.h"

static int	parse_file(t_nm *nm)
{
	nm->file.file_type = detect_file_type(&nm->file);
	if (nm->file.file_type == FILE_ELF)
	{
		if (nm->file.is_64bit)
			return (parse_elf64(nm));
		return (parse_elf32(nm));
	}
	if (nm->file.file_type == FILE_MACHO)
	{
		if (nm->file.is_64bit)
			return (parse_macho64(nm));
		return (parse_macho32(nm));
	}
	return (print_error(nm->file.filename, "file format not recognized"), -1);
}

static int	process_file(t_nm *nm, const char *filename)
{
	int	opts;

	opts = nm->options;
	ft_memset(nm, 0, sizeof(t_nm));
	nm->options = opts;
	if (map_file(&nm->file, filename) < 0)
		return (-1);
	if (parse_file(nm) < 0)
	{
		unmap_file(&nm->file);
		return (-1);
	}
	sort_symbols(nm);
	print_symbols(nm);
	free(nm->symbols);
	unmap_file(&nm->file);
	return (0);
}

static void	init_args(int *argc, char **argv, int first_file)
{
	if (first_file >= *argc)
	{
		argv[*argc] = "a.out";
		(*argc)++;
	}
}

int	main(int argc, char **argv)
{
	t_nm	nm;
	int		first_file;
	int		i;
	int		ret;

	ft_memset(&nm, 0, sizeof(t_nm));
	first_file = parse_options(argc, argv, &nm.options);
	if (first_file < 0)
		return (1);
	init_args(&argc, argv, first_file);
	nm.multi_files = (argc - first_file > 1);
	ret = 0;
	i = first_file;
	while (i < argc)
	{
		if (process_file(&nm, argv[i]) < 0)
			ret = 1;
		i++;
	}
	return (ret);
}
