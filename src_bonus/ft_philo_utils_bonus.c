/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philo_utils_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 23:01:27 by rselva-2          #+#    #+#             */
/*   Updated: 2026/04/23 00:50:44 by rselva-2         ###   ########.fr       */
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

void	generate_name(char dest[MAX_SEM_NAME], char source[], int philo_num)
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
	while (philo_num)
	{
		dest[i] = philo_num % 10 + '0';
		i++;
		philo_num /= 10;
	}
	dest[i] = 0;
}

int	display_msg(t_global_data *data, const char *msg, char *color)
{
	long	timestamp_in_ms;

	sem_wait(data->write_sem);
	data->double_check_sem = sem_open(data->double_check_sem_name, 0);
	if (data->double_check_sem == SEM_FAILED)
	{
		sem_post(data->write_sem);
		return (1);
	}
	timestamp_in_ms = get_current_time_ms() - data->initial_time;
	printf("%s%-5li %i %s\001\e[0m\002\n", color, timestamp_in_ms,
		data->philo_number, msg);
	sem_close(data->double_check_sem);
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
