/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philo_manage_philos_bonus.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 22:58:24 by rselva-2          #+#    #+#             */
/*   Updated: 2026/04/28 17:23:16 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers_bonus.h"

static int	init_philos(t_global_data *data, int *philosophers)
{
	unsigned int	i;

	data->is_dead = 0;
	data->last_eat = data->initial_time;
	i = 0;
	while (i < data->number_of_philosophers)
	{
		philosophers[i] = fork();
		if (philosophers[i] == 0)
		{
			free(philosophers);
			data->philo_number = i + 1;
			philo_routine(data);
		}
		i++;
	}
	return (0);
}

static int	close_philos(t_global_data *data, int *philosophers)
{
	unsigned int	i;
	int				wstatus;
	int				final_status;

	final_status = 0;
	i = 0;
	while (i < data->number_of_philosophers)
	{
		final_status |= waitpid(philosophers[i], &wstatus, 0);
		i++;
	}
	return (0);
}

void	close_semaphores(t_global_data *data)
{
	if (data->forks_sem != SEM_FAILED)
		sem_close(data->forks_sem);
	if (data->write_sem != SEM_FAILED)
		sem_close(data->write_sem);
	if (data->death_sem != SEM_FAILED)
		sem_close(data->death_sem);
	if (data->control_sem != SEM_FAILED)
		sem_close(data->control_sem);
	if (data->data_sem != SEM_FAILED)
		sem_close(data->data_sem);
	if (data->iteration_sem != SEM_FAILED)
		sem_close(data->iteration_sem);
}

static void	open_semaphores(t_global_data *data)
{
	data->forks_sem = create_sem("philo_fork", data->number_of_philosophers, 0);
	data->write_sem = create_sem("philo_write", 1, 0);
	data->death_sem = create_sem("philo_death", 0, 0);
	data->iteration_sem = create_sem("philo_iteration", 0, 0);
	data->control_sem = create_sem("philo_forks_control",
			(data->number_of_philosophers + 1) / 2, 0);
	if (!data->forks_sem || !data->write_sem || !data->death_sem
		|| !data->control_sem || !data->iteration_sem)
	{
		printf("Error creating semaphores\n");
		close_semaphores(data);
		exit(ERROR_SEM);
	}
	data->data_sem = SEM_FAILED;
}

int	manage_philosophers(t_global_data *data)
{
	int			*philosophers;
	pthread_t	iter_monitor;
	pthread_t	sim_monitor;

	open_semaphores(data);
	philosophers = malloc(data->number_of_philosophers * sizeof(int));
	if (!philosophers)
		exit (ERROR_MALLOC);
	init_philos(data, philosophers);
	data->data_sem = create_sem("philo_data", 1, 0);
	pthread_create(&iter_monitor, NULL, monitorize_iterations, data);
	pthread_create(&sim_monitor, NULL, stop_simulation, data);
	pthread_join(iter_monitor, NULL);
	pthread_join(sim_monitor, NULL);
	close_semaphores(data);
	close_philos(data, philosophers);
	free(philosophers);
	return (0);
}
