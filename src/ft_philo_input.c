/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philo_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 20:06:59 by rselva-2          #+#    #+#             */
/*   Updated: 2026/04/28 14:10:53 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers.h"

static int	ft_is_numeric(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static unsigned int	ms_atoi(char *str)
{
	int				i;
	unsigned long	number;

	number = 0;
	i = 0;
	while (str[i])
	{
		if (i > 10)
			return (-1);
		number = number * 10 + str[i] - '0';
		i++;
	}
	if (number > UINT_MAX / 1000)
		return (UINT_MAX);
	return (number);
}

static int	is_valid_int(char *str)
{
	if (!ft_is_numeric(str))
	{
		printf("Invalid argument: ");
		printf("%s is not numeric (or it is negative)\n", str);
		return (0);
	}
	if (ms_atoi(str) == UINT_MAX)
	{
		printf("Invalid argument: ");
		printf("number %s is greater than %d\n", str, UINT_MAX / 1000);
		return (0);
	}
	return (1);
}

int	check_input(int argc, char **argv)
{
	int	i;

	if (argc < 5)
	{
		printf("At least 4 arguments are needed\n");
		return (0);
	}
	if (argc > 6)
	{
		printf("No more than 5 arguments are accepted\n");
		return (0);
	}
	i = 1;
	while (i < argc)
	{
		if (!is_valid_int(argv[i]))
			return (0);
		i++;
	}
	return (1);
}

void	save_data(int argc, char **argv, t_global_data *data)
{
	data->number_of_philosophers = ms_atoi(argv[1]);
	data->time_to_die = ms_atoi(argv[2]);
	data->time_to_eat = ms_atoi(argv[3]);
	data->time_to_sleep = ms_atoi(argv[4]);
	if (argc == 6)
		data->number_of_times_each_philosopher_must_eat = ms_atoi(argv[5]);
	else
		data->number_of_times_each_philosopher_must_eat = UNLIMITED_LOGS;
}
