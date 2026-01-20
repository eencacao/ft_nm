# FT_NM

ft_nm - display name list (symbol table)

Works with ELF and Mach-O binaries (32-bit and 64-bit).

## Usage

```
./ft_nm [-agurp] [file...]
```

## Options

| Option | Description |
|--------|-------------|
| `-a` | Display all symbol table entries, including debugger symbols |
| `-g` | Display only global (external) symbols |
| `-u` | Display only undefined symbols |
| `-r` | Sort in reverse order |
| `-p` | Don't sort; display in symbol-table order |

## Architecture

```
                            ┌─────────────┐
                            │   ft_nm     │
                            │   main()    │
                            └──────┬──────┘
                                   │
                    ┌──────────────┼──────────────┐
                    ▼              ▼              ▼
             ┌────────────┐ ┌────────────┐ ┌────────────┐
             │ map_file() │ │  options   │ │  output    │
             │  mmap()    │ │  parsing   │ │  sorting   │
             └─────┬──────┘ └────────────┘ └────────────┘
                   │
                   ▼
          ┌────────────────┐
          │ detect_file_   │
          │    type()      │
          └───────┬────────┘
                  │
        ┌─────────┴─────────┐
        ▼                   ▼
┌───────────────┐   ┌───────────────┐
│   ELF File    │   │  Mach-O File  │
└───────┬───────┘   └───────┬───────┘
        │                   │
   ┌────┴────┐         ┌────┴────┐
   ▼         ▼         ▼         ▼
┌──────┐ ┌──────┐ ┌──────┐ ┌──────┐
│ ELF  │ │ ELF  │ │Mach-O│ │Mach-O│
│  32  │ │  64  │ │  32  │ │  64  │
└──┬───┘ └──┬───┘ └──┬───┘ └──┬───┘
   │        │        │        │
   └────────┴────────┴────────┘
                  │
                  ▼
         ┌────────────────┐
         │ Symbol Table   │
         │ - name         │
         │ - value        │
         │ - type (T/U/D) │
         └───────┬────────┘
                 │
                 ▼
         ┌────────────────┐
         │  sort_symbols  │
         │ (alphabetical) │
         └───────┬────────┘
                 │
                 ▼
         ┌────────────────┐
         │ print_symbols  │
         │   to stdout    │
         └────────────────┘
```

## Symbol Types

| Type | Description |
|------|-------------|
| `A` | Absolute symbol |
| `B/b` | BSS (uninitialized data) |
| `C` | Common symbol |
| `D/d` | Initialized data section |
| `T/t` | Text (code) section |
| `U` | Undefined symbol |
| `S/s` | Small data section |
| `R/r` | Read-only data |
| `W/w` | Weak symbol |

*Uppercase = global, lowercase = local*

## File Structure

```
ft_nm/
├── Makefile
├── includes/
│   ├── ft_nm.h         # Main header with structs & prototypes
│   └── macho.h         # Mach-O format definitions
└── srcs/
    ├── main.c          # Entry point, argument handling
    ├── file_detect.c   # ELF/Mach-O detection
    ├── elf_parser.c    # ELF file mapping & validation
    ├── elf32.c         # ELF 32-bit symbol extraction
    ├── elf32_utils.c   # ELF 32-bit helpers
    ├── elf64.c         # ELF 64-bit symbol extraction
    ├── elf64_utils.c   # ELF 64-bit helpers
    ├── macho32.c       # Mach-O 32-bit symbol extraction
    ├── macho32_utils.c # Mach-O 32-bit helpers
    ├── macho64.c       # Mach-O 64-bit symbol extraction
    ├── macho64_utils.c # Mach-O 64-bit helpers
    ├── output.c        # Symbol printing
    ├── sort.c          # Symbol sorting
    ├── options.c       # Command-line parsing
    ├── utils.c         # String utilities
    └── error.c         # Error handling
```

## Build

```bash
make        # Build
make clean  # Remove objects
make fclean # Remove all
make re     # Rebuild
```