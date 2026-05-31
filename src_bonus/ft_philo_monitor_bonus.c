/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philo_monitor_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 22:36:48 by rselva-2          #+#    #+#             */
/*   Updated: 2026/05/08 15:15:28 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers_bonus.h"

static void	post_death(t_global_data *data)
{
	unsigned int	i;

	i = 0;
	while (i <= data->number_of_philosophers)
	{
		sem_post(data->death_sem);
		i++;
	}
	sem_post(data->forks_sem);
}

void	*monitorize_iterations(void *arg)
{
	t_global_data	*data;
	unsigned int	philos;

	data = (t_global_data *)arg;
	philos = data->number_of_philosophers;
	while (get_int(&data->finished) == 0)
	{
		sem_wait(data->iteration_sem);
		philos--;
		if (!philos)
		{
			post_death(data);
			break ;
		}
	}
	return (NULL);
}

static void	*monitorize_death(void *arg)
{
	t_global_data	*data;

	data = (t_global_data *)arg;
	while (get_int(&data->finished) == 0)
	{
		if (get_current_time_ms()
			>= get_size_t(&data->last_eat) + data->time_to_die)
		{
			sem_wait(data->write_sem);
			if (get_int(&data->finished) == 0)
				display_death(data);
			post_death(data);
			set_int(&data->finished, 1);
			usleep(10000);
			sem_post(data->write_sem);
			break ;
		}
		usleep(1000);
	}
	return (NULL);
}

void	*stop_simulation(void *args)
{
	t_global_data	*data;

	data = (t_global_data *)args;
	sem_wait(data->death_sem);
	set_int(&data->finished, 1);
	sem_post(data->iteration_sem);
	return (NULL);
}

void	create_death_monitor(
		t_global_data *data, pthread_t *death_monitor, pthread_t *sim_monitor)
{
	data->last_eat.sem = create_sem("philo_last_eat", 1, data->philo_number);
	if (data->last_eat.sem == SEM_FAILED)
		exit (ERROR_SEM);
	set_size_t(&data->last_eat, data->initial_time);
	pthread_create(death_monitor, NULL, monitorize_death, (void *)data);
	pthread_create(sim_monitor, NULL, stop_simulation, (void *)data);
}
