#include "ft_nm.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

static char	to_lower(char c)
{
	if (c >= 'A' && c <= 'Z')
		return (c + 32);
	return (c);
}

int	ft_strcmp_nocase(const char *s1, const char *s2)
{
	if (*s1 == '_' && *s2 == '_')
	{
		s1++;
		s2++;
	}
	while (*s1 && *s2)
	{
		if (to_lower(*s1) != to_lower(*s2))
			return (to_lower(*s1) - to_lower(*s2));
		s1++;
		s2++;
	}
	return (to_lower(*s1) - to_lower(*s2));
}

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*p;

	p = s;
	while (n--)
		*p++ = (unsigned char)c;
	return (s);
}
