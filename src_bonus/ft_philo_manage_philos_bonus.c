/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philo_manage_philos_bonus.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 22:58:24 by rselva-2          #+#    #+#             */
/*   Updated: 2026/04/23 00:25:06 by rselva-2         ###   ########.fr       */
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

void	open_semaphores(t_global_data *data)
{
	generate_name(data->forks_sem_name, "philo_forks", 42);
	generate_name(data->write_sem_name, "philo_write", 42);
	generate_name(data->double_check_sem_name, "philo_double_check", 42);
	data->forks_sem = sem_open(data->forks_sem_name, O_CREAT | O_EXCL,
			S_IRWXU | S_IRWXG | S_IRWXO, data->number_of_philosophers);
	data->write_sem = sem_open(data->write_sem_name, O_CREAT | O_EXCL,
			S_IRWXU | S_IRWXG | S_IRWXO, 1);
	data->double_check_sem = sem_open(data->double_check_sem_name,
			O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO, 1);
	data->control_sem = sem_open("philo_forks_control",
			O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO,
			(data->number_of_philosophers + 1) / 2);
	if (!data->forks_sem || !data->write_sem || !data->double_check_sem
		|| !data->control_sem)
	{
		printf("semaphore already exists\n");
		sem_close(data->forks_sem);
		sem_unlink(data->forks_sem_name);
		sem_close(data->write_sem);
		sem_unlink(data->write_sem_name);
		exit(2);
	}
	sem_unlink(data->forks_sem_name);
	sem_unlink(data->write_sem_name);
	sem_unlink("philo_forks_control");
}

int	manage_philosophers(t_global_data *data)
{
	int	*philosophers;

	open_semaphores(data);
	philosophers = malloc(data->number_of_philosophers * sizeof(int));
	if (!philosophers)
		exit (1);
	init_philos(data, philosophers);
	sem_close(data->forks_sem);
	sem_close(data->write_sem);
	sem_close(data->double_check_sem);
	sem_close(data->control_sem);
	close_philos(data, philosophers);
	free(philosophers);
	return (0);
}
