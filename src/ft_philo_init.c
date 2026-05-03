/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philo_init.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 15:18:26 by rselva-2          #+#    #+#             */
/*   Updated: 2026/05/03 17:08:37 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers.h"

static int	allocate_mem(t_global_data *g_data, pthread_t **philosophers,
	t_philo_data **philos_data, unsigned int size)
{
	*philosophers = malloc(size * sizeof(pthread_t));
	if (!*philosophers)
		return (1);
	g_data->forks = malloc(size * sizeof(t_protected_int));
	if (!g_data->forks)
	{
		free(philosophers);
		return (1);
	}
	*philos_data = malloc(size * sizeof(t_philo_data));
	if (!*philos_data)
	{
		free(philosophers);
		free(g_data->forks);
		return (1);
	}
	g_data->last_eats = malloc(size * sizeof(t_protected_size_t));
	if (!g_data->last_eats)
	{
		free(philosophers);
		free(g_data->forks);
		free(philos_data);
		return (1);
	}
	return (0);
}

static int	init_mutex_array(pthread_mutex_t **mutex_arr, unsigned int size)
{
	unsigned int	i;

	i = 0;
	while (i < size)
	{
		if (pthread_mutex_init(mutex_arr[i], NULL))
		{
			write(2, "Error initializing mutex\n", 25);
			while (i)
			{
				i--;
				pthread_mutex_destroy(mutex_arr[i]);
			}
			return (E_INIT_MUTEX);
		}
		i++;
	}
	return (0);
}

static int	init_forks_and_mutex(t_global_data *g_d,
				pthread_mutex_t ***mutex)
{
	unsigned int	i;

	*mutex = malloc(
			(g_d->number_of_philosophers * 2 + 4) * sizeof(pthread_mutex_t *));
	if (!*mutex)
		return (E_MALLOC);
	i = 0;
	while (i < g_d->number_of_philosophers)
	{
		(*mutex)[i] = &(g_d->forks[i].mutex);
		(*mutex)[i + g_d->number_of_philosophers] = &(g_d->last_eats[i].mutex);
		i++;
	}
	(*mutex)[i * 2 + 0] = &(g_d->any_death.mutex);
	(*mutex)[i * 2 + 1] = &(g_d->finished.mutex);
	(*mutex)[i * 2 + 2] = &(g_d->start_time.mutex);
	(*mutex)[i * 2 + 3] = &(g_d->write_mutex);
	if (init_mutex_array(*mutex, g_d->number_of_philosophers * 2 + 4))
		return (E_INIT_MUTEX);
	while (i)
	{
		i--;
		set_int(g_d->forks + i, 1);
	}
	return (E_SUCCESS);
}

static int	init_philos(t_global_data *g_data, pthread_t *philosophers,
	t_philo_data *philos_data)
{
	unsigned int	i;
	int				error;

	set_int(&(g_data->any_death), 0);
	set_size_t(&(g_data->start_time), 0);
	set_uint(&(g_data->finished), 0);
	i = 0;
	error = E_SUCCESS;
	memset(philosophers, 0, g_data->number_of_philosophers * sizeof(pthread_t));
	while (i < g_data->number_of_philosophers)
	{
		fill_philo_data(philos_data + i, i, g_data);
		if (pthread_create(&philosophers[i], NULL,
				philo_routine, (void *)(philos_data + i)))
		{
			philosophers[i] = 0;
			error = E_INIT_THREADS;
			set_int(&(g_data->any_death), 1);
			break ;
		}
		i++;
	}
	usleep(10000);
	set_size_t(&(g_data->start_time), get_current_time_ms());
	return (error);
}

int	init_all(t_global_data *g_data, pthread_t **philosophers,
		t_philo_data **philos_data, pthread_mutex_t ***all_mutex)
{
	int	error;

	if (allocate_mem(g_data, philosophers, philos_data,
			g_data->number_of_philosophers))
	{
		write(2, "Memory error\n", 13);
		return (E_MALLOC);
	}
	error = init_forks_and_mutex(g_data, all_mutex);
	if (error)
		return (error);
	if (init_philos(g_data, *philosophers, *philos_data))
	{
		write(2, "Error initializing threads\n", 27);
		return (E_INIT_THREADS);
	}
	return (0);
}
