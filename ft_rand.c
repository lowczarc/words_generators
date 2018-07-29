/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rand.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zerevo <lowczarc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/02 20:47:14 by zerevo            #+#    #+#             */
/*   Updated: 2018/02/20 11:22:32 by lowczarc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>

#define RANDOM_FILE "/dev/random"

int	ft_rand(int min, int max)
{
	unsigned int		ret;
	int					fd;

	ret = 0;
	if (max < min)
		return (0);
	fd = open(RANDOM_FILE, O_RDONLY);
	read(fd, &ret, sizeof(int));
	close(fd);
	ret = (unsigned int)((unsigned long long)ret * (max - min) / 0xFFFFFFFF);
	return ((int)ret + min);
}
