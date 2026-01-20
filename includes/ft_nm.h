#ifndef FT_NM_H
# define FT_NM_H

# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/mman.h>
# include <sys/stat.h>
# include <unistd.h>
# include <stdint.h>

# include "macho.h"

/* ELF Magic */
# define ELFMAG0 0x7f
# define ELFMAG1 'E'
# define ELFMAG2 'L'
# define ELFMAG3 'F'

/* ELF Ident indexes */
# define EI_MAG0    0
# define EI_MAG1    1
# define EI_MAG2    2
# define EI_MAG3    3
# define EI_CLASS   4
# define EI_DATA    5
# define EI_NIDENT  16

/* ELF Class */
# define ELFCLASS32 1
# define ELFCLASS64 2

/* ELF Data encoding */
# define ELFDATA2LSB 1
# define ELFDATA2MSB 2

/* Section indexes */
# define SHN_UNDEF   0
# define SHN_ABS     0xfff1
# define SHN_COMMON  0xfff2

/* Section types */
# define SHT_NULL     0
# define SHT_PROGBITS 1
# define SHT_SYMTAB   2
# define SHT_STRTAB   3
# define SHT_RELA     4
# define SHT_HASH     5
# define SHT_DYNAMIC  6
# define SHT_NOTE     7
# define SHT_NOBITS   8
# define SHT_REL      9
# define SHT_DYNSYM   11
# define SHT_INIT_ARRAY    14
# define SHT_FINI_ARRAY    15
# define SHT_PREINIT_ARRAY 16

/* Section flags */
# define SHF_WRITE     0x1
# define SHF_ALLOC     0x2
# define SHF_EXECINSTR 0x4

/* Symbol binding */
# define STB_LOCAL  0
# define STB_GLOBAL 1
# define STB_WEAK   2
# define STB_GNU_UNIQUE 10

/* Symbol types */
# define STT_NOTYPE  0
# define STT_OBJECT  1
# define STT_FUNC    2
# define STT_SECTION 3
# define STT_FILE    4

# define ELF32_ST_BIND(i)   ((i) >> 4)
# define ELF32_ST_TYPE(i)   ((i) & 0xf)
# define ELF64_ST_BIND(i)   ((i) >> 4)
# define ELF64_ST_TYPE(i)   ((i) & 0xf)

/* ELF32 Header */
typedef struct {
	unsigned char	e_ident[EI_NIDENT];
	uint16_t		e_type;
	uint16_t		e_machine;
	uint32_t		e_version;
	uint32_t		e_entry;
	uint32_t		e_phoff;
	uint32_t		e_shoff;
	uint32_t		e_flags;
	uint16_t		e_ehsize;
	uint16_t		e_phentsize;
	uint16_t		e_phnum;
	uint16_t		e_shentsize;
	uint16_t		e_shnum;
	uint16_t		e_shstrndx;
}	Elf32_Ehdr;

/* ELF64 Header */
typedef struct {
	unsigned char	e_ident[EI_NIDENT];
	uint16_t		e_type;
	uint16_t		e_machine;
	uint32_t		e_version;
	uint64_t		e_entry;
	uint64_t		e_phoff;
	uint64_t		e_shoff;
	uint32_t		e_flags;
	uint16_t		e_ehsize;
	uint16_t		e_phentsize;
	uint16_t		e_phnum;
	uint16_t		e_shentsize;
	uint16_t		e_shnum;
	uint16_t		e_shstrndx;
}	Elf64_Ehdr;

/* ELF32 Section Header */
typedef struct {
	uint32_t	sh_name;
	uint32_t	sh_type;
	uint32_t	sh_flags;
	uint32_t	sh_addr;
	uint32_t	sh_offset;
	uint32_t	sh_size;
	uint32_t	sh_link;
	uint32_t	sh_info;
	uint32_t	sh_addralign;
	uint32_t	sh_entsize;
}	Elf32_Shdr;

/* ELF64 Section Header */
typedef struct {
	uint32_t	sh_name;
	uint32_t	sh_type;
	uint64_t	sh_flags;
	uint64_t	sh_addr;
	uint64_t	sh_offset;
	uint64_t	sh_size;
	uint32_t	sh_link;
	uint32_t	sh_info;
	uint64_t	sh_addralign;
	uint64_t	sh_entsize;
}	Elf64_Shdr;

/* ELF32 Symbol */
typedef struct {
	uint32_t	st_name;
	uint32_t	st_value;
	uint32_t	st_size;
	unsigned char	st_info;
	unsigned char	st_other;
	uint16_t	st_shndx;
}	Elf32_Sym;

/* ELF64 Symbol */
typedef struct {
	uint32_t	st_name;
	unsigned char	st_info;
	unsigned char	st_other;
	uint16_t	st_shndx;
	uint64_t	st_value;
	uint64_t	st_size;
}	Elf64_Sym;

# define OPT_A 0x01
# define OPT_G 0x02
# define OPT_U 0x04
# define OPT_R 0x08
# define OPT_P 0x10

# define FILE_ELF   1
# define FILE_MACHO 2

typedef struct s_symbol
{
	char			*name;
	unsigned long	value;
	char			type;
	unsigned char	bind;
	unsigned char	sym_type;
	uint16_t		shndx;
	int				index;
}	t_symbol;

typedef struct s_file
{
	const char		*filename;
	void			*data;
	size_t			size;
	int				is_64bit;
	int				is_little_endian;
	int				file_type;
}	t_file;

typedef struct s_nm
{
	t_file			file;
	t_symbol		*symbols;
	int				sym_count;
	int				options;
	int				multi_files;
}	t_nm;

typedef struct s_symtab32
{
	Elf32_Shdr	*symtab;
	Elf32_Shdr	*strtab;
}	t_symtab32;

typedef struct s_symtab64
{
	Elf64_Shdr	*symtab;
	Elf64_Shdr	*strtab;
}	t_symtab64;

typedef struct s_extract32
{
	t_file		*file;
	Elf32_Ehdr	*ehdr;
	Elf32_Shdr	*symtab;
	Elf32_Shdr	*strtab;
	Elf32_Sym	*syms;
	int			num_syms;
	int			i;
}	t_extract32;

typedef struct s_extract64
{
	t_file		*file;
	Elf64_Ehdr	*ehdr;
	Elf64_Shdr	*symtab;
	Elf64_Shdr	*strtab;
	Elf64_Sym	*syms;
	int			num_syms;
	int			i;
}	t_extract64;

typedef struct s_macho_ctx
{
	char	*sect_types;
	int		nsects;
	int		j;
}	t_macho_ctx;

int				map_file(t_file *file, const char *filename);
void			unmap_file(t_file *file);
int				validate_elf(t_file *file);
int				check_offset(t_file *file, size_t offset, size_t size);

int				parse_elf32(t_nm *nm);
Elf32_Shdr		*get_section_32(t_file *file, Elf32_Ehdr *ehdr, int idx);
char			get_symbol_type_32(Elf32_Sym *sym, Elf32_Shdr *shdr);
int				find_symtab_32(t_file *file, Elf32_Ehdr *ehdr, t_symtab32 *st);

int				parse_elf64(t_nm *nm);
Elf64_Shdr		*get_section_64(t_file *file, Elf64_Ehdr *ehdr, int idx);
char			get_symbol_type_64(Elf64_Sym *sym, Elf64_Shdr *shdr);
int				find_symtab_64(t_file *file, Elf64_Ehdr *ehdr, t_symtab64 *st);

int				filter_symbol(t_symbol *sym, int options);
void			print_symbols(t_nm *nm);
void			sort_symbols(t_nm *nm);

int				parse_options(int argc, char **argv, int *options);
void			print_usage(const char *prog_name);

int				ft_strcmp(const char *s1, const char *s2);
int				ft_strcmp_nocase(const char *s1, const char *s2);
void			*ft_memset(void *s, int c, size_t n);
void			print_error(const char *filename, const char *msg);

int				detect_file_type(t_file *file);
int				parse_macho64(t_nm *nm);
int				parse_macho32(t_nm *nm);
char			get_sect_type_macho(char *segname, char *sectname);
char			get_symbol_type_macho64(t_nlist_64 *sym, char *st, int ns);
t_symtab_command	*find_symtab_macho64(t_file *file);
int				count_sections_macho64(t_file *file);

#endif
