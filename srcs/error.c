#include "ft_nm.h"

void	print_error(const char *filename, const char *msg)
{
	fprintf(stderr, "ft_nm: %s: %s\n", filename, msg);
}
