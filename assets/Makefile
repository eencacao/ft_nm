CC = gcc
CFLAGS = -Wall -Wextra -Werror

SRCS = text.c data.c bss.c rodata.c undef.c weak.c weak_obj.c common.c

OBJS = $(SRCS:.c=.o)

all: $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean

re: fclean all

.PHONY: all clean fclean re
.NOTPARALLEL:
