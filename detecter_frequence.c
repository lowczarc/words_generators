#include <stdio.h>
#include <locale.h>
#include <wchar.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int	ft_rand(int min, int max);

void 	aff_res(long **occ)
{
	int i = 0;
	int j;
	int a;
	long k = 0;

	while (i < 256)
	{
		j = 0;
		a = 0;
		while (j < 256)
		{
			if (occ[i][j])
				a = 1;
			j++;
		}
		if (a)
		{
			j = 0;
			k = 0;
			printf("%c (%#hhx) :\n", i, i);
			while (j < 256)
			{
				if (occ[i][j])
					printf("\t%c (%#hhx) : %ld\n", j, j, occ[i][j]);
				k += occ[i][j];
				j++;
			}
	//		printf("\t\ttotal : %ld\n", k);
		}
		i++;
	}
}

long	**read_words()
{
	unsigned char b = '\n';
	unsigned char a = 'a';
	long **occ;
	int i = 0;

	occ = malloc(sizeof(long*) * 256);
	while (i < 256)
	{
		occ[i] = malloc(sizeof(long) * 256);
		bzero(occ[i], sizeof(sizeof(long) * 256));
		i++;
	}
	while (read(0, &a, 1) == 1)
	{
		if (a && a !='\n' && a != 0x0d)
		{
			occ[b][a] += 1;
			b = a;
		}
		else if (b && b !='\n' && b != 0x0d)
		{
			occ[b]['N'] += 1;
			b = '\n';
		}
		a = 0;
	}
	return (occ);
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

void create_word(long **tab, unsigned char first)
{
	while (first != 'N')
	{
		first = choose_in_tab(tab[first], ft_rand(0, nb_tot_in_tab(tab[first])));
		if (first != 'N')
			write(1, &first, 1);
	}
	write(1, "\n", 1);
}

int	main(int argc, char **argv)
{
	long **tab;

	tab = read_words();
	//aff_res(tab);
	for(int i = 0; i < 50; i++)
		create_word(tab, '\n');
}
