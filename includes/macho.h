#ifndef MACHO_H
# define MACHO_H

# include <stdint.h>

# define MH_MAGIC     0xfeedface
# define MH_CIGAM     0xcefaedfe
# define MH_MAGIC_64  0xfeedfacf
# define MH_CIGAM_64  0xcffaedfe
# define FAT_MAGIC    0xcafebabe
# define FAT_CIGAM    0xbebafeca

# define CPU_TYPE_I386    7
# define CPU_TYPE_X86_64  0x01000007
# define CPU_TYPE_ARM     12
# define CPU_TYPE_ARM64   0x0100000c

# define LC_SEGMENT       0x1
# define LC_SYMTAB        0x2
# define LC_SEGMENT_64    0x19

# define N_STAB  0xe0
# define N_PEXT  0x10
# define N_TYPE  0x0e
# define N_EXT   0x01

# define N_UNDF  0x0
# define N_ABS   0x2
# define N_SECT  0xe
# define N_PBUD  0xc
# define N_INDR  0xa

typedef struct s_mach_header
{
	uint32_t	magic;
	int32_t		cputype;
	int32_t		cpusubtype;
	uint32_t	filetype;
	uint32_t	ncmds;
	uint32_t	sizeofcmds;
	uint32_t	flags;
}	t_mach_header;

typedef struct s_mach_header_64
{
	uint32_t	magic;
	int32_t		cputype;
	int32_t		cpusubtype;
	uint32_t	filetype;
	uint32_t	ncmds;
	uint32_t	sizeofcmds;
	uint32_t	flags;
	uint32_t	reserved;
}	t_mach_header_64;

typedef struct s_load_command
{
	uint32_t	cmd;
	uint32_t	cmdsize;
}	t_load_command;

typedef struct s_symtab_command
{
	uint32_t	cmd;
	uint32_t	cmdsize;
	uint32_t	symoff;
	uint32_t	nsyms;
	uint32_t	stroff;
	uint32_t	strsize;
}	t_symtab_command;

typedef struct s_segment_command
{
	uint32_t	cmd;
	uint32_t	cmdsize;
	char		segname[16];
	uint32_t	vmaddr;
	uint32_t	vmsize;
	uint32_t	fileoff;
	uint32_t	filesize;
	int32_t		maxprot;
	int32_t		initprot;
	uint32_t	nsects;
	uint32_t	flags;
}	t_segment_command;

typedef struct s_segment_command_64
{
	uint32_t	cmd;
	uint32_t	cmdsize;
	char		segname[16];
	uint64_t	vmaddr;
	uint64_t	vmsize;
	uint64_t	fileoff;
	uint64_t	filesize;
	int32_t		maxprot;
	int32_t		initprot;
	uint32_t	nsects;
	uint32_t	flags;
}	t_segment_command_64;

typedef struct s_section
{
	char		sectname[16];
	char		segname[16];
	uint32_t	addr;
	uint32_t	size;
	uint32_t	offset;
	uint32_t	align;
	uint32_t	reloff;
	uint32_t	nreloc;
	uint32_t	flags;
	uint32_t	reserved1;
	uint32_t	reserved2;
}	t_section;

typedef struct s_section_64
{
	char		sectname[16];
	char		segname[16];
	uint64_t	addr;
	uint64_t	size;
	uint32_t	offset;
	uint32_t	align;
	uint32_t	reloff;
	uint32_t	nreloc;
	uint32_t	flags;
	uint32_t	reserved1;
	uint32_t	reserved2;
	uint32_t	reserved3;
}	t_section_64;

typedef struct s_nlist
{
	uint32_t	n_strx;
	uint8_t		n_type;
	uint8_t		n_sect;
	int16_t		n_desc;
	uint32_t	n_value;
}	t_nlist;

typedef struct s_nlist_64
{
	uint32_t	n_strx;
	uint8_t		n_type;
	uint8_t		n_sect;
	uint16_t	n_desc;
	uint64_t	n_value;
}	t_nlist_64;

typedef struct s_fat_header
{
	uint32_t	magic;
	uint32_t	nfat_arch;
}	t_fat_header;

typedef struct s_fat_arch
{
	int32_t		cputype;
	int32_t		cpusubtype;
	uint32_t	offset;
	uint32_t	size;
	uint32_t	align;
}	t_fat_arch;

#endif
