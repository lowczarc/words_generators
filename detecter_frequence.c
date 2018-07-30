#include <stdio.h>
#include <locale.h>
#include <wchar.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define PROFONDEUR 4

int	ft_rand(int min, int max);

void	decaler_tab(unsigned char* tab, unsigned int size)
{
	int	i = size - 2;

	while (i >= 0)
	{
		tab[i + 1] = tab[i];
		i--;
	}
}

void	increment_occ(unsigned char* tab, void *occ, unsigned int size)
{
	if (size == 1)
	{
		((long*)occ)[*tab]++;
	}
	else
	{
		if (((void**)occ)[tab[size - 1]] == NULL)
		{
			((void**)occ)[tab[size - 1]] = malloc(sizeof(long*) * 256);
			bzero(((void**)occ)[tab[size - 1]], sizeof(long*) * 256);
		}
		increment_occ(tab, ((void**)occ)[tab[size - 1]], size - 1);
	}
}

long	*return_1d_tab(unsigned char* tab, void *occ, unsigned int size)
{
	if (size == 1)
		return (occ);
	else
		return return_1d_tab(tab, ((void**)occ)[tab[size - 1]], size - 1);
}

void	read_words(void *occ, int profondeur)
{
	unsigned char* tab;

	tab = malloc(sizeof(unsigned char*) * profondeur);
	memset(tab, '\n', profondeur);
	while (read(0, tab, 1) == 1)
	{
		if (profondeur == 1 || (tab[0] && tab[0] !='\n' && tab[0] != 0x0d))
		{
			increment_occ(tab, occ, profondeur);
			decaler_tab(tab, profondeur);
		}
		else if (profondeur > 1 && tab[1] && tab[1] !='\n' && tab[1] != 0x0d)
		{
			tab[0] = '\n';
			increment_occ(tab, occ, profondeur);
			memset(tab, '\n', profondeur);
		}
	}
}

int nb_tot_in_tab(long *tab)
{
	int i = 0;
	int res = 0;

	while (i < 256)
	{
		res += tab[i];
		i++;
	}
	return (res);
}

char choose_in_tab(long *tab, int nb)
{
	int	i = 0;

	while (i < 256)
	{
		nb -= tab[i];
			if (nb < 0)
				return ((char)i);
		i++;
	}
	return (0);
}

void create_word(long ***occ, unsigned char* tab, int profondeur)
{
	do {
		decaler_tab(tab, profondeur);
		tab[0] = choose_in_tab(return_1d_tab(tab, occ, profondeur), ft_rand(0, nb_tot_in_tab(return_1d_tab(tab, occ, profondeur))));
		write(1, tab, 1);
	} while (tab[0] != '\n');
}

int	main(int argc, char **argv)
{
	void *tab;
	unsigned char* a;

	if (PROFONDEUR <= 0)
		return (-1);
	tab = malloc(sizeof(void*) * 256);
	bzero(tab, sizeof(long*) * 256);
	read_words(tab, PROFONDEUR);
	a = malloc(sizeof(unsigned char*) * PROFONDEUR);
	for(int i = 0; i < 50; i++)
	{
		memset(a, '\n', sizeof(unsigned char*) * PROFONDEUR);
		create_word(tab, a, PROFONDEUR);
	}
}
