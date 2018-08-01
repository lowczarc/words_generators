#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <math.h>

#define PROFONDEUR 2

int		ft_rand(int min, int max);

void	decaler_tab(unsigned char* tab, unsigned int size)
{
	for (int i = size - 2; i >= 0; i--)
		tab[i + 1] = tab[i];
}

void	increment_occ(unsigned char* tab, void *occ, unsigned int size)
{
	if (size == 1)
		((long*)occ)[*tab]++;
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
	else if (occ == NULL)
		return (NULL);
	else
		return return_1d_tab(tab, ((void**)occ)[tab[size - 1]], size - 1);
}

void	read_words(int fd, void *occ, int profondeur)
{
	unsigned char* tab;

	tab = malloc(sizeof(unsigned char*) * profondeur);
	memset(tab, '\n', profondeur);
	while (read(fd, tab, 1) == 1)
	{
		if (profondeur == 1 || (tab[0] && tab[0] != ' ' && tab[0] !='\n' && tab[0] != 0x0d))
		{
			increment_occ(tab, occ, profondeur);
			decaler_tab(tab, profondeur);
		}
		else if (profondeur > 1 && tab[1] && tab[1] != ' ' && tab[1] !='\n' && tab[1] != 0x0d)
		{
			tab[0] = '\n';
			increment_occ(tab, occ, profondeur);
			memset(tab, '\n', profondeur);
		}
	}
}

int		nb_tot_in_tab(long *tab)
{
	int res = 0;

	if (tab == NULL)
		return (1);
	for (int i = 0; i < 256; i++)
		res += tab[i];
	return (res);
}

char	choose_in_tab(long *tab, int nb)
{
	if (tab == NULL)
		return (0);
	for (int i = 0; i < 256; i++)
	{
		nb -= tab[i];
			if (nb < 0)
				return ((char)i);
	}
	return (0);
}

void	create_word(void *occ, unsigned char* tab, int profondeur)
{
	do {
		decaler_tab(tab, profondeur);
		tab[0] = choose_in_tab(return_1d_tab(tab, occ, profondeur), ft_rand(0, nb_tot_in_tab(return_1d_tab(tab, occ, profondeur))));
		write(1, tab, 1);
	} while (tab[0] != '\n');
}

double	calculate_luck(void *occ, unsigned char* tab, int profondeur)
{
	long nume;
	long deno;
	long *tmp;

	tmp = return_1d_tab(tab, occ, profondeur);
	if (tmp == NULL)
		nume = 0;
	else
		nume = tmp[tab[0]];
	deno = nb_tot_in_tab(return_1d_tab(tab, occ, profondeur));
	return ((double)deno/(double)nume);
}

int		main(int argc, char **argv)
{
	void *tab1;
	void *tab2;
	unsigned char* a;
	int fd1;
	int fd2;
	double luck1 = 1;
	double luck2 = 1;
	double finaluck1 = 1;
	double finaluck2 = 1;

	if (PROFONDEUR <= 0)
		return (-1);
	tab1 = malloc(sizeof(void*) * 256);
	tab2 = malloc(sizeof(void*) * 256);
	bzero(tab1, sizeof(long*) * 256);
	bzero(tab2, sizeof(long*) * 256);
	fd1 = open(argv[1], O_RDONLY);
	fd2 = open(argv[2], O_RDONLY);
	read_words(fd1, tab1, PROFONDEUR);
	read_words(fd2, tab2, PROFONDEUR);
	a = malloc(sizeof(unsigned char*) * PROFONDEUR);
	memset(a, '\n', sizeof(unsigned char*) * PROFONDEUR);
	while (read(0, a, 1) == 1)
	{
		if (PROFONDEUR == 1 || (a[0] && a[0] != ' ' && a[0] !='\n' && a[0] != 0x0d))
		{
			luck1 *= calculate_luck(tab1, a, PROFONDEUR);
			luck2 *= calculate_luck(tab2, a, PROFONDEUR);
			decaler_tab(a, PROFONDEUR);
		}
		else if (a[1] && a[1] != ' ' && a[1] != '\n' && a[1] != 0x0d)
		{
			a[0] = '\n';
			luck1 *= calculate_luck(tab1, a, PROFONDEUR);
			luck2 *= calculate_luck(tab2, a, PROFONDEUR);
			if (luck1 == INFINITY && luck2 == INFINITY)
			{
				luck1 = 0;
				luck2 = 0;
			}
			else if (luck1 == INFINITY)
			{
				luck2 = 0;
				luck1 = 1;
			}
			else if (luck2 == INFINITY)
			{
				luck1 = 0;
				luck2 = 1;
			}
			finaluck1 *= (luck2/(luck1+luck2));
			finaluck2 *= (luck1/(luck1+luck2));
			finaluck1 = (finaluck1/(finaluck1+finaluck2));
			finaluck2 = (finaluck2/(finaluck1+finaluck2));
			luck1 = 1;
			luck2 = 1;
			memset(a, '\n', sizeof(unsigned char*) * PROFONDEUR);
		}
	}
		printf("%s : %lf%%, %s : %lf%%\n", argv[1], 100*(finaluck1/(finaluck1+finaluck2)), argv[2], 100*(finaluck2/(finaluck1+finaluck2)));
//	for(int i = 0; i < 50; i++)
//	{
//		memset(a, '\n', sizeof(unsigned char*) * PROFONDEUR);
//		create_word(tab, a, PROFONDEUR);
//	}
}
