/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philo_monitor_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 22:36:48 by rselva-2          #+#    #+#             */
/*   Updated: 2026/04/23 03:33:42 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers_bonus.h"

int	check_deaths(t_global_data *data)
{
	data->double_check_sem = sem_open(data->double_check_sem_name, 0);
	if (data->double_check_sem == SEM_FAILED)
	{
		sem_post(data->death_sem);
		return (1);
	}
	sem_close(data->double_check_sem);
	if (data->number_of_times_each_philosopher_must_eat == UNLIMITED_LOGS)
		return (0);
	return (data->meals >= data->number_of_times_each_philosopher_must_eat);
}

static void	*monitorize_death(void *arg)
{
	t_global_data	*data;

	data = (t_global_data *)arg;
	while (1)
	{
		sem_wait(data->death_sem);
		if (get_current_time_ms() > data->last_eat + data->time_to_die)
		{
			sem_wait(data->write_sem);
			if (!check_deaths(data))
			{
				display_death(data);
				sem_unlink(data->double_check_sem_name);
			}
			sem_post(data->write_sem);
			sem_post(data->death_sem);
			break ;
		}
		sem_post(data->death_sem);
		usleep(1000);
	}
	return (NULL);
}

void	create_death_monitor(t_global_data *data, pthread_t *monitor)
{
	sem_close(data->double_check_sem);
	generate_name(data->death_sem_name, "philo_death", data->philo_number);
	data->death_sem = sem_open(data->death_sem_name, O_CREAT,
			S_IRWXU | S_IRWXG | S_IRWXO, 1);
	if (data->death_sem == SEM_FAILED)
	{
		perror("death_sem");
		sem_unlink(data->death_sem_name);
		exit (3);
	}
	sem_unlink(data->death_sem_name);
	pthread_create(monitor, NULL, monitorize_death, (void *)data);
}
