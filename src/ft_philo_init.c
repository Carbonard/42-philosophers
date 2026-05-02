/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philo_init.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 15:18:26 by rselva-2          #+#    #+#             */
/*   Updated: 2026/05/02 16:27:52 by rselva-2         ###   ########.fr       */
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

static int	init_other_mutex(
		t_global_data *g_data, pthread_mutex_t *mutex_arr[4])
{
	unsigned int	i_forks;
	int				i_mutex;

	i_forks = g_data->number_of_philosophers;
	i_mutex = 0;
	while (i_mutex < 4)
	{
		if (pthread_mutex_init(mutex_arr[i_mutex], NULL))
		{
			printf("Error: pthread_mutex_init failed\n");
			while (i_forks)
			{
				i_forks--;
				pthread_mutex_destroy(&(g_data->forks[i_forks].mutex));
			}
			while (i_mutex)
			{
				i_mutex--;
				pthread_mutex_destroy(mutex_arr[i_mutex]);
			}
			return (E_INIT_MUTEX);
		}
		i_mutex++;
	}
	return (0);
}

static int	init_mutex(t_global_data *g_data)
{
	unsigned int	i;
	pthread_mutex_t	*mutex_arr[4];

	i = 0;
	while (i < g_data->number_of_philosophers)
	{
		if (pthread_mutex_init(&(g_data->forks[i].mutex), NULL))
		{
			printf("Error: pthread_mutex_init failed\n");
			while (i)
			{
				i--;
				pthread_mutex_destroy(&(g_data->forks[i].mutex));
			}
			return (E_INIT_MUTEX);
		}
		i++;
	}
	mutex_arr[0] = &(g_data->write_mutex);
	mutex_arr[1] = &(g_data->any_death.mutex);
	mutex_arr[2] = &(g_data->start_time.mutex);
	mutex_arr[3] = &(g_data->finished.mutex);
	return (init_other_mutex(g_data, mutex_arr));
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
	usleep(100);
	set_size_t(&(g_data->start_time), get_current_time_ms());
	return (error);
}

int	init_all(t_global_data *g_data, pthread_t **philosophers,
		t_philo_data **philos_data)
{
	if (allocate_mem(g_data, philosophers, philos_data,
			g_data->number_of_philosophers))
	{
		write(2, "Memory error\n", 13);
		return (E_MALLOC);
	}
	if (init_mutex(g_data))
	{
		write(2, "Error initializing mutex\n", 25);
		return (E_INIT_MUTEX);
	}
	if (init_philos(g_data, *philosophers, *philos_data))
	{
		write(2, "Error initializing threads\n", 27);
		return (E_INIT_THREADS);
	}
	return (0);
}
