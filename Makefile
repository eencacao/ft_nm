NAME		= ft_nm

CC			= gcc
CFLAGS		= -Wall -Wextra -Werror
INCLUDES	= -I includes

SRCDIR		= srcs
OBJDIR		= objs

SRCS		= main.c \
			  elf_parser.c \
			  elf32.c \
			  elf32_utils.c \
			  elf64.c \
			  elf64_utils.c \
			  macho64.c \
			  macho64_utils.c \
			  macho64_fill.c \
			  macho32.c \
			  macho32_utils.c \
			  file_detect.c \
			  output.c \
			  sort.c \
			  options.c \
			  utils.c \
			  error.c

OBJS		= $(addprefix $(OBJDIR)/, $(SRCS:.c=.o))

all: $(NAME) assets

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

assets:
	$(MAKE) -C assets

clean:
	rm -rf $(OBJDIR)
	$(MAKE) -C assets clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C assets fclean

re: fclean all

.PHONY: all clean fclean re assets
.NOTPARALLEL:
