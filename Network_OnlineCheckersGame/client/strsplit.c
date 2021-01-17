#include "main.h"

static size_t	count_words(char const *s, char c)
{
	size_t	count;

	count = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
			count++;
		while (*s && *s != c)
			s++;
	}
	return (count);
}

static char		*strsub(char const *s, unsigned int start, size_t len)
{
	char	*dst;

	if (!s)
		return (NULL);
	if (!(dst = (char*)calloc((len + 1), sizeof(char))))
		return (NULL);
	dst[len] = '\0';
	strncpy(dst, s + start, len);
	return (dst);
}

void			deltab(void ***matrix, size_t size) {
	if (!matrix || !(*matrix))
		return ;
	for (size_t i = 0; i < size; i++) {
		free((*matrix)[i]);
		(*matrix)[i] = NULL;
	}
	free(*matrix);
	*matrix = NULL;
}

size_t			strsplit(char const *s, char ***tab_addr, char c)
{
	char	**tab = *tab_addr;
	size_t	tab_len;
	size_t	i;
	size_t	len;

	tab = NULL;
	if (!s || !c)
		return (0);
	tab_len = count_words(s, c);
	if (!(tab = (char **)calloc((tab_len + 1), sizeof(char *))))
		return (0);
	tab[tab_len] = NULL;
	i = 0;
	while (i < tab_len)
	{
		while (*s == c)
			s++;
		len = 0;
		while (s[len] && s[len] != c)
			len++;
		if (!(tab[i] = strsub(s, 0, len)))
			return (0);
		s += len;
		i++;
	}
	*tab_addr = tab;
	return (tab_len);
}
