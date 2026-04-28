/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philo_monitor_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 22:36:48 by rselva-2          #+#    #+#             */
/*   Updated: 2026/04/28 18:00:44 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers_bonus.h"

void	*monitorize_iterations(void *arg)
{
	t_global_data	*data;
	unsigned int	philos;

	data = (t_global_data *)arg;
	philos = data->number_of_philosophers;
	sem_wait(data->data_sem);
	while (!data->is_dead)
	{
		sem_post(data->data_sem);
		sem_wait(data->iteration_sem);
		philos--;
		if (!philos)
		{
			sem_post(data->death_sem);
			break ;
		}
		sem_wait(data->data_sem);
	}
	sem_post(data->data_sem);
	return (NULL);
}

static void	*monitorize_death(void *arg)
{
	t_global_data	*data;

	data = (t_global_data *)arg;
	sem_wait(data->data_sem);
	while (!data->is_dead)
	{
		sem_post(data->data_sem);
		if (get_current_time_ms() > data->last_eat + data->time_to_die)
		{
			sem_wait(data->write_sem);
			sem_wait(data->data_sem);
			if (!data->is_dead)
				display_death(data);
			sem_post(data->death_sem);
			usleep(1000);
			sem_post(data->write_sem);
			data->is_dead = 1;
			break ;
		}
		sem_wait(data->data_sem);
	}
	sem_post(data->data_sem);
	return (NULL);
}

void	*stop_simulation(void *args)
{
	t_global_data	*data;

	data = (t_global_data *)args;
	sem_wait(data->death_sem);
	sem_post(data->death_sem);
	sem_wait(data->data_sem);
	data->is_dead = 1;
	sem_post(data->iteration_sem);
	sem_post(data->data_sem);
	return (NULL);
}

void	create_death_monitor(
		t_global_data *data, pthread_t *death_monitor, pthread_t *sim_monitor)
{
	data->data_sem = create_sem("philo_data", 0, data->philo_number);
	if (data->data_sem == SEM_FAILED)
	{
		exit (ERROR_SEM);
	}
	pthread_create(death_monitor, NULL, monitorize_death, (void *)data);
	pthread_create(sim_monitor, NULL, stop_simulation, (void *)data);
}
