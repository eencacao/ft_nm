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
| `I/i` | Indirect reference to another symbol |
| `N` | Debug symbol |
| `R/r` | Read-only data |
| `S/s` | Small data section |
| `T/t` | Text (code) section |
| `U` | Undefined symbol |
| `W/w` | Weak symbol |
| `V/v` | Weak object |

*Uppercase = global, lowercase = local*

### Symbol Type Details

**T/t - Text (Code)**
Functions and executable code. Located in the `.text` section.
```c
void my_function(void) { }  // T _my_function
static void helper(void) { } // t _helper
```

**D/d - Data (Initialized)**
Global variables with initial values. Located in the `.data` section.
```c
int global_var = 42;        // D _global_var
static int local_var = 10;  // d _local_var
```

**B/b - BSS (Uninitialized)**
Global variables without initial values (zero-initialized). Located in `.bss` section.
```c
int uninitialized;          // B _uninitialized
static int local_uninit;    // b _local_uninit
```

**R/r - Read-only Data**
Constants and read-only variables. Located in `.rodata` section (read-only data).
- `R` = global read-only symbol
- `r` = local read-only symbol (static const)
```c
const int global_const = 100;         // R _global_const
static const int local_const = 50;    // r _local_const
const char *str = "hello";            // R _str
static const char msg[] = "hi";       // r _msg
```
The linker places these in memory pages marked as non-writable.
Writing to these symbols causes a segmentation fault.

**U - Undefined**
Symbol is referenced but not defined in this file. Must be resolved at link time.
```c
extern void external_func(void); // U _external_func
printf("hello");                 // U _printf
```

**A - Absolute**
Symbol has a fixed address that won't change during linking.

**C - Common**
Uninitialized global variable (legacy behavior, similar to BSS).
```c
int common_var;  // C _common_var (without extern, in some compilers)
```

**W/w - Weak Symbol**
Can be overridden by a strong symbol of the same name at link time.
- `W` = weak defined symbol (typically functions)
- `w` = weak undefined symbol (resolved to NULL/0 if not found at link time)
```c
__attribute__((weak)) void optional_hook(void) { }  // W _optional_hook
extern void __attribute__((weak)) maybe_exists(void); // w if unresolved
```
Use cases:
- Default implementations that can be overridden
- Optional library features
- Plugin systems

**V/v - Weak Object**
Weak symbol that is specifically a data object (variable), not a function.
- `V` = weak global object
- `v` = weak local object (rare)
```c
__attribute__((weak)) int optional_config = 42;  // V _optional_config
```
*Note: V/v is ELF-specific. Mach-O uses W/w for all weak symbols.*

**I/i - Indirect Symbol**
Reference that resolves to another symbol (GNU extension, also in Mach-O).
Used for symbol aliasing and resolver functions.
- `I` = global indirect (externally visible)
- `i` = local indirect (file-scoped, static)
```c
// GNU indirect function - resolved at runtime
__attribute__((ifunc("resolver"))) void my_func(void);        // I
static __attribute__((ifunc("res"))) void local_func(void);   // i
```

**N - Debug Symbol**
Symbol used for debugging only (not allocated in memory).
Only shown with `-a` flag. Located in debug sections like `.debug_*`.

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