/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philo_manage_philos.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 22:58:24 by rselva-2          #+#    #+#             */
/*   Updated: 2026/05/06 15:34:24 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers.h"

static int	join_philos(t_global_data *g_data, pthread_t *philosophers)
{
	unsigned int	i;
	int				error;

	error = 0;
	i = 0;
	while (i < g_data->number_of_philosophers)
	{
		if (philosophers[i] && pthread_join(philosophers[i], NULL))
		{
			printf("Error: pthread_join failed");
			error = E_JOIN_THREADS;
		}
		i++;
	}
	return (error);
}

static int	destroy_all_mutex(pthread_mutex_t **mutex_arr, unsigned int size)
{
	unsigned int	i;
	int				error;

	error = 0;
	i = 0;
	while (i < size)
	{
		if (pthread_mutex_destroy(mutex_arr[i]))
		{
			write(2, "Error destroying mutex\n", 23);
			printf("number: %u\n", i);
			error = E_DESTROY_MUTEX;
		}
		i++;
	}
	return (error);
}

int	manage_philosophers(t_global_data *g_data)
{
	pthread_t		*philosophers;
	t_philo_data	*philos_data;
	pthread_mutex_t	**all_mutex;
	pthread_t		monitor;
	int				error;

	error = init_all(g_data, &philosophers, &philos_data, &all_mutex);
	if (error && error != E_INIT_THREADS)
		exit (error);
	if (pthread_create(&monitor, NULL, monitorize, (void *)g_data))
	{
		error = E_INIT_THREADS;
		write(2, "Error initializing threads\n", 27);
		set_int(&(g_data->any_death), 1);
	}
	error |= join_philos(g_data, philosophers);
	pthread_join(monitor, NULL);
	error |= destroy_all_mutex(all_mutex,
			g_data->number_of_philosophers * 2 + 4);
	free(philosophers);
	free(g_data->forks);
	free(philos_data);
	free(g_data->last_eats);
	free(all_mutex);
	return (error);
}
