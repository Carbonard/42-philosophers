/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philo_utils_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 23:01:27 by rselva-2          #+#    #+#             */
/*   Updated: 2026/05/03 23:52:13 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers_bonus.h"

size_t	get_current_time_ms(void)
{
	struct timeval	tv;
	long			time_in_ms;

	if (gettimeofday(&tv, NULL))
	{
		write(2, "Error getting time\n", 19);
		return (0);
	}
	time_in_ms = tv.tv_sec * 1000000 + tv.tv_usec;
	time_in_ms /= 1000;
	return (time_in_ms);
}

static void	generate_name(char dest[MAX_SEM_NAME], char source[], int id)
{
	int				i;
	unsigned int	num;

	i = 0;
	while (source[i])
	{
		dest[i] = source[i];
		i++;
	}
	num = get_current_time_ms();
	dest[i] = '_';
	i++;
	while (num)
	{
		dest[i] = num % 10 + '0';
		i++;
		num /= 10;
	}
	while (id)
	{
		dest[i] = id % 10 + '0';
		i++;
		id /= 10;
	}
	dest[i] = 0;
}

sem_t	*create_sem(char *name, int number, int id)
{
	sem_t	*semaphore;
	char	sem_name[100];

	generate_name(sem_name, name, id);
	semaphore = sem_open(sem_name, O_CREAT | O_EXCL,
			S_IRWXU | S_IRWXG | S_IRWXO, number);
	sem_unlink(sem_name);
	if (semaphore == SEM_FAILED)
	{
		printf("Error creating semaphore '%s'\n", sem_name);
		perror(NULL);
	}
	return (semaphore);
}

int	display_msg(t_global_data *data, const char *msg, char *color)
{
	long	timestamp_in_ms;

	sem_wait(data->write_sem);
	timestamp_in_ms = get_current_time_ms() - data->initial_time;
	if (get_int(&data->finished) != 0
		|| get_current_time_ms()
		>= get_size_t(&data->last_eat) + data->time_to_die)
	{
		sem_post(data->write_sem);
		return (1);
	}
	printf("%s%-5li %i %s\001\e[0m\002\n", color, timestamp_in_ms,
		data->philo_number, msg);
	sem_post(data->write_sem);
	return (0);
}

void	display_death(t_global_data *data)
{
	long	timestamp_in_ms;

	timestamp_in_ms = get_current_time_ms() - data->initial_time;
	printf("%s%-5li %i died\001\e[0m\002\n", RED, timestamp_in_ms,
		data->philo_number);
}
