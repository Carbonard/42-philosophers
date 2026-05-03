/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philo_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 23:01:27 by rselva-2          #+#    #+#             */
/*   Updated: 2026/05/02 18:44:49 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers.h"

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

int	wait_ms(t_philo_data *data, useconds_t msec)
{
	size_t	init_time;

	init_time = get_current_time_ms();
	while (get_current_time_ms() < init_time + msec)
	{
		if (get_int(data->any_death) != 0)
			return (1);
		usleep(100);
	}
	return (0);
}

void	fill_philo_data(t_philo_data *p_data, int i, t_global_data *g_data)
{
	p_data->time_to_die = g_data->time_to_die;
	p_data->time_to_eat = g_data->time_to_eat;
	p_data->time_to_sleep = g_data->time_to_sleep;
	p_data->number_of_times_each_philosopher_must_eat
		= g_data->number_of_times_each_philosopher_must_eat;
	p_data->number = i + 1;
	if (p_data->number % 2)
	{
		p_data->first_fork = g_data->forks + i;
		p_data->second_fork
			= g_data->forks + (i + 1) % g_data->number_of_philosophers;
	}
	else
	{
		p_data->first_fork
			= g_data->forks + (i + 1) % g_data->number_of_philosophers;
		p_data->second_fork = g_data->forks + i;
	}
	p_data->start_time = &(g_data->start_time);
	p_data->initial_time = 0;
	p_data->last_eat = g_data->last_eats + i;
	set_size_t(p_data->last_eat, 0);
	p_data->any_death = &(g_data->any_death);
	p_data->finished = &(g_data->finished);
	p_data->write_mutex = &(g_data->write_mutex);
}
